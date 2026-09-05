# 구현 현황

검토일: 2026-09-05. C++ 소스와 설정 파일을 정적으로 대조한 기록입니다.

이 문서는 현재 구현 사실과 연결 누락을 기록합니다. 제품 기획, 기능 우선순위, 1차 목표를 정하지 않습니다. 구조와 위치는 [프로젝트 구조](PROJECT_MAP.md), 준수할 개발 규칙은 [개발 규칙](CONVENTIONS.md)을 참고합니다.

## 판정 기준

- **코드 경로 확인:** 함수 구현과 C++ 호출 관계를 확인함. 실행 성공을 뜻하지 않음.
- **부분 구현:** 데이터·함수·UI 중 일부는 있지만 호출 연결이나 실제 처리가 부족함.
- **규칙 미준수:** 개발 규칙과 다른 구현을 확인함. 규칙 자체의 오류와 구분함.
- **실행 검증 필요:** 블루프린트, 에셋 또는 실제 실행을 확인해야 최종 판단할 수 있음.

C++ 호출이 없다는 기록은 블루프린트 호출까지 없다는 단정이 아닙니다. 아래 영향 설명 중 조건부 표현은 재현 완료된 버그가 아닌 코드상 위험을 뜻합니다.

## 시스템별 상태

| 영역 | 상태 | 확인한 내용 |
|---|---|---|
| 리소스 목록 | 코드 경로 확인 | DataManagerSubsystem이 무기·애니메이션·발사체 테이블을 조회하고 선택 위젯에 제공. 애니메이션은 WeaponType으로 필터링 |
| 스킬 저장·조회 | 코드 경로 확인 | SaveGameSubsystem에서 생성·로드·슬롯 저장, CurrentSkillSet에서 SkillID 조회. 최종 UI 데이터 전달은 별도 문제 존재 |
| 제작 편집 UI | 부분 구현 | 신규·수정 화면 전환, 이름 입력, 저장 API 호출. 상세값 전달과 신규 편집 초기화 문제 존재 |
| 프리뷰 | 코드 경로 확인 | 상세값을 HUD에 반영하고 필요 시 임시 PreviewSkill ID를 부여해 공통 SkillComponent 호출 |
| 몽타주·노티파이 | 코드 경로 확인 | 재생 전에 CurrentSkillData 설정. 선택 목록은 유효한 트리거 노티파이만 표시. 이름 일치 시 발사체 생성 |
| 발사체 전투 연결 | 부분 구현 | 이동·충돌·소멸은 구현. 스킬 데이터 전달과 충돌 피해·상태이상 처리는 미연결 |
| 근접·광역 | 부분 구현 | 근접 노티파이에서 타격 함수를 호출하지 않음. 광역 상태이상 함수도 트리거 노티파이의 활성 경로에서 호출하지 않음 |
| 상태이상 UI·전투 | 부분 구현 / 규칙 미준수 | 카드값 전달 누락, 타이머·이동 복구·멀티캐스트 처리 문제 존재 |
| 훈련장 UI·슬롯 | 부분 구현 | UI 생성 코드와 슬롯 API는 있으나 핵심 핸들러 및 저장 스킬 장착 연결 미완성 |
| 네트워크·쿨다운 | 부분 구현 / 규칙 미준수 | RPC 골격과 쿨다운 기록은 있음. 권한 보유자의 직접 실행에 쿨다운 검사 누락 |

## 데이터와 저장

근거: [리소스 조회](../Source/SkillMaker/Game/SKDataManagerSubsystem.cpp), [저장 서브시스템](../Source/SkillMaker/Game/SKSaveGameSubsystem.cpp), [SaveGame 객체](../Source/SkillMaker/Save/SKPlayerSkillSave.cpp).

- `FSKWeaponData`, `FSKAnimationData`, `FSKProjectileData`는 현재 조회에 사용하는 데이터 테이블 행 구조체입니다. 레거시로 분류하는 것은 `USKDataManager`와 `USKSkillManager`입니다.
- 스킬 목록과 단일 스킬 조회·저장은 `CurrentSkillSet.Skills`를 사용합니다. 키는 `SkillID`이며 표시 이름은 `SkillName`입니다.
- 신규 저장 시 ID가 없으면 메인 위젯이 GUID를 생성합니다. 기존 ID가 있으면 같은 키에 저장합니다. 사용자가 ID를 직접 입력하는 C++ UI 경로는 없습니다.
- 기본 슬롯은 [서브시스템 헤더](../Source/SkillMaker/Game/SKSaveGameSubsystem.h)의 `Test1`, UserIndex는 `0`입니다. `LoadData` 호출로 변경됩니다.
- 저장 파일이 없으면 객체를 생성하고 슬롯에 기록합니다. 파일 로드·캐스팅에 실패한 경우 `LoadData`는 경고 후 반환하며, 저장 요청의 `GetOrCreatePlayerSkillSave`에는 객체 생성 보완 경로가 있습니다.
- `PlayerSkills`는 이름을 키로 하는 스킬셋 맵입니다. `SetSkillSet`은 현재 스킬 맵을 해당 이름으로 복사합니다. Q/E/R/F 슬롯 배치를 저장하는 구조와는 별개입니다.
- `SaveAllSkillSet`은 현재 SaveGame 객체를 슬롯에 기록합니다. 이 함수가 슬롯 배치를 수집하거나 새로운 이름의 스킬셋을 생성하지는 않습니다.
- 스킬셋 생성·장착을 잇는 UI C++ 경로와 스킬 삭제 API는 확인되지 않았습니다. `DT_SkillData`에서 현재 저장소로 초기 데이터를 자동으로 주입·이관하는 활성 UI 경로도 확인되지 않았습니다.

## 제작 UI의 연결 문제

### UI-01: 상세 편집값의 최종 저장 전달

근거: [메인 위젯](../Source/SkillMaker/UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorMainWidget.cpp)의 `OnFinishSkillEditing`, `OnSaveSkillClicked`; [상세 위젯](../Source/SkillMaker/UI/UI-SkillMaker/Common/SkillSelect/SKSkillDetailWidget.cpp)의 `SaveSkillData`, `OnPreviewSkillClicked`.

- 상세 변경은 `EditingSkillData`에 반영됩니다. 메인 위젯의 편집 완료는 화면만 전환하고, 최종 저장은 HUD의 `CurrentEditingSkill`을 읽습니다.
- 프리뷰와 상세 위젯의 저장 함수는 상세값을 HUD에 전달하지만, 메인 위젯의 완료·저장 함수에서 상세 저장 함수를 호출하지 않습니다.
- 블루프린트에 별도 연결이 없다면 프리뷰 없이 저장하거나 마지막 프리뷰 이후 수정한 값이 저장에서 누락될 수 있습니다.
- 확인할 실행: 프리뷰 없이 변경 후 저장, 프리뷰 후 재수정·저장, 목록 재선택 후 값 비교.

### UI-02: 신규 편집 초기화와 저장 ID 동기화

근거: [제작 HUD](../Source/SkillMaker/UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorHUD.cpp)의 `BeginPlay`, `InitializeNewSkill`; [메인 위젯](../Source/SkillMaker/UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorMainWidget.cpp)의 `OnCreateSkillClicked`, `OnSaveSkillClicked`.

- HUD의 `BeginPlay`에서는 초기화하지만 새로 만들기 버튼은 무기 선택 화면으로만 이동합니다.
- 기존 스킬 로드 후 새로 만들기를 수행하면, 별도 초기화가 없는 한 기존 ID와 데이터가 남아 같은 키에 저장될 수 있습니다.
- 신규 저장에서 생성한 ID는 로컬 `SkillData`와 SaveGame에 기록되지만 HUD로 다시 반영되지 않습니다. 같은 신규 편집 상태를 다시 저장하면 새 ID가 생성될 수 있습니다.
- 확인할 실행: 기존 스킬 수정 후 신규 생성, 첫 저장 후 재저장, 저장 목록의 ID와 데이터 비교.

### UI-03: 상태이상 카드값 전달

근거: [상세 위젯](../Source/SkillMaker/UI/UI-SkillMaker/Common/SkillSelect/SKSkillDetailWidget.cpp)의 `PopulateStatusEffectList`, `SaveSkillDetails`, `OnStatusEffectToggled`; [상태이상 카드](../Source/SkillMaker/UI/UI-SkillMaker/Common/StatusEffectSelect/SKStatusEffectCardWidget.cpp).

- 목록은 데이터 테이블이 아닌 `EStatusEffect` 열거형 순회로 생성됩니다. 무기별 허용 목록 조회는 없습니다.
- 카드는 체크·시간·DOT·중첩 수를 자체 보관하지만 상세 위젯으로 전달하는 C++ 연결이 없습니다. 상세 저장도 `GetCurrentStatusEffectData`로 카드값을 수집하지 않습니다.
- 상세 위젯의 `OnStatusEffectToggled`는 존재하지만 C++ 호출·바인딩이 확인되지 않았습니다.
- 저장값 초기화는 일반 필드를 갱신하며, 상태이상 카드 재구성은 `NativeConstruct`에서 수행됩니다. 다른 스킬을 선택했을 때 카드 표시가 저장값과 일치하는지도 확인해야 합니다.

## 훈련장 연결 상태

근거: [훈련장 메인 위젯](../Source/SkillMaker/UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainMainWidget.cpp), [훈련장 HUD](../Source/SkillMaker/UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.cpp), [스킬 스테이션](../Source/SkillMaker/Prop/SKSkillStation.cpp), [플레이어 컨트롤러](../Source/SkillMaker/Player/SKPlayerController.cpp).

- 스킬셋 수정·생성, 스킬 생성·선택, 애니메이션 선택, 편집 완료, 저장 핸들러가 비어 있습니다. `StartSkillMaker`는 플레이어 참조만 저장합니다.
- HUD는 `MainWidgetClass`가 설정된 경우 위젯을 생성하고 HUD 참조를 전달합니다. 생성자의 기본 위젯 클래스 로드는 주석 처리되어 있고, `BeginPlay`에서 화면에 추가하지는 않습니다.
- 스테이션은 별도로 위젯을 생성해 플레이어 참조를 전달하고 화면에 추가합니다. 이 경로에는 HUD 참조 전달이 없습니다.
- 상세 위젯은 훈련장 HUD 참조를 받지만 실제 데이터 초기화·저장은 제작 HUD에 의존합니다. 공통 클래스 사용만으로 양쪽 편집이 연결된 것은 아닙니다.
- Q/E/R/F 키 매핑과 슬롯 설정·조회 API는 있습니다. 저장 스킬 조회 → 캐릭터 SkillMap 등록 → 슬롯 배치를 잇는 C++ 호출은 확인되지 않았습니다.

## 스킬 실행과 전투

### COMBAT-01: 발사체에 스킬 효과 미연결

근거: [트리거 노티파이](../Source/SkillMaker/Animation/SKSkillAnimNotify_Trigger.cpp), [발사체 구현](../Source/SkillMaker/Skill/SKProjectileActor.cpp), [발사체 헤더](../Source/SkillMaker/Skill/SKProjectileActor.h).

- 노티파이는 몽타주 재생 전에 설정된 `CurrentSkillData`의 `NotifyName`을 비교하여 발사체 클래스를 생성합니다.
- 발사체 `StartProject`에는 시전자만 전달합니다. 발사 당시의 `FSKSkillData`, 데미지, 상태이상을 복사하는 경로가 없습니다.
- 충돌 후 호출하는 `ApplyStatusEffect`의 실제 처리는 주석뿐이며 `TakeDamage` 호출도 없습니다. `ProjectileSound`는 선언되어 있으나 C++ 재생 코드가 없습니다.
- 광역 상태이상 함수는 존재하지만 노티파이에서 호출하는 부분은 주석입니다. [근접 노티파이](../Source/SkillMaker/Animation/SKSkillAnimNotify_HitCheck.cpp)도 타격 함수를 호출하지 않으며 해당 함수의 피해 적용은 미구현 표시로 남아 있습니다.
- 발사체 블루프린트에 별도 효과·사운드·충돌 처리가 있는지는 별도 검증 대상입니다.

### COMBAT-02: 상태이상 규칙 미준수

근거: [전투 컴포넌트](../Source/SkillMaker/Combat/SKCombatComponent.cpp)의 `MulticastApplyStatusEffect_Implementation`; 기준: [개발 규칙](CONVENTIONS.md)의 전투 규칙.

- 반복 피해와 해제 예약에 같은 `StatusEffectTimers[EffectType]` 핸들을 사용합니다. 타이머 분리 규칙을 충족하지 않습니다.
- Slow는 원본 속도 저장 없이 적용 시 `×0.5`, 해제 시 `×2`를 사용합니다. 중복 적용 또는 다른 속도 변경이 끼면 원래 상태 복원을 보장하지 못합니다.
- 멀티캐스트 안에서 이동 상태와 피해 타이머를 변경합니다. 서버의 상태 변경과 표현 동기화가 분리되어 있지 않습니다.
- Burn·Poison의 피해 코드는 1초마다 고정값 `5.0f`를 사용하며 `DamagePerSecond`와 중첩 설정을 읽지 않습니다. Shock·Curse의 switch 분기 처리도 없습니다.
- 체력 복제와 피해·사망 함수의 존재는 확인했지만, 발사체에서 이 경로가 사용되는 것은 아닙니다.

### NET-01: 실행 권한과 쿨다운의 경로 차이

근거: [스킬 컴포넌트](../Source/SkillMaker/Skill/SKSkillComponent.cpp), [트리거 노티파이](../Source/SkillMaker/Animation/SKSkillAnimNotify_Trigger.cpp), [발사체](../Source/SkillMaker/Skill/SKProjectileActor.cpp).

- 권한이 없는 소유자는 `ServerUseSkill`로 요청하며 서버 구현에서 `IsSkillAvailable`을 검사합니다.
- 권한을 가진 소유자는 `ClientRequestUseSkill`에서 `ExecuteSkill`을 직접 호출하며 이 분기에 쿨다운 검사가 없습니다. 단독 실행·호스트·프리뷰에서 반복 사용을 별도로 확인해야 합니다.
- 몽타주는 멀티캐스트에서 재생됩니다. 노티파이의 발사체 생성과 발사체 충돌·소멸에는 명시적인 권한 검사가 없습니다.
- 발사체 C++ 생성자에 복제 활성화 설정은 없으며 블루프린트 복제 설정은 확인하지 않았습니다. 서버 단독 생성·효과 적용이 보장된다고 문서화할 수 없습니다.

## 데이터 선언과 실행 지원의 구분

근거: [스킬 데이터](../Source/SkillMaker/Skill/SKSkillData.h), 스킬 컴포넌트·상세 위젯·노티파이·전투 컴포넌트의 사용처 검색.

| 필드 | C++에서 확인한 사용 범위 |
|---|---|
| SkillType | Attack/Buff/Debuff 선언과 UI 선택은 있음. 실행 분기는 확인되지 않음 |
| DamageValue | UI 편집·저장 데이터·프리뷰 로그. 발사체 피해 적용에 사용되지 않음 |
| MinRange / MaxRange | UI 편집·저장·로그. MaxRange는 활성 노티파이 경로에서 호출하지 않는 근접·광역 보조 함수에도 사용됨 |
| CooldownTime | 실행 후 쿨다운 기록에 사용. 상세 위젯 C++ 편집 핸들러는 확인되지 않음 |
| Cost / bCanMoveWhileChanneling | 데이터 선언·기본값 외 실행 적용이 확인되지 않음 |
| bAffectEnemies / bAffectAllies | 데이터 선언·기본값 외 대상 필터 적용이 확인되지 않음 |
| EffectNotifyNames | 배열 선언은 있지만 활성 트리거 경로는 단일 NotifyName을 비교 |
| bCanStack / MaxStack / DamagePerSecond | 데이터·일부 카드 처리는 있음. 전투 적용은 미연결 |
| bUseMoveBlendSpace / MoveSkillBlendSpace | 데이터 필드는 있지만 C++ 실행 적용이 확인되지 않음 |

## 실행 검증이 남은 항목

- 위젯 블루프린트의 BindWidget·이벤트 연결, 상세 저장 호출, 훈련장 HUD·스테이션 생성 경로.
- 맵별 월드 설정 및 GameMode 재정의. 설정 파일의 기본 맵·GameMode 문자열만으로 실제 선택 클래스를 확정하지 않음.
- 데이터 테이블 행과 리소스 참조, 몽타주의 노티파이·슬롯·AnimInstance 연결, 프리뷰의 실제 표시.
- UI-01~03의 저장·재조회, 프로세스 재시작 후 SaveGame 복원, 스킬셋·슬롯 연결.
- 패키징 후 리소스 로드. 개발일지에 기록된 문제의 해결 여부를 이번 정적 검토로 확정하지 않음.
- 호스트·클라이언트별 쿨다운, 발사체 생성 개수, 상태이상·피해·소멸의 권한과 복제.

문서만 변경한 이번 작업에서는 빌드·플레이·패키징을 실행하지 않았습니다. 후속 검증 결과는 실행 환경과 결과를 함께 기록하고, 코드 경로 확인 상태와 구분해야 합니다.

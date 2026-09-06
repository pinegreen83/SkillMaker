# 프로젝트 구조

## 문서 범위

2026-09-06 C++·설정 파일 정적 검토와 제작 UI 실행 확인을 반영했다. 이 문서는 코드의 책임과 소스 위치를 설명하며 전체 실행 흐름의 검증 완료를 뜻하지 않는다. 프로젝트를 시작한 이유와 현재 설계 원칙은 [프로젝트의 출발점과 세계관 방향](PROJECT_VISION.md), 연결 누락·규칙 미준수·실행 확인 항목은 [구현 현황](IMPLEMENTATION_STATUS.md), 개발 시 준수할 기준은 [개발 규칙](CONVENTIONS.md)을 참고한다.

클래스별 상속, 주요 필드·메서드, 델리게이트와 `BindWidget` 연결은 [코드베이스 상세 참조](CODE_REFERENCE.md)에 정리한다.

`UEEditor`와 스킬 제작 편집기는 런타임 모듈이 게임 월드에 생성하는 HUD·UMG 화면을 가리킨다. 별도 Unreal Editor 확장 모듈은 없다. 제작 UI의 주요 사용자 흐름은 실행 확인했지만 블루프린트 그래프 전체와 맵별 월드 설정은 별도로 검토하지 않았다.

## 루트 구성

- `SkillMaker.uproject`: UE 5.8 프로젝트. 런타임 모듈은 `SkillMaker`다.
- `Source/SkillMaker.Target.cs`: 게임 빌드 타깃.
- `Source/SkillMakerEditor.Target.cs`: 에디터 빌드 타깃.
- `Config/DefaultEngine.ini`: 기본 맵·GameMode·리다이렉트·렌더러·플랫폼 설정.
- `Config/DefaultInput.ini`: Enhanced Input 기본 설정.
- `Content/SkillMaker`: 프로젝트 전용 게임플레이 에셋, 맵, 데이터 테이블, 입력 에셋, UI 위젯.
- `Docs/`: 프로젝트 구조, 구현 현황, 빌드 명령, 개발 규칙, AI 작업 규칙. 문서 진입점은 [프로젝트 안내](../README.md)다.

빌드 타깃 설정:

- `Source/SkillMaker.Target.cs`: `TargetType.Game`, `BuildSettingsVersion.V7`, `EngineIncludeOrderVersion.Unreal5_8`.
- `Source/SkillMakerEditor.Target.cs`: `TargetType.Editor`, `BuildSettingsVersion.V7`, `EngineIncludeOrderVersion.Unreal5_8`.

## 모듈

`Source/SkillMaker/SkillMaker.Build.cs`에서 의존성을 설정한다.

- 공개 의존 모듈: `Core`, `CoreUObject`, `Engine`, `InputCore`, `UMG`, `Slate`, `SlateCore`, `EnhancedInput`, `GameplayTags`.
- 비공개 의존 모듈: `Niagara`.

## 기능별 영역

다음 폴더는 `Source/SkillMaker/` 아래에 있다. 상세 클래스와 파일은 다음 절에 정리했다.

| 폴더 | 역할 |
|---|---|
| `Character/` | 공통·플레이어·AI·프리뷰 캐릭터와 스킬·전투 컴포넌트 소유 |
| `Player/` | 플레이어 입력·Q/E/R/F 슬롯 및 제작 편집기 제어 |
| `Skill/` | 스킬 정의·실행·쿨다운·발사체와 레거시 스킬 매니저 |
| `Combat/` | 체력 복제·피해·상태이상·사망 |
| `Data/` | 활성 데이터 테이블 행 구조체와 레거시 데이터 매니저 |
| `Game/` | GameInstance 및 리소스 조회·SaveGame 저장 서브시스템 |
| `Save/` | 현재 스킬 맵과 이름 있는 스킬셋을 보관하는 저장 객체 |
| `GameMode/` | 제작 맵·훈련장 맵의 폰·컨트롤러·HUD 구성 |
| `UI/UI-SkillMaker/UEEditor/` | 제작 HUD·메인 위젯 |
| `UI/UI-SkillMaker/TrainingRoom/` | 훈련장 HUD·메인 위젯 |
| `UI/UI-SkillMaker/Common/` | 스킬·무기·애니메이션·발사체·상태이상 공통 선택·상세 위젯 |
| `Animation/` | 애니메이션 인스턴스 및 스킬 트리거·근접 노티파이 |
| `Prop/` | 상호작용 액터와 스킬 스테이션 |
| `Logging/` | `LogSkillMaker` 로그 범주와 `SK_LOG` 보조 매크로 |

## 소스 파일 목록

아래 경로는 별도 표시가 없으면 `Source/SkillMaker/` 기준이다.

### 모듈

- `Source/SkillMaker/SkillMaker.h/.cpp`: 모듈 진입점.
- `Source/SkillMaker/SkillMaker.Build.cs`: 런타임 모듈 의존성 목록.

### 캐릭터

- `Character/SKBaseCharacter.h/.cpp`: 기본 캐릭터 `ASKBaseCharacter`. `USKSkillComponent`·`USKCombatComponent`를 소유하고 `CurrentSkillData`를 보관하며 `UseSkill(SkillID)`를 스킬 컴포넌트로 전달한다.
- `Character/SKPlayerCharacter.h/.cpp`: 플레이어 캐릭터 `ASKPlayerCharacter`. 이동·시점과 스킬 호출 전달을 구현한다. 무기 필드가 선언되어 있으며 상호작용 보조 함수의 HUD 호출은 주석 처리되어 있다. 상호작용 대상을 보관하지는 않는다.
- `Character/SKAICharacter.h/.cpp`: `ASKBaseCharacter`를 상속하는 `ASKAICharacter`. 추가 C++ 로직은 거의 없다.
- `Character/SKPreviewCharacter.h/.cpp`: 제작 HUD가 사용하는 프리뷰 캐릭터 `ASKPreviewCharacter`.

### 플레이어 제어

- `Player/SKPlayerController.h/.cpp`: `ASKPlayerController`의 Enhanced Input 설정, 이동·시점·점프·상호작용, Q/E/R/F 슬롯 입력 전달.
- `Player/SKSkillMakerController.h/.cpp`: 제작 맵 컨트롤러 `ASKSkillMakerController`. `BeginPlay`에서 마우스 커서와 `GameAndUI` 입력 모드를 활성화한다.

### 스킬

- `Skill/SKSkillData.h`: 스킬 열거형·데이터 구조체. `FSKSkillData`는 `FTableRowBase`를 상속하고 SaveGame 저장에도 사용한다. 무기는 Gameplay Tag로, 몽타주·발사체·BlendSpace는 소프트 참조로 보관한다. 필드 선언과 실행 지원 여부는 구분한다.
- `Skill/SKSkillComponent.h/.cpp`: 스킬 맵, 쿨다운 기록, 서버 RPC, `ASKBaseCharacter`를 통한 멀티캐스트 몽타주 실행. 서버 RPC는 쿨다운을 검사하지만 권한 보유자의 직접 진입 분기는 검사를 건너뛴다.
- `Skill/SKProjectileActor.h/.cpp`: `ASKProjectileActor`의 이동·충돌·수명·시전자 참조. 스킬의 피해·효과 데이터를 전달받지 않으며 충돌 피해는 없다. 상태이상 적용은 주석만 남은 미완성 함수다.
- `Skill/SKSkillManager.h/.cpp`: 레거시 데이터 테이블 기반 `USKSkillManager`. 기존 데이터 이관을 명시적으로 수행하는 경우 외에는 신규 흐름과 연결하지 않는다.

### 전투

- `Combat/SKCombatComponent.h/.cpp`: `USKCombatComponent`의 체력 복제, 피해 적용, 상태이상 적용·해제, 사망 처리.

### 데이터

- `Data/SKWeaponData.h`: 단일 `WeaponTag`를 가진 무기 행 구조체 `FSKWeaponData`.
- `Data/SKAnimationData.h`: `CompatibleWeaponTags`와 소프트 몽타주·썸네일을 가진 애니메이션 행 구조체 `FSKAnimationData`.
- `Data/SKProjectileData.h`: 소프트 발사체 클래스와 썸네일을 가진 발사체 행 구조체 `FSKProjectileData`.
- `Data/SKDataManager.h/.cpp`: 레거시 자리표시 객체 `USKDataManager`. 현재 리소스 목록 조회는 `USKDataManagerSubsystem`을 통한다.

### 전역 시스템

- `Game/SKGameInstance.h/.cpp`: 프로젝트 GameInstance 클래스 `USKGameInstance`. 현재 `Init()`은 `Super::Init()`만 호출한다.
- `Game/SKDataManagerSubsystem.h/.cpp`: `UGameInstanceSubsystem` 기반 활성 리소스 조회 서브시스템. 기본 무기·애니메이션·발사체 테이블을 로드하고 UI에 목록을 반환한다.
- `Game/SKSaveGameSubsystem.h/.cpp`: `UGameInstanceSubsystem` 기반 활성 스킬 저장 서브시스템. `USKPlayerSkillSave`를 로드·생성하고 `SkillID`로 저장하며 목록·단일 조회 API를 제공한다.

### 저장

- `Save/SKPlayerSkillSave.h/.cpp`: SaveGame 객체 `USKPlayerSkillSave`. `FSKSkillSet`은 `SkillID -> FSKSkillData` 맵을 저장한다. 저장 객체는 현재 맵·이름 있는 스킬셋을 보관하고 설정·조회·목록 보조 함수를 제공한다.

### 게임 모드

- `GameMode/SKSkillMakerGameMode.h/.cpp`: `ASKSkillMakerGameMode`가 제작 맵의 컨트롤러·HUD 클래스를 지정한다.
- `GameMode/SKSkillTrainGameMode.h/.cpp`: `ASKSkillTrainGameMode`가 훈련장 맵의 플레이어 폰·컨트롤러·HUD 클래스를 지정한다.

### 애니메이션

- `Animation/SKPlayerAnimInstance.h/.cpp`: 플레이어 애니메이션 인스턴스. 현재 C++ 로직은 최소한의 골격이다.
- `Animation/SKSkillAnimNotify_Trigger.h/.cpp`: `USKSkillAnimNotify_Trigger`가 `CurrentSkillData`를 읽고 `NotifyTriggerName`과 `SkillData.NotifyName`을 비교해 선택된 발사체를 생성한다. 광역 효과 보조 함수는 활성 노티파이 경로에서 호출하지 않는다.
- `Animation/SKSkillAnimNotify_HitCheck.h/.cpp`: 미완성 근접 노티파이. `Notify`는 `PerformMeleeAttack`을 호출하지 않는다. 보조 함수에 스윕 검사는 있지만 피해 적용은 `TODO`로 남아 있다.

### 상호작용 오브젝트

- `Prop/SKInteractableActor.h/.cpp`: 상호작용 진입점을 갖는 기본 액터.
- `Prop/SKSkillStation.h/.cpp`: 조건이 충족되면 훈련장 위젯을 생성하고 현재 플레이어를 전달해 화면에 추가한다. 이 경로는 훈련장 HUD 참조를 지정하지 않는다.

### 제작 편집기 UI

- `UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorHUD.h/.cpp`: 메인 위젯 생성, `CurrentEditingSkill` 소유, 저장과 프리뷰 실행을 담당한다. 생성하는 프리뷰는 `BP_SKPreviewCharacter`가 아닌 네이티브 `ASKPreviewCharacter::StaticClass()`이며 스킬 컴포넌트로 실행한다.
- `UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorMainWidget.h/.cpp`: 신규·수정 상태 전환, 이름 입력, 저장 요청 전달과 선택 위젯 간 연결을 처리한다.

### 훈련장 UI

- `UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h/.cpp`: SaveGame 조회 보조 함수가 있고 클래스가 지정되면 `MainWidget`을 생성한다. 기본 클래스 로드는 주석 처리되어 있고 `BeginPlay`는 위젯을 화면에 추가하지 않는다. 선택 핸들러에서 조회 보조 함수를 호출하지 않는다.
- `UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainMainWidget.h/.cpp`: 상태·탐색 골격. 스킬·스킬셋 생성, 수정, 선택, 애니메이션 선택, 편집 완료, 저장 핸들러가 비어 있다. `StartSkillMaker`는 플레이어 참조만 저장한다.

### 공통 UI 위젯

다음 경로는 `Source/SkillMaker/UI/UI-SkillMaker/` 기준이다.

- `Common/SkillSelect/SKSkillSelectionWidget.h/.cpp`: `USKSaveGameSubsystem`에서 저장 스킬 목록을 읽는다.
- `Common/SkillSelect/SKSkillCardWidget.h/.cpp`: 저장 스킬 한 개의 카드.
- `Common/SkillSelect/SKSkillDetailWidget.h/.cpp`: 별도 스킬 원본 없이 제작 HUD의 최신값을 사용해 유형·상태이상·발사체·노티파이·프리뷰를 처리한다. 선택값은 델리게이트로 HUD에 즉시 전달하며 데미지·범위 입력은 레거시 주석 상태다.
- `Common/WeaponSelect/SKWeaponSelectionWidget.h/.cpp`: `USKDataManagerSubsystem`에서 무기 목록을 읽는다.
- `Common/WeaponSelect/SKWeaponCardWidget.h/.cpp`: 무기 행 한 개의 카드.
- `Common/AnimationSelect/SKAnimationSelectionWidget.h/.cpp`: 같은 서브시스템에서 Gameplay Tag 호환성으로 필터링한 애니메이션 목록을 읽는다.
- `Common/AnimationSelect/SKAnimationCardWidget.h/.cpp`: 애니메이션 행 한 개의 카드.
- `Common/AnimationSelect/SKAnimNotifySelectionWidget.h/.cpp`: 선택한 몽타주의 유효한 스킬 트리거 노티파이 목록을 표시한다.
- `Common/AnimationSelect/SKAnimNotifyCardWidget.h/.cpp`: 노티파이 한 개의 카드.
- `Common/ProjectileSelect/SKProjectileSelectionWidget.h/.cpp`: `USKDataManagerSubsystem`에서 발사체 목록을 읽고 카드 클릭 시 즉시 선택을 적용한다.
- `Common/ProjectileSelect/SKProjectileCardWidget.h/.cpp`: 발사체 행 한 개의 카드.
- `Common/StatusEffectSelect/SKStatusEffectCardWidget.h/.cpp`: 옵션 하나의 체크박스·수치를 보관하고 값 변경 델리게이트를 발행한다. 상세 위젯은 열거형으로 카드를 만들고 이벤트를 현재 스킬 데이터에 반영한다.

## 데이터 소유와 호출 경로

- 리소스 목록: 선택 위젯 → `USKDataManagerSubsystem` → `FSKWeaponData` / `FSKAnimationData` / `FSKProjectileData` 테이블 행. 이 구조체들은 활성 코드이며 `USKDataManager` 객체는 레거시다.
- 편집: 제작 HUD가 `CurrentEditingSkill` 원본을 단독 소유한다. 상세 위젯은 이벤트 순간 HUD 최신값의 지역 사본에서 선택 필드만 바꾸고 `OnSkillDetailChanged`로 HUD를 즉시 갱신한다. HUD의 `OnEditingSkillChanged`는 현재 선택 요약 UI 연결 지점이다.
- 저장: 메인 위젯은 이름과 저장 요청만 HUD에 전달한다. HUD가 필수값과 저장 기반을 검증하고 ID를 확정한 뒤 `USKSaveGameSubsystem::SaveSkillData` → `USKPlayerSkillSave::SetSkillData` → `CurrentSkillSet.Skills` → 슬롯 기록으로 이어진다.
- 이름 있는 스킬셋: `PlayerSkills`는 스킬셋 이름을 `FSKSkillSet`에 연결한다. `SetSkillSet`은 현재 맵을 해당 이름의 항목에 복사한다. 슬롯 순서는 PlayerController가 별도로 보관하며 이 저장 구조에는 표현되지 않는다.
- 실행: `ASKBaseCharacter::UseSkill` → `ClientRequestUseSkill` → 권한이 없으면 서버 RPC, 있으면 `ExecuteSkill` 직접 호출 → 멀티캐스트 몽타주 → 트리거 노티파이 → 발사체 생성. 발사체의 시각 효과·사운드·충돌·소멸은 연결되어 있고 피해와 상태이상 처리는 미연결 상태다.
- 프리뷰: HUD가 임시 또는 저장 스킬을 네이티브 프리뷰 캐릭터의 `SkillMap`에 넣고 같은 캐릭터·컴포넌트 진입점을 사용한다. 몽타주와 선택한 트리거 시점의 발사체 이펙트·사운드 출력, 타겟 충돌을 실행 확인했다.

## 주요 에셋

- 맵:
  - `/Game/SkillMaker/Map/SkillTrainingMap`
  - `/Game/SkillMaker/Map/SkillMakingMap`
  - `/Game/SkillMaker/Map/TrainingMap`
- 데이터 테이블:
  - `DT_SkillData`
  - `DT_WeaponData`
  - `DT_AnimationData`
  - `DT_ProjectileData`
- 블루프린트:
  - `BP_SKPlayerCharacter`
  - `BP_SKPreviewCharacter`
  - `BP_SKAI`
  - `BP_SKPlayerController`
  - `BP_SKSkillMakerController`
  - `BP_FireBall`
  - `BP_WaterBall`
- 입력:
  - `IMC_Default`
  - `IA_Move`, `IA_Look`, `IA_Jump`
  - `IA_SkillQ`, `IA_SkillE`, `IA_SkillR`, `IA_SkillF`
- UI:
  - 제작 편집기: `WBP_SKSkillMakerEditorMain`. `WBP_SKSkillMakeEditorHUD` 에셋도 있지만 현재 C++ HUD는 전자를 직접 로드한다.
  - 훈련장: `WBP_SKSkillMakerTrainMain`, `WBP_SKSkillMakerTrainHUD`.
  - 공통 카드·선택 위젯: `Content/SkillMaker/UI` 아래에 있다.

## 설정과 에셋 연결

- `DefaultEngine.ini`의 `GameDefaultMap`과 `EditorStartupMap`은 `/Game/SkillMaker/Map/SkillTrainingMap`으로 설정되어 있다.
- 같은 파일의 `GlobalDefaultGameMode` 값은 `/Script/SkillMaker.SkillMakerGameMode`다.
- `ASKSkillTrainGameMode`는 `BP_SKPlayerCharacter`, `BP_SKPlayerController`, `ASKSkillMakerTrainHUD`를 지정한다.
- `ASKSkillMakerGameMode`는 `BP_SKSkillMakerController`와 `ASKSkillMakerEditorHUD`를 지정한다.
- `SkillMakingMap`의 명령행 로드에서 `ASKSkillMakerGameMode` 사용과 제작 HUD·메인 위젯 생성을 확인했다. 다른 맵의 GameMode 재정의는 Unreal Editor의 월드 설정에서 별도로 확인해야 한다.
- `ASKSkillMakerEditorHUD`는 `/Game/SkillMaker/UI/Editor/WBP_SKSkillMakerEditorMain.WBP_SKSkillMakerEditorMain_C`를 로드하고 `BeginPlay()`에서 화면에 추가한다.
- `USKDataManagerSubsystem`은 테이블 참조가 비어 있으면 `/Game/SkillMaker/Data/DT_WeaponData`, `/Game/SkillMaker/Data/DT_AnimationData`, `/Game/SkillMaker/Data/DT_ProjectileData`에서 기본 테이블을 로드한다.
- 제작 스킬은 `USKSaveGameSubsystem`과 `USKPlayerSkillSave`를 통해 `SkillID`로 저장·조회한다. `DT_SkillData`는 현재 UI 저장소가 아니며 활성 C++ UI 경로에서 자동 초기 데이터 주입·이관 처리는 확인되지 않았다.
- 프리뷰는 필요 시 임시 `PreviewSkill` ID를 부여하고 데이터를 로그에 기록한 뒤 프리뷰 캐릭터의 스킬 맵에 넣어 `USKSkillComponent`로 실행한다.
- 발사체 카드는 클릭 시 선택하고 이벤트를 즉시 전파한다. 레거시 확인 버튼의 C++ 바인딩과 핸들러는 제거됐다.
- 애님 노티파이 목록에는 유효한 `NotifyTriggerName`을 가진 `USKSkillAnimNotify_Trigger`만 표시하며 `None`은 제외한다.
- `Config/DefaultEngine.ini`에는 이전 클래스·프로퍼티 이름의 리다이렉트가 있다. 명시적으로 이관이 완료된 경우 외에는 유지한다.

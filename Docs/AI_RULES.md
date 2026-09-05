# AI 작업 규칙

## 문서 언어와 역할

- 프로젝트 문서의 기본 언어는 한국어다. 제목·본문·표·링크 설명은 한국어로 작성한다.
- 코드, 명령어, 클래스·함수·변수명, 파일명·경로, 설정 키, API 및 제품 고유 명칭은 원문을 유지한다. 설명 문장은 한국어로 쓰며 기존 문서를 갱신할 때도 같은 기준을 적용한다.
- [프로젝트 안내](../README.md)부터 읽는다. 프로젝트를 시작한 이유와 세계관 관점은 [프로젝트의 출발점과 세계관 방향](PROJECT_VISION.md), 최초 구현 범위와 완료 조건은 [프로토타입 범위](PROTOTYPE_SCOPE.md), 구조는 [프로젝트 구조](PROJECT_MAP.md), 알려진 연결 누락과 미검증 동작은 [구현 현황](IMPLEMENTATION_STATUS.md)을 참고한다.
- 프로젝트의 출발점과 세계관 문서에는 동기·문제의식·판단 기준과 여기서 직접 도출된 설계 원칙을 함께 기록한다. 설계가 나온 이유를 이해하는 데 필요한 구현 방향은 남기되, 검증되지 않은 정확한 계산식과 임시 균형 수치를 확정된 내용처럼 섞지 않는다.
- [개발 규칙](CONVENTIONS.md)은 수정 시 준수할 기준이다. 기존 코드의 준수 여부를 증명하는 문서가 아니다. 규칙 미준수는 구현 현황에 기록하며 미완성 코드에 맞춰 규칙을 약화하지 않는다.
- C++ 정적 분석 결과와 블루프린트·실행 검증 결과를 구분한다. 함수나 필드가 선언되어 있다는 이유만으로 UI나 게임플레이에서 사용된다고 판단하지 않는다.
- 구현 검토 결과에는 근거 소스 파일과 함수명을 기록한다. 개발일지는 과거 기록으로 유지하며 명시적인 결정 없이 당시 아이디어를 현재 요구사항으로 간주하지 않는다.
- 문서 검토 요청을 코드 수정 허가로 해석하지 않는다. 사용자가 요청한 변경 범위와 코드 승인 지침을 따른다.
- 기획의 최종 결정은 사용자가 내린다. AI는 코드와 문서 근거를 바탕으로 구체적인 권장안을 제시하고, 확정이 필요한 항목은 동의를 묻는 표현 대신 해당 안에 대한 승인을 요청한다. 승인되지 않은 제안은 확정 사항으로 문서화하지 않는다.

## 프로젝트 정보

- Unreal Engine 5.8 C++ 프로젝트: `SkillMaker`.
- 주 모듈: `Source/SkillMaker`.
- 런타임 모듈만 있다. 명시적인 요청 없이 새 플러그인이나 모듈을 추가하지 않는다.
- 스킬 생성·편집, SaveGame 저장, 훈련장 UI 골격, 발사체·전투 컴포넌트가 있다. 전체 연결은 미완성이므로 구현 현황을 확인한다.
- 스킬 제작 편집기는 런타임 HUD·UMG 화면이며 별도 Unreal Editor 확장 모듈이 아니다.
- 현재 기본 맵은 `/Game/SkillMaker/Map/SkillTrainingMap`이다.

## 수정 규칙

- 기존 사용자 변경을 보존한다. 위험한 수정 전에 `git status --short`를 확인한다.
- 명시적인 요청 없이 `Content/**/*.uasset`, `*.umap`, `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.idea/`, 자동 생성 IDE 파일을 수정하지 않는다.
- C++ 변경은 `Source/SkillMaker`, 설정 변경은 `Config`를 우선 사용한다.
- UMG 바인딩 위젯의 `UPROPERTY(meta = (BindWidget))` 이름을 위젯 블루프린트의 이름과 일치시킨다.
- 기존 `SK` 접두사 명명 규칙을 사용한다.
- 기존 코드에서 `TObjectPtr<>`를 사용하는 UObject 참조에는 같은 방식을 적용한다.
- 다른 로그 범주를 일관되게 쓰는 기존 코드를 수정하는 경우 외에는 `SK_LOG(LogSkillMaker, ...)`를 사용한다.
- 기능 구현이나 버그 수정 중 광범위한 리팩터링을 피한다.

## Unreal/C++ 규칙

- 리플렉션 타입을 추가·변경한 헤더는 대응하는 `.generated.h`를 마지막으로 포함한다.
- UI나 에셋에서 사용하는 타입은 `UCLASS`, `USTRUCT`, `UENUM`, `UFUNCTION`, `UPROPERTY` 메타데이터를 블루프린트에서 사용 가능하게 유지한다.
- 데이터 테이블 행 구조체는 `FTableRowBase`를 상속한다.
- 네트워크 API의 기존 책임을 유지한다. 권한 보유자 분기의 쿨다운 검사 누락은 구현 현황 NET-01을 참고한다.
  - 클라이언트 요청 진입점: `ClientRequestUseSkill`
  - 서버 검증·실행: `ServerUseSkill`
  - 멀티캐스트 실행: `MulticastExecuteSkill`
- GameMode의 기본 클래스 지정 등 기존 패턴에 해당하는 경우 외에는 에셋 경로를 하드코딩하지 않는다.
- UI·데이터 흐름에 필요한 에셋은 블루프린트에서 지정하거나 서브시스템에서 관리하는 참조를 우선한다.

## 데이터·저장·스킬 규칙

- 리소스 데이터는 `USKDataManagerSubsystem`을 통해 데이터 테이블에서 읽는다.
- `FSKWeaponData`, `FSKAnimationData`, `FSKProjectileData`는 현재 사용하는 행 구조체다. 레거시 매니저와 혼동하지 않는다.
- 데이터 테이블 접근에 `USKDataManager::Get()`이나 GameInstance 직접 캐스팅을 사용하지 않는다.
- 런타임에서 제작한 스킬은 `USKSaveGameSubsystem`과 `USKPlayerSkillSave`를 통해 저장한다.
- `SkillID`는 생성된 스킬의 저장·수정·조회에 쓰는 고정 키다.
- 현재 저장 UI는 `SkillID`가 `None`이면 GUID를 생성하고 기존 ID가 있으면 유지한다. `SkillName`은 사용자가 입력하는 표시명이다. 편집 상태 동기화 문제는 구현 현황 UI-02를 참고한다.
- 저장은 Unreal 저장 API인 `UGameplayStatics::SaveGameToSlot`, `LoadGameFromSlot`, `DoesSaveGameExist`, `CreateSaveGameObject`를 사용한다.
- `DT_SkillData`는 초기·샘플 데이터로 남길 수 있지만 현재 제작 UI에서 만든 스킬의 저장소로 사용하지 않는다.
- `USKSkillManager`는 레거시 데이터 테이블 기반 코드다. 신규 UI 저장·조회 경로가 의존하게 만들지 않는다.
- 스킬 ID와 스킬셋 이름에 쓰는 `FName` 키의 의미를 보존한다. 의미를 바꾸면 기존 저장 데이터나 데이터 테이블 행을 찾지 못할 수 있다.

## 스킬 프리뷰 규칙

- 프리뷰는 일반 스킬과 같은 `USKSkillComponent` 실행 경로를 사용한다.
- 저장된 `SkillID`가 없는 프리뷰 스킬에는 임시 `PreviewSkill` ID를 부여할 수 있다.
- 몽타주 재생 전에 `ASKBaseCharacter::CurrentSkillData`를 실행할 `FSKSkillData`로 설정한다.
- 애님 노티파이 선택 목록에는 유효한 `NotifyTriggerName`을 가진 `USKSkillAnimNotify_Trigger`만 노출한다.
- 현재 발사체 카드는 클릭 시 선택을 즉시 적용한다. 확인 버튼에서 선택 이벤트를 중복 전파하지 않는다.

## 검증

- 문서만 변경했다면 코드 근거, 로컬 링크, 내용의 일관성, 변경 범위를 확인한다. 실제 실행하지 않은 빌드나 테스트를 통과했다고 보고하지 않는다.
- C++ 변경 후에는 최소한 에디터 빌드를 실행한다.
- UI 변경 후에는 에디터에서 해당 위젯 블루프린트의 바인딩을 확인한다.
- 스킬·전투·저장 변경 후에는 관련 범위에 따라 `Content/SkillMaker/Map/SkillTrainingMap.umap`과 `Content/SkillMaker/Map/SkillMakingMap.umap`의 제작 흐름을 검증한다.
- 데이터 테이블 관련 변경 후에는 에디터에서 `DT_SkillData`, `DT_WeaponData`, `DT_AnimationData`, `DT_ProjectileData` 참조를 확인한다.

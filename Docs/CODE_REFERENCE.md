# 코드베이스 상세 참조

검토 기준일: 2026-09-07
검토 범위: `Source/SkillMaker`, `SkillMaker.uproject`, `Config/DefaultEngine.ini`, `Config/DefaultInput.ini`, `Config/DefaultGameplayTags.ini`의 정적 분석

이 문서는 현재 코드에 선언된 타입, 클래스 책임, 주요 필드와 메서드, 델리게이트 연결 및 데이터 흐름을 빠르게 찾기 위한 참조다. 실제 구현 상태와 알려진 결함은 [구현 현황](IMPLEMENTATION_STATUS.md), 큰 폴더 구조와 에셋 위치는 [프로젝트 구조](PROJECT_MAP.md), 변경 규칙은 [개발 규칙](CONVENTIONS.md)을 함께 참고한다.

블루프린트 내부 그래프 전체, 맵의 World Settings와 데이터 테이블 전체 행 값은 이번 정적 분석에 포함하지 않았다. 다만 `WBP_SKSkillDetail`의 `GeneralTabPanel > GeneralTabScrollBox > GeneralTabContent` 계층과 네 탭 패널 바인딩은 명령행 로드 로그와 사용자 실행으로 확인했다. 그 밖의 `BindWidget`, 클래스 참조와 C++ 호출 기록은 실제 블루프린트 연결 성공을 뜻하지 않는다.

## 모듈과 의존성

- 프로젝트: Unreal Engine 5.8, 런타임 모듈 `SkillMaker` 하나
- 진입점: `Source/SkillMaker/SkillMaker.cpp`의 `IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, SkillMaker, "SkillMaker")`
- `Source/SkillMaker/SkillMaker.h`는 현재 `CoreMinimal.h`만 포함하며 별도 모듈 클래스나 로그 카테고리를 선언하지 않는다.
- 공개 모듈: `Core`, `CoreUObject`, `Engine`, `InputCore`, `UMG`, `Slate`, `SlateCore`, `EnhancedInput`, `GameplayTags`
- 비공개 모듈: `Niagara`
- 별도 Unreal Editor 확장 모듈은 없다. 소스 경로의 `UEEditor`는 런타임 제작 UI를 뜻한다.
- 무기 태그는 `Config/DefaultGameplayTags.ini`의 `Weapon` 계층에서 등록한다.

기본 설정:

- `GameDefaultMap`, `EditorStartupMap`: `/Game/SkillMaker/Map/SkillTrainingMap`
- `GlobalDefaultGameMode`: `/Script/SkillMaker.SkillMakerGameMode`
- 맵별 GameMode 재정의는 에디터에서 확인해야 한다.

## 주요 상속 구조

```text
ACharacter
└─ ASKBaseCharacter
   ├─ ASKPlayerCharacter
   ├─ ASKPreviewCharacter
   └─ ASKAICharacter

APlayerController
├─ ASKPlayerController
└─ ASKSkillMakerController

AGameModeBase
├─ ASKSkillMakerGameMode
└─ ASKSkillTrainGameMode

AHUD
├─ ASKSkillMakerEditorHUD
└─ ASKSkillMakerTrainHUD

AActor
├─ ASKProjectileActor
└─ ASKInteractableActor
   └─ ASKSkillStation

UActorComponent
├─ USKSkillComponent
└─ USKCombatComponent

UUserWidget
├─ USKSkillMakerEditorMainWidget
├─ USKSkillMakerTrainMainWidget
├─ USKMapNavigationWidget
├─ USKSkillSlotAssignmentWidget
├─ USKSkillDetailWidget
├─ USKSkillSelectionWidget
├─ USKSkillCardWidget
├─ USKWeaponSelectionWidget
├─ USKWeaponCardWidget
├─ USKAnimationSelectionWidget
├─ USKAnimationCardWidget
├─ USKAnimNotifySelectionWidget
├─ USKAnimNotifyCardWidget
├─ USKProjectileSelectionWidget
├─ USKProjectileCardWidget
└─ USKStatusEffectCardWidget

UAnimInstance
└─ USKPlayerAnimInstance

UAnimNotify
├─ USKSkillAnimNotify_Trigger
└─ USKSkillAnimNotify_HitCheck

UGameInstance
└─ USKGameInstance

UGameInstanceSubsystem
├─ USKDataManagerSubsystem
└─ USKSaveGameSubsystem

USaveGame
└─ USKPlayerSkillSave

UObject
├─ USKDataManager       (레거시 자리표시)
└─ USKSkillManager      (레거시 데이터 테이블 매니저)
```

## 핵심 데이터 타입

### 스킬 데이터

근거: `Source/SkillMaker/Skill/SKSkillData.h`

#### `ESkillType`

- 값: `Attack`, `Buff`, `Debuff`
- 상세 UI에서 문자열 옵션과 수동으로 대응한다.
- 실행 코드에서 타입별 분기는 확인되지 않는다.

#### `EStatusEffect`

- 값: `None`, `Stun`, `Slow`, `Burn`, `Freeze`, `Poison`, `Shock`, `Curse`
- `ENUM_RANGE_BY_COUNT`로 UI 목록 순회에 사용한다.

#### `FStatusEffectData`

| 필드 | 타입 | 의미/사용 |
|---|---|---|
| `EffectType` | `EStatusEffect` | 상태이상 종류 |
| `Duration` | `float` | 지속 시간 |
| `DamagePerSecond` | `float` | DOT 값. 현재 전투 처리는 읽지 않음 |
| `bCanStack` | `bool` | 중첩 허용. 현재 전투 처리는 읽지 않음 |
| `MaxStack` | `int32` | 최대 중첩. 현재 전투 처리는 읽지 않음 |

모든 필드는 `SaveGame`이다.

#### `FSKSkillData`

`FTableRowBase`를 상속하고 제작 중 데이터, SaveGame 데이터, 실행 RPC 데이터에 함께 사용된다.

| 필드 | 타입 | 현재 사용 |
|---|---|---|
| `SkillID` | `FName` | 저장·조회·실행의 고정 키 |
| `SkillName` | `FString` | 사용자 표시명 |
| `SkillType` | `ESkillType` | 상세 UI 편집. 실행 분기 없음 |
| `TargetingType` | `ESKTargetingType` | 기본값 `NonTarget`. 타겟팅 실행 분기는 아직 없음 |
| `ElementTag` | `FGameplayTag` | 상세 UI에서 선택한 단일 `Element` 계층 속성 |
| `WeaponTag` | `FGameplayTag` | 무기 선택과 애니메이션 호환 필터 |
| `SkillMontage` | `TSoftObjectPtr<UAnimMontage>` | 선택·저장 중 소프트 참조, 상세/실행 시 로드 |
| `SkillDuration` | `float` | 몽타주 길이 기록 |
| `bCanMoveWhileChanneling` | `bool` | 실행 연결 확인되지 않음 |
| `CooldownTime` | `float` | `USKSkillComponent` 쿨다운 |
| `Cost` | `float` | 실행 연결 확인되지 않음 |
| `DamageValue` | `float` | 기본 탭 입력, 저장·프리뷰 로그, 발사체 충격 스냅샷과 현재 명중 피해 계산의 기본값. 장기 계산 규칙으로 교체 예정 |
| `EffectNotifyNames` | `TArray<FName>` | 선언되어 있으나 활성 실행은 `NotifyName` 사용 |
| `StatusEffects` | `TArray<FStatusEffectData>` | 발사체 충격 스냅샷을 거쳐 전투 컴포넌트까지 전달·기록. 실제 효과 적용은 미연결 |
| `bAffectEnemies` | `bool` | 대상 필터 미연결 |
| `bAffectAllies` | `bool` | 대상 필터 미연결 |
| `ProjectileActor` | `TSoftClassPtr<ASKProjectileActor>` | 선택된 이펙트·사운드 발사체 클래스의 소프트 참조 |
| `NotifyName` | `FName` | 실행할 `USKSkillAnimNotify_Trigger` 이름 |
| `MinRange` | `float` | 저장 호환용 필드. 현재 상세 UI에서는 숨김, 활성 발사체 경로에서 미사용 |
| `MaxRange` | `float` | 저장 호환용 필드. 현재 상세 UI에서는 숨김, 비활성 근접·광역 보조 함수에서 사용 |
| `bUseMoveBlendSpace` | `bool` | 실행 연결 확인되지 않음 |
| `MoveSkillBlendSpace` | `TSoftObjectPtr<UBlendSpace>` | 실행 연결 확인되지 않음 |

### 리소스 데이터 테이블 행

#### `FSKWeaponData`

근거: `Source/SkillMaker/Data/SKWeaponData.h`

- 부모: `FTableRowBase`
- 필드: `WeaponName: FString`, `WeaponTag: FGameplayTag`, `WeaponThumbnail: TSoftObjectPtr<UTexture2D>`
- 사용처: `USKDataManagerSubsystem::GetWeaponList`, `USKWeaponSelectionWidget`
- `WeaponName`은 카드 표시, `WeaponTag`는 선택 결과와 애니메이션 필터에 사용한다.

#### `FSKAnimationData`

근거: `Source/SkillMaker/Data/SKAnimationData.h`

- 부모: `FTableRowBase`
- 필드: `AnimationName: FString`, `CompatibleWeaponTags: FGameplayTagContainer`, `AnimationThumbnail: TSoftObjectPtr<UTexture2D>`, `AnimationMontage: TSoftObjectPtr<UAnimMontage>`
- 사용처: `USKDataManagerSubsystem::GetAnimationsForWeapon`, `USKAnimationSelectionWidget`
- 카드는 `AnimationName`을 표시하고 몽타주와 썸네일을 소프트 참조로 유지한다.
- 선택 무기 태그가 호환 태그와 같거나 그 하위이면 표시한다. `Weapon`은 전체, `Weapon.Melee.Sword`는 검 계열 공통 범위로 사용할 수 있다.

#### `FSKProjectileData`

근거: `Source/SkillMaker/Data/SKProjectileData.h`

- 부모: `FTableRowBase`
- 필드: `ProjectileName`, `Description`, `Thumbnail: TSoftObjectPtr<UTexture2D>`, `ProjectileActor: TSoftClassPtr<ASKProjectileActor>`, `SupportedElementTags: FGameplayTagContainer`
- `ProjectileActor`가 이펙트·사운드·이동·충돌 구현의 묶음 역할을 한다.
- 발사체 카드는 클래스를 로드하지 않고 데이터의 `ProjectileName`을 표시한다. 지원 속성이 비어 있는 기존 행은 모든 속성과 호환된다.

#### `FSKElementData`

근거: `Source/SkillMaker/Data/SKElementData.h`

- 필드: `ElementTag`, `DisplayName`, `Description`, `DisplayColor`
- 프로토타입의 화염·물·바람·대지 카드 표시와 마우스 오버 설명에 사용한다.

#### 조회 결과 래퍼

`SKDataManagerSubsystem.h`에 `FSKWeaponRow`, `FSKAnimationRow`, `FSKProjectileRow`가 있으며 각각 `RowName`과 실제 `Data` 복사본을 보관한다.

### 저장 타입

#### `FSKSkillSet`

- 필드: `Skills: TMap<FName, FSKSkillData>`
- 키는 `SkillID`다.

#### `USKPlayerSkillSave`

- 부모: `USaveGame`
- `CurrentSkillSet`: 현재 제작 스킬 맵
- `PlayerSkills`: 이름을 키로 저장한 여러 `FSKSkillSet`
- `SetSkillSet(Name)`은 현재 맵을 `PlayerSkills`에 복사한다. Q/E/R/F 슬롯 저장과는 별개다.

## 게임 전역 시스템

### `USKDataManagerSubsystem`

근거: `Source/SkillMaker/Game/SKDataManagerSubsystem.h/.cpp`

- 부모: `UGameInstanceSubsystem`
- 책임: 리소스 데이터 테이블 로드와 목록 조회
- 소유 필드: `WeaponDataTable`, `AnimationDataTable`, `ProjectileDataTable`
- 초기화: `Initialize(Collection)` → `LoadDefaultDataTables()`
- 기본 경로:
  - `/Game/SkillMaker/Data/DT_WeaponData`
  - `/Game/SkillMaker/Data/DT_AnimationData`
  - `/Game/SkillMaker/Data/DT_ProjectileData`

주요 API:

| 메서드 | 반환/입력 | 동작 |
|---|---|---|
| `GetWeaponList()` | `TArray<FSKWeaponRow>` | 전체 무기 행 복사 |
| `GetAnimationsForWeapon(WeaponTag)` | `TArray<FSKAnimationRow>`, `FGameplayTag` | 호환 컨테이너의 같거나 상위인 태그가 있으면 애니메이션 반환 |
| `GetProjectileList()` | `TArray<FSKProjectileRow>` | 전체 발사체 행 복사 |
| `GetProjectilesForElement(ElementTag)` | `TArray<FSKProjectileRow>` | 선택 속성을 지원하거나 지원 태그가 비어 있는 발사체 행 조회 |

호출자: 무기·애니메이션·발사체 선택 위젯. 레거시 `USKDataManager`는 사용하지 않는다.

### `USKSaveGameSubsystem`

근거: `Source/SkillMaker/Game/SKSaveGameSubsystem.h/.cpp`

- 부모: `UGameInstanceSubsystem`
- 책임: `USKPlayerSkillSave` 생성·로드·디스크 기록과 조회 중계
- 필드: `PlayerSkillSave`, `DefaultSlotName = "Test1"`, `DefaultUserIndex = 0`
- 초기화: `Initialize(Collection)` → `LoadData(DefaultSlotName, DefaultUserIndex)`

주요 API:

| 메서드 | 의미 |
|---|---|
| `SaveSkillData(InSkillID, InSaveData)` | ID 검증 후 현재 스킬셋에 넣고 슬롯 기록 |
| `SaveAllSkillSet()` | 현재 SaveGame 객체를 슬롯에 기록 |
| `GetSavedSkillList()` | 현재 스킬셋 값 배열 반환 |
| `GetSkillDataByID(InSkillID, OutSkillData)` | 현재 스킬셋에서 단일 조회 |
| `GetSkillSet(InSkillID)` | 이름 있는 `PlayerSkills` 맵 조회. 파라미터명은 실제 의미와 다름 |
| `LoadData(SlotName, UserIndex)` | 슬롯 설정 후 로드하거나 새 저장 객체 생성 |
| `GetOrCreatePlayerSkillSave()` | 내부 객체 보장 |
| `WritePlayerSkillSaveToSlot()` | `SaveGameToSlot` 실행 |

호출자: 제작 메인 위젯, 제작/훈련 HUD, 스킬 선택 위젯.

### `USKGameInstance`

- 부모: `UGameInstance`
- `Init()`은 `Super::Init()`만 호출한다.
- 데이터와 저장은 GameInstance 자체가 아니라 두 서브시스템이 담당한다.

### 레거시 객체

#### `USKDataManager`

- 부모: `UObject`
- 구현 없는 자리표시 객체다.
- 신규 코드에서 사용하지 않는다.

#### `USKSkillManager`

- 부모: `UObject`
- 정적 `Instance`와 `Get()`을 사용하는 레거시 싱글턴 형태다.
- `SkillDataTable`에서 목록·단일 조회·행 추가/수정을 수행한다.
- `GetSkillDataByID`는 `FName SkillID`를 받지만 실제로 `Row->SkillName == SkillID`를 비교한다.
- 현재 제작 UI의 영구 저장 경로는 이 객체가 아니라 `USKSaveGameSubsystem`이다.

## 캐릭터와 입력

### `ASKBaseCharacter`

근거: `Source/SkillMaker/Character/SKBaseCharacter.h/.cpp`

- 부모: `ACharacter`
- 책임: 공통 캐릭터 설정, 스킬/전투 컴포넌트 소유, 실행 중 스킬 데이터 보관
- 컴포넌트: `SkillComponent: USKSkillComponent`, `CombatComponent: USKCombatComponent`
- 상태: `CurrentSkillData: TOptional<FSKSkillData>`

주요 API:

- `GetCurrentSkillData()` / `SetCurrentSkillData(SkillData)`: 노티파이가 읽는 현재 실행 데이터
- `SetSkillDataInMap(SkillData)`: 스킬 한 개를 컴포넌트에 등록
- `SetSkillDataInComponent(SkillDataMap)`: 스킬 맵 전체 교체
- `UseSkill(SkillID)`: `SkillComponent->ClientRequestUseSkill` 호출

### `ASKPlayerCharacter`

- 부모: `ASKBaseCharacter`
- 필드: `CameraBoom`, `FollowCamera`, 선언만 된 `Weapon`, `WeaponName`
- `Move(Value)`, `Look(Value)`: 컨트롤러 입력 수행
- `UseSkill(SkillID)`: 부모 호출
- `SetInteractableTarget(Target)`, `ClearInteractableTarget(Target)`: 훈련 HUD를 찾지만 실제 표시 호출은 주석 처리됨. 대상 자체도 저장하지 않음

### `ASKPreviewCharacter`

- 부모: `ASKBaseCharacter`
- 필드: `PreviewMontage`, `CameraBoom`, `FollowCamera`
- API: `PlayPreviewAnimation`, `SetPreviewAnimationTime`, `GetPreviewAnimationTime`, `GetPreviewMontage`, `SetPreviewMontage`
- 제작 HUD의 현재 프리뷰는 이 직접 API보다 부모의 `SetSkillDataInMap` → `UseSkill` 공통 실행 경로를 사용한다.

### `ASKAICharacter`

- 부모: `ASKBaseCharacter`
- 추가 C++ 필드와 메서드가 없다.

### `ASKPlayerController`

근거: `Source/SkillMaker/Player/SKPlayerController.h/.cpp`

- 부모: `APlayerController`
- 책임: Enhanced Input 설정, 이동·시점·점프·상호작용·Q/E/R/F 스킬 입력
- 입력 필드: `DefaultMappingContext`, `MoveAction`, `LookAction`, `JumpAction`, `InteractAction`, 기존 에셋 호환용 `SkillActionQ/E/R/F`, `OpenSkillChangeUIAction`
- 슬롯 필드:
  - `SkillSlots: TArray<FName>`: 4개 슬롯의 SkillID
  - `KeyToSkillSlotMap: TMap<FKey, UInputAction*>`: 키별 InputAction
  - `KeyToSkillIndexMap: TMap<FKey, int32>`: 키별 슬롯 인덱스
  - `RuntimeSkillMappingContext`, `RuntimeSkillActionQ/E/R/F`: C++에서 고정 키 매핑을 보장하는 런타임 Enhanced Input 객체

주요 API:

- `BeginPlay()`: 입력 매핑을 모두 지운 뒤 기본 컨텍스트와 Q/E/R/F 런타임 컨텍스트를 추가하고 GameOnly 입력 모드 설정
- `SetupInputComponent()`: Q/E/R/F 런타임 액션을 `Started` 시점에 한 번씩 내부 처리 함수에 바인딩
- `OpenSkillChangeUIAction`: `/Game/SkillMaker/Input/Actions/IA_OpenSkillChangeUI`를 직접 로드하고 `Started`에 `OpenSkillChangeUI()`를 바인딩. 숫자 `0` 키 매핑은 `IMC_Default`가 담당
- `OpenSkillChangeUI()`: 훈련장 HUD를 찾아 현재 Q/E/R/F 매핑 현황 화면을 열며 HUD가 마우스 커서와 GameAndUI 입력 모드를 설정
- `SetSkillInSlot(SlotIndex, SkillID)`, `GetSkillInSlot(SlotIndex)`
- `SetKeyForSkillSlot(NewKey, SlotIndex)`
- `SkillInputSetup()`: Q/E/R/F와 0~3 슬롯 초기화. Q→슬롯 0, E→슬롯 1, R→슬롯 2, F→슬롯 3 순서를 사용
- `UseSkillByKey(PressedKey)`: 슬롯 ID를 찾아 플레이어 캐릭터에 전달
- `TryInteract()`: 전방 200 거리 Visibility 라인 트레이스로 `ASKInteractableActor::OnInteract` 호출

훈련장 HUD가 저장 스킬 선택 후 캐릭터 `SkillMap`과 선택한 슬롯에 ID를 등록한다.

### `ASKSkillMakerController`

- 부모: `APlayerController`
- `BeginPlay()`에서 마우스 커서를 표시하고 `GameAndUI` 입력 모드를 설정한다.

## 스킬 실행과 전투

### `USKSkillComponent`

- 부모: `UActorComponent`, 기본 복제 활성화
- 필드: `SkillMap`, `SkillCooldowns`, `CooldownTimers`

주요 실행 흐름:

```text
ClientRequestUseSkill(SkillID)
├─ Owner가 Authority: SkillMap 조회 → ExecuteSkill
└─ 비권한: ServerUseSkill RPC
   └─ IsSkillAvailable → SkillMap 조회 → ExecuteSkill

ExecuteSkill
├─ MulticastExecuteSkill(SkillData)
│  ├─ ASKBaseCharacter::SetCurrentSkillData
│  └─ AnimInstance::Montage_Play
└─ ApplyCooldown
```

메서드:

- `SetSkillDataInMap(SkillData)`: ID 키로 추가/교체
- `SetSkillMapInComponent(SkillDataMap)`: 전체 맵 교체
- `ClientRequestUseSkill(SkillID)`: 로컬 권한 판정 진입점
- `ServerUseSkill(SkillID)`: 서버 RPC 및 검증
- `MulticastExecuteSkill(SkillData)`: 현재 데이터 설정과 몽타주 재생
- `IsSkillAvailable`, `GetSkillCooldownRemaining`
- 내부 `ExecuteSkill`, `ApplyCooldown`, `ClearCooldown`

권한 보유자가 직접 실행하는 분기는 쿨다운을 검사하지 않는다. `ServerUseSkill_Validate`의 조건도 `SkillMap.Contains || SkillCooldowns.Contains`로 되어 있다.

### `ASKProjectileActor`

- 부모: `AActor`
- 컴포넌트: `CollisionComponent`, `ParticleComponent`, `NiagaraComponent`, `ProjectileComponent`
- 필드: `SkillOwner`, `ImpactData`, `ProjectileSound`
- `InitializeProjectile(InSkillOwner, InImpactData)`: 지연 생성 중 시전자와 발사 시점의 전투 정보 스냅샷을 먼저 저장하고 시전자 충돌을 제외
- `StartProject()`: 생성 완료 후 Particle·Niagara 컴포넌트를 표시·활성화하고 발사 사운드를 재생
- `OnOverlap(...)`: 시전자 외 Actor와 겹치면 `ASKBaseCharacter` 여부를 확인하고 소멸
- `ApplySkillImpact(TargetCharacter)`: 권한이 있는 대상의 `USKCombatComponent`에 스킬 충격 정보를 전달
- `FSKSkillImpactData`는 식별자·타입·타기팅·속성·무기·지속시간·이동 가능 여부·쿨다운·코스트·기본 피해·상태이상·대상 플래그·범위와 공격자를 보관한다. 몽타주·노티파이·BlendSpace·발사체 클래스는 포함하지 않는다.
- `ProjectileSound`가 있으면 발사체 시작 위치에서 한 번 재생한다. 시각 이펙트나 사운드가 없으면 경고 로그를 남긴다.

### `USKCombatComponent`

- 부모: `UActorComponent`, 기본 복제 활성화
- 필드: 복제되는 `MaxHealth`, `CurrentHealth`; `StatusEffectTimers`; `OwnerCharacter`
- `TakeDamage(DamageAmount)`: 서버 권한으로 전달 후 체력 감소·사망 처리
- `ApplySkillImpact(ImpactData)`: 공격자와 스킬 전투 정보를 검증하고 `CalculateFinalDamage` 결과를 체력에 적용
- `CalculateFinalDamage(ImpactData)`: 현재는 음수를 막은 `DamageValue`를 반환하며 이후 속성·공격자·대상 보정의 확장 지점
- `ApplyStatusEffect(EffectType, Duration)`: 서버 RPC 또는 멀티캐스트 실행
- `HandleDeath()`: 소유 캐릭터 파괴
- `GetLifetimeReplicatedProps`: 체력 필드 등록

현재 상태이상 처리의 주요 제약:

- 반복 피해와 해제에 같은 타이머 핸들을 사용한다.
- Burn/Poison은 데이터의 DPS 대신 고정 `5.0f`를 사용한다.
- Slow는 원래 속도 저장 없이 `×0.5`, 해제 시 `×2`를 사용한다.
- Shock/Curse 처리가 없다.
- 발사체 충돌은 이 컴포넌트의 직접 피해 경로에 연결됐지만 상태이상 데이터의 자동 적용은 아직 하지 않는다.

## 애니메이션

### `USKPlayerAnimInstance`

- 부모: `UAnimInstance`
- 상태: `bIsCastingSkill`, `bCanMoveWhileCasting`, `bUseMoveBlendSpace`
- 에셋: `DefaultMoveBlendSpace`, `MoveSkillBlendSpace`, `CurrentBlendSpace`
- `OwnerCharacter`: `NativeInitializeAnimation`에서 `TryGetPawnOwner` 캐스팅
- `SetSkillAnimationState(...)`: 상태와 블렌드 스페이스 설정
- `StopSkillAnimation()`: 몽타주 중지와 상태 초기화
- `NativeUpdateAnimation(DeltaSeconds)`: 상태에 따라 블렌드 스페이스 선택
- `USKSkillComponent`는 이 상태 API를 호출하지 않고 `Montage_Play`만 수행한다.

### `USKSkillAnimNotify_Trigger`

- 부모: `UAnimNotify`
- 필드: `NotifyTriggerName`, `StatusEffects`, `StatusDuration`, `AppliedBuffs`
- 델리게이트: `OnSkillNotifyTriggered(FName)` 선언은 있으나 브로드캐스트·구독이 확인되지 않음
- `Notify(...)`: 캐릭터의 `CurrentSkillData`를 읽고 이름이 `NotifyName`과 같으면 발사체 생성
- `SpawnProjectile(Character, ProjectileClass, SkillData)`: 발사체를 지연 생성하고 전투 정보·공격자 스냅샷을 먼저 초기화한 뒤 생성을 완료해 충돌 이벤트보다 데이터 주입이 앞서도록 보장
- `ApplyAOEEffect(Character, SkillData)`: 광역 상태이상 보조 함수. 활성 `Notify` 경로에서 호출되지 않음

### `USKSkillAnimNotify_HitCheck`

- 부모: `UAnimNotify`
- `Notify(...)`: 캐릭터 캐스팅까지만 수행
- `PerformMeleeAttack(Character, SkillData)`: 스윕과 대상 로그가 있지만 `Notify`에서 호출하지 않으며 피해 적용도 TODO

## 월드 흐름

### `ASKSkillMakerGameMode`

- 부모: `AGameModeBase`
- HUD: `ASKSkillMakerEditorHUD`
- PlayerController: `/Game/SkillMaker/Blueprint/Controller/BP_SKSkillMakerController`
- `BeginPlay()`은 부모만 호출한다.

### `ASKSkillTrainGameMode`

- 부모: `AGameModeBase`
- Pawn: `/Game/SkillMaker/Blueprint/Character/BP_SKPlayerCharacter`
- PlayerController: `/Game/SkillMaker/Blueprint/Controller/BP_SKPlayerController`
- HUD: `ASKSkillMakerTrainHUD`

### `ASKInteractableActor`

- 부모: `AActor`
- 필드: `StaticMeshComponent`, `InteractionTrigger`, `CurrentPlayerCharacter`, `InteractionRange`
- `BeginPlay()`에서 `InteractionTrigger`의 Begin/EndOverlap 델리게이트를 바인딩한다.
- `OnPlayerEnter/Exit`: 현재 플레이어 설정·해제와 플레이어 보조 함수 호출
- 생성자에서 컴포넌트를 생성하지 않으므로 블루프린트 지정이 필요하며 `BeginPlay` 널 검사가 없다.
- `OnInteract()` 기본 구현은 비어 있다.

### `ASKSkillStation`

- 부모: `ASKInteractableActor`
- 필드: `SkillMakerTrainMainWidgetClass`
- `OnInteract()`: 현재 플레이어 컨트롤러의 `ASKSkillMakerTrainHUD`를 찾아 `ShowSkillSelection()` 호출
- 이 생성 경로에서는 훈련 HUD 참조를 위젯에 전달하지 않는다.

## 제작 UI

### `ASKSkillMakerEditorHUD`

- 부모: `AHUD`
- 책임: 제작 메인 위젯 생성, 편집 원본 `CurrentEditingSkill` 관리, 스킬 저장, 프리뷰 캐릭터 실행
- 필드: `MainWidgetClass`, `MainWidget`, `CurrentEditingSkill`, `PreviewCharacter`
- 생성자에서 `/Game/SkillMaker/UI/Editor/WBP_SKSkillMakerEditorMain`을 직접 로드한다.
- `BeginPlay()`에서 메인 위젯을 생성·표시하고 네이티브 `ASKPreviewCharacter`를 월드에 생성한다.

주요 API:

- `InitializeNewSkill()`: 기본 구조체와 이름 `NewSkill`로 초기화
- `LoadSkillForEditing(SkillID)`: SaveGame 서브시스템에서 로드
- `GetCurrentSkillData`, `SetCurrentSkillData`: 전체 편집 데이터를 교체하고 `OnEditingSkillChanged` 발행
- `SetSkillName(SkillName)`, `SetSkillWeaponTag(WeaponTag)`, `SetSkillMontage(SoftMontage)`. 무기를 바꾸면 호환성을 다시 선택해야 하는 몽타주·재생 시간·노티파이는 초기화하고 타입·속성·발사체는 유지한다.
- `SaveCurrentSkill(SkillName)`: 필수 선택값과 저장 기반을 확인하고 ID를 확정한 뒤 HUD의 현재 스킬을 저장
- `OnEditingSkillChanged`: HUD 데이터가 초기화·로드·교체되거나 개별 선택값이 바뀔 때 현재 전체 데이터를 전달
- `PreviewSkillEffect(SkillData)`: 임시 ID를 보완하고 프리뷰 캐릭터의 공통 스킬 실행 경로 사용
- `BeginPlay()`에서 네이티브 `USKMapNavigationWidget`을 생성해 `/Game/SkillMaker/Map/SkillTrainingMap` 이동 버튼을 표시

### `USKSkillMakerEditorMainWidget`

- 부모: `UUserWidget`
- 상태 enum: `ChooseAction`, `ChooseSkill`, `ChooseWeapon`, `ChooseAnimation`, `SkillDetail`, `SaveSkill`
- 책임: 제작 단계 이동, 공통 선택 위젯과 HUD 연결, 저장 요청 전달
- 상태: `CurrentState`, `PreviousStates`, `CurrentEditMode`, `HUDReference`

`BindWidget`:

- `SkillMakerSwitcher`
- `SkillSelectionWidget`, `WeaponSelectionWidget`, `AnimationSelectionWidget`, `SkillDetailWidget`
- `ModifySkillButton`, `CreateSkillButton`, `FinishEditingSkillButton`, `SaveSkillButton`, `BackButton`
- `SkillNameInput`

주요 메서드:

- `Initialize()`: 하위 위젯 델리게이트와 버튼 클릭 바인딩
- `SetHUDReference(InHUD)`: HUD와 상세 위젯 연결
- `SetSkillMakerState(NewState, bFromBackNavigation)`, `GoBackToPreviousState()`
- `OnModifySkillClicked()`: 수정 모드를 설정하고 목록 새로고침 이벤트 후 기존 스킬 선택 화면으로 이동
- `OnCreateSkillClicked()`: 생성 모드를 설정하고 HUD의 새 스킬 데이터와 이름 입력란을 초기화한 뒤 무기 화면으로 이동
- `OnSkillSelected(SkillID)`: HUD에 저장 스킬을 로드하고 기존 이름을 입력란에 설정한 뒤 무기 선택 화면으로 이동
- `OnWeaponSelected(WeaponTag)`: 애니메이션 선택 화면으로 전환. HUD는 같은 선택 델리게이트를 직접 구독
- `OnAnimationSelected(SoftMontage)`: 상세 화면으로 전환. HUD는 같은 선택 델리게이트를 직접 구독
- `OnFinishSkillEditing()`: 현재 생성·수정 모드를 로그에 표시하고 저장 이름 화면으로 이동. 상세 선택값은 선택 시점에 이미 HUD로 전달됨
- `OnSaveSkillClicked()`: 현재 모드와 저장 요청을 로그에 표시하고 이름을 HUD의 `SaveCurrentSkill`에 전달한다. 성공하면 목록을 갱신하고 `PreviousStates`를 `ChooseAction` 하나로 초기화한 뒤 초기 화면으로 이동해 뒤로가기 버튼을 숨기고 모드를 초기화한다.

신규 `SkillID`는 저장 후보에 생성하며 저장 성공 후 HUD 원본에 확정한다.

## 훈련장 UI

### `ASKSkillMakerTrainHUD`

- 부모: `AHUD`
- 필드: `SkillSelectionWidgetClass`, `SkillSelectionWidget`, `SkillSlotAssignmentWidget`, `NavigationWidget`, `CurrentEditingSkillSet`, `CurrentEditingSkill`, `PlayerCharacter`
- 생성자에서 제작 화면도 사용하는 기존 `/Game/SkillMaker/UI/WBP_SKSkillSelection` 클래스를 로드
- `BeginPlay()`에서 실제 소유 Pawn을 `ASKPlayerCharacter`로 저장하고 기존 스킬 선택 위젯, 네이티브 슬롯 위젯과 `SkillMakingMap` 이동 버튼을 생성
- 저장 스킬 카드 선택 시 SaveGame 데이터를 읽어 슬롯 위젯에 표시
- 슬롯 선택 시 전체 스킬 데이터를 `ASKPlayerCharacter::SetSkillDataInMap`으로 등록하고 ID를 `ASKPlayerController::SetSkillInSlot`으로 Q/E/R/F 슬롯에 배치
- 화면 전환 시 이전 위젯을 Viewport에서 제거하고 다음 위젯만 추가한다. 실제 Viewport 크기의 절반을 위치로 사용하고 위젯 정렬점을 중앙으로 설정한다. 생성·델리게이트·Viewport·위치·가시성·포커스 결과를 단계별 로그로 기록
- 편집 데이터 초기화·로드·getter/setter API는 제작 HUD와 유사함

### `USKSkillSlotAssignmentWidget`

- 부모: `UUserWidget`
- 블루프린트 에셋 없이 `WidgetTree`로 선택 스킬 문구, Q/E/R/F 슬롯 버튼과 스킬 선택 버튼을 구성
- `SetSelectedSkill`로 현재 선택을 표시하고 `SetAssignedSkill`로 슬롯별 스킬 이름을 갱신
- `ShowAssignmentOverview()`는 선택 대상 ID를 비우고 슬롯 버튼을 표시 전용으로 잠근 뒤 현재 매핑 안내를 표시
- 슬롯 클릭 시 `OnSkillSlotSelected(SlotIndex)`를 발행
- 스킬 선택 클릭 시 `OnAssignmentCancelled`를 발행해 저장 스킬 목록으로 이동

### `USKMapNavigationWidget`

- 부모: `UUserWidget`
- 블루프린트 에셋 없이 버튼과 문구를 구성
- HUD가 표시 문구와 목적지 맵을 설정하고 클릭 시 `UGameplayStatics::OpenLevel` 호출

### `USKSkillMakerTrainMainWidget`

- 부모: `UUserWidget`
- 상태 enum: `ChooseAction`, `ChooseSkillSet`, `ChooseSkill`, `ChooseAnimation`, `SkillDetail`, `SaveSkill`
- 필드: `HUDReference`, `PlayerCharacter`, `SelectedWeaponTag`, 상태·이력
- `BindWidget`: `SkillMakerSwitcher`, `SkillSelectionWidget`, `AnimationSelectionWidget`, `SkillDetailWidget`, 수정/생성/완료/저장/뒤로 버튼, `SkillNameInput`
- `Initialize()`에서 선택 위젯과 버튼을 바인딩한다.
- `StartSkillMaker(InPlayerCharacter)`는 플레이어 참조만 저장한다.
- 상태 전환과 뒤로 가기는 구현됨
- `OnModifySkillSetClicked`, `OnCreateSkillSetClicked`, `OnCreateSkillClicked`, `OnSkillSelected`, `OnAnimationSelected`, `OnFinishSkillEditing`, `OnSaveSkillClicked`는 비어 있음
- `OnSkillDataFromSaveGame` 델리게이트는 선언되어 있으나 브로드캐스트·구독이 확인되지 않음

## 공통 선택 UI

### 무기 선택

#### `USKWeaponCardWidget`

- 부모: `UUserWidget`
- `BindWidget`: `WeaponButton`, `WeaponThumbnail`, `WeaponNameText`
- 상태: `WeaponTag: FGameplayTag`
- `SetWeaponInfo(InWeaponName, InWeaponTag, SoftThumbnail, bIsSelected)`: 표시와 내부 값 및 선택 색상 설정
- 버튼 클릭 → `OnWeaponCardSelected(WeaponTag)` 브로드캐스트

#### `USKWeaponSelectionWidget`

- 부모: `UUserWidget`
- `BindWidget`: `WeaponGridPanel`
- 클래스 참조: `WBP_SKWeaponCard`
- `NativeConstruct()` → 빈 현재 태그로 `LoadWeaponList(CurrentWeaponTag)`
- `LoadWeaponList(CurrentWeaponTag)`, `CreateWeaponCard(Row, Index, CurrentWeaponTag)`: HUD의 현재 태그와 같은 카드 선택 표시 복원
- 데이터 서브시스템에서 전체 무기 행을 가져와 2열 카드 생성
- 카드의 `OnWeaponCardSelected` 구독 → `OnWeaponSelected` 재전파
- 태그가 없는 무기 행은 카드를 선택 경로에 추가하지 않는다.

### 애니메이션 선택

#### `USKAnimationCardWidget`

- 부모: `UUserWidget`
- `BindWidget`: `AnimationButton`, `AnimationThumbnail`, `AnimationNameText`
- 상태: `SelectedMontage: TSoftObjectPtr<UAnimMontage>`
- `SetAnimationInfo(AnimationName, SoftMontage, SoftThumbnail, bIsSelected)`: 데이터 표시명과 소프트 참조를 보관하고 선택 색상 설정
- 유효하지 않은 몽타주는 카드 생성과 선택 이벤트에서 제외한다.
- 버튼 클릭 → `OnAnimationCardSelected(SelectedMontage)`

#### `USKAnimationSelectionWidget`

- 부모: `UUserWidget`
- `BindWidget`: `AnimationGridPanel`
- 클래스 참조: `WBP_SKAnimationCard`
- `LoadAnimationsForWeapon(WeaponTag, CurrentMontage)`: 태그 계층 호환 목록 조회와 현재 몽타주 선택 표시 복원
- `CreateAnimationCard(Row, Index, CurrentMontage)`: 몽타주를 로드하지 않고 2열 카드 생성, 썸네일은 `SetBrushFromSoftTexture` 사용
- 카드 델리게이트 구독 → `OnAnimationSelected` 재전파

### 발동 시점 선택

#### `USKAnimNotifyCardWidget`

- 부모: `UUserWidget`
- `BindWidget`: `NotifyButton`, `NotifyText`
- 상태: `NotifyName`, `NotifyTime`
- `SetNotifyInfo(InNotifyType, InNotifyName, InNotifyTime, bIsSelected)`: 이름·시간과 선택 색상 표시
- `SetSelected(bIsSelected)`, `GetNotifyName()`: 카드 선택 색상 갱신과 목록의 현재 카드 비교
- 클릭 → `OnNotifySelected(NotifyName)`

#### `USKAnimNotifySelectionWidget`

- 부모: `UUserWidget`
- `BindWidget`: `NotifyListBox`, `SelectedNotifyText`
- 클래스 참조: `WBP_AnimNotifyCard`
- 상태: `AvailableNotifies`, `SelectedNotify`, `SelectedNotifyTime`, `NotifyCards`
- `PopulateNotifyList(Montage, CurrentNotifyName)`: `USKSkillAnimNotify_Trigger`이고 `NotifyTriggerName`이 유효한 이벤트만 카드로 만들고 현재 선택 복원
- 카드 이벤트 구독 → 모든 카드의 이름을 비교해 선택 색상 갱신 → 선택 이름 표시 → `OnAnimNotifySelected(NotifyName)`
- 카드에는 시간이 보이지만 저장과 이벤트에는 이름만 전달한다.
- 같은 이름의 트리거가 여러 개면 실행 시 구분하지 못한다.

### 이펙트·사운드 발사체 선택

#### `USKProjectileCardWidget`

- 부모: `UUserWidget`
- `BindWidget`: `ProjectileSelectButton`, `ProjectileNameText`
- 상태: `ProjectileClass`
- `SetProjectileInfo(ProjectileName, ProjectileClass, bIsSelected)`: 클래스 이름과 선택 색상 설정
- `SetSelected(bIsSelected)`, `GetProjectileClass()`: 카드 선택 색상 갱신과 목록의 현재 카드 비교
- 클래스 이름을 화면에 표시
- 클릭 → `OnProjectileCardSelected(ProjectileClass)`

#### `USKProjectileSelectionWidget`

- 부모: `UUserWidget`
- `BindWidget`: `ProjectileListBox`, `SelectedEffectPreview`, `SelectedEffectText`, 선택적 레거시 `ConfirmButton`
- 클래스 참조: `WBP_ProjectileCard`
- 상태: `CurrentProjectile`, `ProjectileCards`
- `Initialize()`에서 빈 선택으로 `SetProjectileCard(CurrentProjectile)` 호출. 상세 화면 진입 시 HUD의 현재 발사체와 속성으로 다시 구성
- 데이터 서브시스템의 발사체 행 중 선택 속성을 지원하거나 지원 태그가 비어 있는 행으로 카드 생성
- 카드 클릭 즉시 모든 카드의 클래스를 비교해 선택 색상을 갱신하고, 선택 텍스트 갱신 후 `OnProjectileSelected`를 브로드캐스트
- 속성 변경으로 기존 발사체가 목록에서 제외되면 선택을 해제하고 변경 이벤트를 발행한다.
- 레거시 확인 핸들러는 제거됐고 WBP에 남은 선택적 `ConfirmButton`은 화면에서 숨긴다. `SelectedEffectPreview`는 아직 갱신하지 않는다.
- `SetProjectileCard`는 디자인 타임, 월드 없음, 게임 월드가 아닌 에셋 검증 상황에서 즉시 반환한다. 실제 게임 월드에서 GameInstance나 데이터 서브시스템이 없으면 오류 로그를 남긴다.

### 속성 선택

#### `USKElementCardWidget`

- 별도 WBP 없이 카드 전체를 클릭할 수 있는 네이티브 버튼 안에 체크박스와 이름을 구성한다.
- Gameplay Tag, 검은색 왼쪽 정렬 표시명, 설명과 기준 색상을 사용하며 설명은 글자 크기 15의 툴팁으로 제공한다.
- 버튼 클릭 시 체크 상태와 속성색 배경을 함께 갱신하고 `OnElementSelectionChanged(ElementTag, bIsSelected)`를 발행한다.

#### `USKSkillDetailWidget`의 속성 탭

- 기존 WBP의 `StatusEffectTabButton`, `StatusEffectListBox` 이름을 호환 목적으로 유지하면서 탭 문구와 내용을 속성 선택으로 교체한다.
- 한 카드를 선택하면 나머지 카드의 체크를 해제하고 `CurrentEditingSkill.ElementTag`를 즉시 갱신한다.
- 수정 화면 진입 시 기존 속성을 복원하고 발사체 목록을 해당 속성으로 다시 필터링한다.
- `MinRangeSlider`, `MaxRangeSlider`와 문구는 선택적 바인딩으로 찾아 `Collapsed` 처리한다.

### 저장 스킬 선택

#### `USKSkillCardWidget`

- 부모: `UUserWidget`
- `BindWidget`: `SkillButton`, `SkillThumbnail`, `SkillNameText`
- 상태: `StoredSkillID`, `StoredSkillName`
- 클릭 → `OnSkillCardSelected(StoredSkillID)`

#### `USKSkillSelectionWidget`

- 부모: `UUserWidget`
- `BindWidget`: `SkillGridPanel`
- 클래스 참조: `WBP_SKSkillCard`
- 참조: `EditorMainWidget`, `TrainMainWidget`
- `NativeConstruct()`에서 SaveGame 목록 로드
- 제작 메인 위젯 설정 시 `OnSkillDataFromTable`을 구독해 목록 갱신
- 훈련 메인 위젯 설정은 참조만 저장
- 카드 이벤트 구독 → `OnSkillSelected(SkillID)` 재전파
- `OnSkillSelected`의 UPROPERTY 지정자가 `BlueprintCallable`로 작성되어 있다. 현재 에디터 타깃 빌드와 C++ `AddDynamic`/`AddUniqueDynamic` 구독은 동작하지만, 블루프린트에서 이벤트를 구독시키려는 공개 계약이라면 `BlueprintAssignable`로 바꿀지 결정해야 한다.

### 세부사항

#### `USKSkillDetailWidget`

- 부모: `UUserWidget`
- `ESKHUDClass` enum은 `SkillMakerEditor`, `SkillMakerTrain` 값을 선언하지만 현재 필드와 분기에서 사용하지 않는다.
- 별도 스킬 데이터는 소유하지 않는다. 표시와 변경은 제작 HUD의 `CurrentEditingSkill`을 기준으로 한다.
- HUD 참조: 제작 HUD와 훈련 HUD를 모두 가지지만 데이터 조회와 변경 델리게이트 연결은 제작 HUD에 의존
- `BindWidget`:
  - 탭: `TabSwitcher`, `GeneralTabPanel`, `StatusEffectTabPanel`, `ProjectileTabPanel`, `AnimNotifyPanel`, 네 탭 버튼
  - 일반: `GeneralTabScrollBox`, `SkillTypeComboBox`, 선택적 `DamageTextBox`. 데미지는 입력 변경 즉시 HUD에 반영하고 기존 저장값을 복원하며, `MinRangeSlider`, `MaxRangeSlider`와 문구는 선택 바인딩 후 숨김
  - 목록: 속성 목록으로 사용하는 `StatusEffectListBox`, `ProjectileSelectionWidget`, `AnimNotifySelectionWidget`
  - 동작: `PreviewSkillButton`
- 기존 WBP 호환을 위해 `StatusEffect` 이름을 가진 탭·목록을 유지하지만 런타임 문구와 내용은 속성 선택으로 사용한다.

주요 메서드:

- `NativeConstruct()`: 블루프린트에 구성된 기본 탭 스크롤을 설정하고, 탭 버튼·데미지 입력·하위 선택 델리게이트 바인딩, 속성 문구 설정 및 사거리 UI 숨김 처리
- `ConfigureGeneralTabScrolling()`: WBP의 `GeneralTabPanel > GeneralTabScrollBox > GeneralTabContent` 계층을 검사하고 콘텐츠에 상단 24, 우측 12, 하단 20의 여백과 항상 표시되는 스크롤바를 적용한다. 런타임 위젯 재배치는 하지 않는다.
- 탭 전환은 자식 인덱스가 아니라 `GeneralTabPanel`, `StatusEffectTabPanel`, `ProjectileTabPanel`, `AnimNotifyPanel` 참조를 사용하며 현재 탭 버튼을 파란색으로 표시
- `SetSkillMakerEditorHUD`, `SetSkillMakerTrainHUD`
- `InitializeFromSkillData()`: 제작 HUD의 현재 데이터로 일반값·속성·발사체 선택 표시 복원
- `PopularSkillDetails()`: 일반 UI 값 반영
- `PopulateElementList()`: 화염·물·바람·대지 네이티브 카드를 생성하고 기존 단일 선택 복원
- `PopulateAnimNotifyList()`: 현재 몽타주에서 유효 트리거 조회
- `OnElementSelectionChanged`: 한 속성만 선택되도록 카드 상태와 `ElementTag`를 갱신하고 발사체 목록 재구성
- `OnProjectileSelected`, `OnNotifySelected`, `OnSkillTypeChanged`: HUD 최신값의 지역 사본에서 선택 필드만 변경하고 `OnSkillDetailChanged` 발행
- `OnPreviewSkillClicked()`: 선택 시점에 동기화된 편집 데이터로 프리뷰

`DamageValue`는 기본 탭의 선택적 `DamageTextBox::OnTextChanged`에서 0 이상의 유한한 숫자를 받아 HUD 최신값에 즉시 반영하고 기존 저장값을 복원한다. 범위 필드는 저장 호환성을 위해 유지하지만 상세 UI 입력 경로에서는 사용하지 않는다.

#### `USKStatusEffectCardWidget`

- 부모: `UUserWidget`
- `BindWidget`: `EffectNameText`, `EffectCheckBox`, `DurationTextBox`, `DOTSlider`; `StackCountTextBox`는 선택 바인딩
- 상태: `CurrentEffectType`, `CurrentDuration`, `CurrentDOT`, `CurrentStackCount`, `bCurrentCanStack`
- `InitializeEffectEntry`, `SetStatusEffectData`, `GetCurrentStatusEffectData`
- 기존 입력 콜백에서 `OnStatusEffectChanged`를 발행한다. 현재 상세 위젯은 이 카드를 생성하거나 델리게이트를 구독하지 않는다.

## 델리게이트 연결표

| 델리게이트 | 선언/발행 | 구독자 | 전달값 | 상태 |
|---|---|---|---|---|
| `OnWeaponCardSelected` | `USKWeaponCardWidget` | `USKWeaponSelectionWidget::WeaponSelected` | `FGameplayTag` | 활성 |
| `OnWeaponSelected` | `USKWeaponSelectionWidget` | 제작 HUD와 제작 메인 위젯 | `FGameplayTag` | HUD 데이터 갱신과 화면 전환에 각각 사용 |
| `OnAnimationCardSelected` | `USKAnimationCardWidget` | `USKAnimationSelectionWidget::AnimationSelected` | `TSoftObjectPtr<UAnimMontage>` | 활성 |
| `OnAnimationSelected` | `USKAnimationSelectionWidget` | 제작 HUD와 제작/훈련 메인 위젯 | `const TSoftObjectPtr<UAnimMontage>&` | 제작 HUD 데이터 갱신과 화면 전환에 각각 사용, 훈련 핸들러 비어 있음 |
| `OnNotifySelected` | `USKAnimNotifyCardWidget` | `USKAnimNotifySelectionWidget::OnNotifyButtonSelected` | `FName` | 활성 |
| `OnAnimNotifySelected` | `USKAnimNotifySelectionWidget` | `USKSkillDetailWidget::OnNotifySelected` | `FName` | 활성 |
| `OnProjectileCardSelected` | `USKProjectileCardWidget` | `USKProjectileSelectionWidget::SelectedProjectile` | `TSoftClassPtr<ASKProjectileActor>` | 활성 |
| `OnProjectileSelected` | `USKProjectileSelectionWidget` | `USKSkillDetailWidget::OnProjectileSelected` | `TSoftClassPtr<ASKProjectileActor>` | 활성, 카드 클릭 즉시 발행 |
| `OnElementSelectionChanged` | `USKElementCardWidget` | `USKSkillDetailWidget::OnElementSelectionChanged` | `FGameplayTag`, 선택 여부 | 활성, 단일 선택과 발사체 필터 갱신 |
| `OnStatusEffectChanged` | `USKStatusEffectCardWidget` | 현재 구독자 없음 | `FStatusEffectData`, 활성 여부 | 레거시 보존 |
| `OnSkillDetailChanged` | `USKSkillDetailWidget` | `ASKSkillMakerEditorHUD::SetCurrentSkillData` | `const FSKSkillData&` | 활성, 선택 즉시 발행 |
| `OnEditingSkillChanged` | 제작 HUD | 블루프린트/요약 UI 구독 가능 | `const FSKSkillData&` | 활성, 현재 C++ 구독자는 없음 |
| `OnSkillCardSelected` | `USKSkillCardWidget` | `USKSkillSelectionWidget::SelectSkill` | `const FName& SkillID` | 활성 |
| `OnSkillSelected` | `USKSkillSelectionWidget` | 제작 메인 위젯, 훈련장 HUD, 레거시 훈련 메인 위젯 | `const FName& SkillID` | 제작과 현재 훈련 HUD 경로 활성, 레거시 훈련 메인 핸들러는 비어 있음 |
| `OnSkillDataFromTable` | 제작 메인 위젯 | `USKSkillSelectionWidget::LoadSkillList` | 없음 | 기존 수정 진입·저장 후 발행 |
| `OnSkillDataFromSaveGame` | 훈련 메인 위젯 | 확인되지 않음 | 없음 | 미연결 |
| `OnSkillNotifyTriggered` | 트리거 노티파이 | 확인되지 않음 | `FName` | 선언만 존재 |

엔진 델리게이트:

- 각 카드와 메인/상세 위젯 버튼은 `UButton::OnClicked`를 `AddDynamic`으로 연결한다.
- 스킬 이름 등 확정형 텍스트 입력은 `OnTextCommitted`, 데미지 입력은 즉시 HUD에 보존하기 위해 `OnTextChanged`, 슬라이더는 `OnValueChanged`를 사용한다. 레거시 상태이상 카드는 `OnCheckStateChanged`를 사용하고 네이티브 속성 카드는 전체 행 버튼의 `OnClicked`로 단일 선택을 처리한다.
- `ASKInteractableActor`는 `OnComponentBeginOverlap`, `OnComponentEndOverlap`을 사용한다.

## 주요 데이터 흐름

### 리소스 선택

```text
DT_WeaponData
→ USKDataManagerSubsystem::GetWeaponList
→ USKWeaponSelectionWidget
→ USKWeaponCardWidget
→ OnWeaponSelected(WeaponTag)
→ USKSkillMakerEditorMainWidget
→ ASKSkillMakerEditorHUD::CurrentEditingSkill.WeaponTag

WeaponTag
→ USKDataManagerSubsystem::GetAnimationsForWeapon
→ DT_AnimationData CompatibleWeaponTags 계층 호환 필터
→ USKAnimationSelectionWidget
→ USKAnimationCardWidget
→ OnAnimationSelected(SoftMontage)
→ ASKSkillMakerEditorHUD::CurrentEditingSkill.SkillMontage
```

### 상세 편집·프리뷰

```text
ASKSkillMakerEditorHUD::CurrentEditingSkill
→ USKSkillDetailWidget이 최신값 조회
→ 지역 사본에서 유형/데미지/속성/발사체/노티파이 중 선택 필드 수정
→ OnSkillDetailChanged
→ HUD CurrentEditingSkill 즉시 교체
→ OnEditingSkillChanged
→ ASKSkillMakerEditorHUD::PreviewSkillEffect
→ ASKPreviewCharacter::SetSkillDataInMap
→ ASKPreviewCharacter::UseSkill
→ USKSkillComponent 공통 실행
```

### 저장·재조회

```text
USKSkillMakerEditorMainWidget::OnSaveSkillClicked
→ ASKSkillMakerEditorHUD::SaveCurrentSkill
→ 필수값·GameInstance·SaveGameSubsystem 검증
→ ID가 없으면 저장 후보에 GUID 생성
→ USKSaveGameSubsystem::SaveSkillData
→ USKPlayerSkillSave::CurrentSkillSet.Skills
→ UGameplayStatics::SaveGameToSlot

USKSkillSelectionWidget::LoadSkillList
→ USKSaveGameSubsystem::GetSavedSkillList
→ 스킬 카드 생성
→ OnSkillSelected(SkillID)
→ HUD LoadSkillForEditing
→ ChooseWeapon에서 기존 WeaponTag 선택 표시
→ ChooseAnimation에서 기존 SkillMontage 선택 표시
→ SkillDetail에서 기존 SkillType·DamageValue·ElementTag·ProjectileActor·NotifyName 선택 표시
```

### 스킬 실행

```text
입력 Q/E/R/F
→ ASKPlayerController::UseSkillByKey
→ ASKPlayerCharacter::UseSkill(SkillID)
→ USKSkillComponent::ClientRequestUseSkill
→ RPC/멀티캐스트
→ CurrentSkillData 설정
→ 소프트 몽타주 로드 후 재생
→ USKSkillAnimNotify_Trigger
→ 이름 일치
→ 소프트 발사체 클래스 로드 후 ASKProjectileActor 지연 생성
→ FSKSkillImpactData에 전투 정보와 SourceCharacter 스냅샷 저장
→ 발사체 충돌 시 대상 ASKBaseCharacter의 USKCombatComponent 조회
→ ApplySkillImpact → CalculateFinalDamage → TakeDamage
```

훈련장 HUD는 저장 스킬 선택 후 슬롯 위젯을 표시하고, Q/E/R/F 중 선택한 위치에 전체 데이터를 캐릭터 `SkillMap`으로 등록한 뒤 SkillID를 컨트롤러 슬롯에 넣는다. 할당이 끝나면 UI 입력을 닫고 GameOnly 입력으로 돌아가 위 실행 경로를 사용할 수 있다.

## 블루프린트·에셋 경계

C++에서 직접 확인한 클래스 경로:

- 제작 메인 위젯: `/Game/SkillMaker/UI/Editor/WBP_SKSkillMakerEditorMain`
- 제작 컨트롤러: `/Game/SkillMaker/Blueprint/Controller/BP_SKSkillMakerController`
- 훈련 플레이어: `/Game/SkillMaker/Blueprint/Character/BP_SKPlayerCharacter`
- 훈련 컨트롤러: `/Game/SkillMaker/Blueprint/Controller/BP_SKPlayerController`

블루프린트에서 지정해야 하는 주요 참조:

- 모든 `BindWidget` 이름과 위젯 타입
- 카드 클래스 `WBP_SKWeaponCard`, `WBP_SKAnimationCard`, `WBP_SKSkillCard`, `WBP_ProjectileCard`, `WBP_AnimNotifyCard`, `WBP_SKStatusEffectCard`
- `IMC_Default`의 숫자 0 → `IA_OpenSkillChangeUI` 매핑
- `ASKInteractableActor`의 컴포넌트 참조
- 데이터 테이블 행의 몽타주·썸네일·발사체 클래스
- 몽타주의 `USKSkillAnimNotify_Trigger`와 `NotifyTriggerName`
- `WBP_SKSkillDetail`의 직접 탭 자식 `GeneralTabPanel`, `StatusEffectTabPanel`, `ProjectileTabPanel`, `AnimNotifyPanel`과 기본 탭 내부의 `GeneralTabScrollBox`

`Content/*`는 저장소 용량 정책에 따라 Git에서 추적하지 않는다. 이 절은 로컬 에셋과 C++ 사이의 계약을 기록하며 에셋 파일을 커밋 대상으로 지정하지 않는다.

## 현재 리소스 참조 기준선

- 무기 호환성은 `Weapon` 계층의 Gameplay Tag로 표현한다.
- 무기 행은 단일 `WeaponTag`, 애니메이션 행은 복수 `CompatibleWeaponTags`를 가진다.
- 선택 무기 태그가 애니메이션 호환 태그와 같거나 하위일 때 목록에 표시한다.
- HUD의 `CurrentEditingSkill`이 제작 중인 스킬의 유일한 영구 편집 데이터다. 선택 UI는 화면 진입 시 이 값을 읽어 무기·애니메이션·타입·속성·발사체·노티파이 선택 표시를 복원하며, 속성·발사체·노티파이는 클릭 시 카드 상태를 즉시 다시 계산한다.
- 데이터 행과 카드에서 몽타주·썸네일·발사체 클래스를 소프트 참조로 유지한다. 선택 델리게이트와 스킬 저장에는 몽타주와 발사체 소프트 참조를 전달한다.
- 일반 설정 함수는 소프트 참조 입력을 `const&`로 받는다. 몽타주와 구조체 동적 델리게이트도 `const&`를 사용하고 발사체 클래스 선택 이벤트는 값으로 전달한다.
- 목록 생성은 몽타주와 발사체 클래스를 동기 로드하지 않는다. 썸네일은 `UImage::SetBrushFromSoftTexture`를 사용한다.
- 몽타주는 상세 화면에서 노티파이를 읽을 때와 스킬 실행 시, 발사체 클래스는 노티파이 실행 시에만 동기 로드한다.
- `DT_WeaponData`, `DT_AnimationData`, `DT_ProjectileData`의 레거시 필드와 `Test1` SaveGame은 새 필드로 자동 변환되지 않는다. 사용자가 확인한 제작 흐름에 필요한 현재 값은 동작하지만 아직 사용하지 않은 행과 이전 저장 데이터는 별도로 점검한다.
- 무기 및 애니메이션 호환 태그는 빈 값으로 두지 않는다. 애니메이션의 상위 태그는 그 하위 무기 태그를 포괄한다.
- 스킬 속성은 단일 `ElementTag`, 이펙트·사운드 호환 범위는 `SupportedElementTags`로 표현한다. 속성 카드 기준 색상과 설명은 구현했으며 실제 이펙트 색상 변경은 아직 연결하지 않았다.
- 데이터 양이 늘어날 경우 스킬 로드 단계의 선행 로드·캐시와 목록의 페이징·가상화를 검토한다. 현재 실행 시 동기 로드 기준과 혼동하지 않는다.
- 패키징에서는 문자열 경로로 찾는 데이터 테이블과 그 안의 소프트 참조 리소스가 쿠킹 대상에 포함되는지 검증한다.

## 문서 갱신 규칙

- 클래스의 부모, 책임, 외부 API, 주요 소유 필드 또는 델리게이트가 바뀌면 해당 절을 함께 갱신한다.
- 데이터 필드의 의미나 저장 위치가 바뀌면 핵심 데이터 타입과 데이터 흐름을 함께 갱신한다.
- 선언만 추가된 기능은 `활성`으로 기록하지 않고 실제 호출자와 실행 경로를 확인한다.
- 블루프린트와 에셋을 확인하지 않았다면 C++ 선언만으로 연결 완료라고 쓰지 않는다.
- 결함의 상세 재현과 우선순위는 이 문서에 중복하지 않고 `IMPLEMENTATION_STATUS.md`에서 관리한다.

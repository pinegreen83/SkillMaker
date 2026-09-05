# SkillMaker 개발 규칙

이 문서는 현재 SkillMaker 프로젝트의 기존 코드 스타일을 우선 기준으로 삼고, 비어 있는 규칙은 Unreal Engine 5.8 공식 C++ 코딩 표준과 에셋 명명 규칙를 따라 보완한다.

이 문서는 코드 작성·수정 시 준수할 규칙입니다. 모든 기존 코드가 이미 이 규칙을 충족한다는 뜻은 아닙니다. 2026-09-05 정적 검토에서 확인한 미준수와 연결 누락은 [구현 현황](IMPLEMENTATION_STATUS.md), 실제 책임과 호출 관계는 [프로젝트 구조](PROJECT_MAP.md)에 기록합니다. 기능 범위와 제품 기획은 여기서 확정하지 않습니다.

참고 문서:
- Epic의 Unreal Engine 5.8 C++ 코딩 표준: https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine
- 권장 에셋 명명 규칙: https://dev.epicgames.com/documentation/unreal-engine/recommended-asset-naming-conventions-in-unreal-engine-projects

## 문서 작성 언어

- 프로젝트 문서의 기본 언어는 한국어다. 제목·본문·표·링크 설명은 한국어로 작성한다.
- 코드·명령어·식별자·파일명·경로·설정 키·API 및 제품 고유 명칭은 원문을 유지한다.
- 코드 예시와 실행 명령은 번역하지 않는다. 기존 문서의 설명을 갱신할 때도 한국어를 사용한다.
- 언어를 정리할 때 구현 사실, 검증 범위, 기존 개발 규칙의 의미를 바꾸지 않는다.

## 기본 원칙

- 기존 프로젝트 구조와 이름을 우선 보존한다.
- 코드 수정은 요청된 기능/버그 범위에 한정한다.
- `Content/**/*.uasset`, `*.umap`, `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`는 명시 요청 없이는 수정하지 않는다.
- C++ 반영 타입을 변경한 뒤에는 에디터 타깃 빌드와 관련 블루프린트 바인딩을 확인한다.
- UE 5.8 기준으로 새 코드를 작성하되, 기존 저장 데이터나 데이터 테이블 행 키를 깨는 변경은 별도 마이그레이션 계획 없이 하지 않는다.

## 프로젝트 구조

- `Source/SkillMaker/Character/`: 캐릭터 계층. 공통 기능은 `ASKBaseCharacter`에 둔다.
- `Source/SkillMaker/Player/`: PlayerController 계층. 입력 바인딩과 플레이어 요청 진입점을 둔다.
- `Source/SkillMaker/Skill/`: 스킬 데이터, 스킬 실행 컴포넌트, 발사체, 레거시 스킬 매니저.
- `Source/SkillMaker/Combat/`: 체력, 데미지, 상태이상, 사망 처리.
- `Source/SkillMaker/Game/`: `UGameInstance`, `UGameInstanceSubsystem` 기반 전역 시스템.
- `Source/SkillMaker/Save/`: `USaveGame` 객체와 저장 데이터 구조.
- `Source/SkillMaker/GameMode/`: 맵/플로우별 GameMode.
- `Source/SkillMaker/UI/UI-SkillMaker/`: UMG C++ 베이스 위젯.
- `Source/SkillMaker/Animation/`: AnimInstance와 애님 노티파이.
- `Source/SkillMaker/Prop/`: 월드 상호작용 Actor.
- `Source/SkillMaker/Data/`: 데이터 테이블 행 구조체와 레거시 데이터 매니저 자리표시 객체.
- `Source/SkillMaker/Logging/`: `LogSkillMaker`, `SK_LOG`.

새 파일은 가장 가까운 책임의 폴더에 둔다. 여러 폴더를 동시에 수정해야 할 때는 책임 방향을 유지한다.

권장 의존 방향:

```text
UI -> Game/Subsystem -> Data/Save
PlayerController -> Character -> Components
SkillComponent -> Animation/Projectile/Combat
Prop -> UI 또는 Character의 상호작용 진입점
```

순환 의존이 생기면 전방 선언, 델리게이트, 서브시스템 API 중 기존 패턴에 가장 가까운 방식을 사용한다.

## C++ 이름 규칙

프로젝트 고유 접두사는 `SK`를 사용한다.

- Actor 계열: `ASK...`
  - 예: `ASKBaseCharacter`, `ASKProjectileActor`, `ASKSkillStation`
- UObject·컴포넌트·서브시스템·위젯 계열: `USK...`
  - 예: `USKSkillComponent`, `USKDataManagerSubsystem`, `USKSkillDetailWidget`
- 구조체 계열: `FSK...`
  - 예: `FSKSkillData`, `FSKWeaponRow`
- 열거형 계열: `ESK...` 또는 도메인이 이미 명확한 경우 `E...`
  - 예: `ESKSkillMakerState`, `EStatusEffect`
- 불리언 변수: `b...`
  - 예: `bCanMoveWhileChanneling`, `bAffectEnemies`
- 템플릿 타입: Unreal 관례대로 `T...`
  - 예: `TArray`, `TMap`, `TSubclassOf`, `TObjectPtr`

일반 이름은 PascalCase를 사용한다. Unreal 공식 기준과 같이 타입/변수/함수명은 명확하고 축약을 피한다.

- 변수/필드: 명사형
  - `CurrentEditingSkill`, `SkillCooldowns`
- 동작 함수: 동사형
  - `LoadSkillForEditing`, `SetSkillMontage`, `ApplyCooldown`
- bool 반환 함수: 질문형
  - `IsSkillAvailable`, `Should...`, `Can...`
- 입력 파라미터는 필요하면 `In` 접두사를 사용한다.
  - `InSkillID`, `InSaveData`
- 출력 파라미터는 `Out` 접두사를 사용한다.
  - `OutSkillData`

## 파일 이름

현재 프로젝트는 클래스명과 파일명을 맞추는 방식을 사용한다. 이 프로젝트에서는 이 규칙을 유지한다.

- `ASKBaseCharacter` -> `SKBaseCharacter.h/.cpp`
- `USKSkillComponent` -> `SKSkillComponent.h/.cpp`
- `FSKSkillData` -> `SKSkillData.h`

Unreal 공식 문서는 일반적으로 파일명에 타입 접두사를 붙이지 않는 방식을 권장하지만, 이 프로젝트는 이미 `SK` 접두사 기반으로 정리되어 있으므로 기존 일관성을 우선한다.

## 헤더 작성 규칙

- 모든 헤더는 `#pragma once`를 사용한다.
- `.generated.h`는 항상 해당 헤더에서 마지막으로 포함한다.
- 헤더에서는 가능한 전방 선언을 사용한다.
- `UCLASS`, `USTRUCT`, `UENUM`, `UFUNCTION`, `UPROPERTY`가 필요한 타입은 UnrealHeaderTool이 처리할 수 있는 형태로 선언한다.
- 블루프린트에 노출되는 함수/프로퍼티에는 블루프린트가 처리 가능한 타입만 사용한다.
- `TOptional`, 복잡한 템플릿 기반 타입, 내부 전용 타입은 블루프린트 노출 API에 직접 쓰지 않는다.
- 데이터 테이블 행 구조체는 `FTableRowBase`를 상속한다.

권장 순서:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "BaseOrRequiredHeader.h"
#include "ThisClass.generated.h"

class UWidgetSwitcher;

UCLASS()
class SKILLMAKER_API USKExampleWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> ExampleSwitcher;
};
```

## 클래스 구성

Unreal 공식 기준처럼 독자가 공개 API를 먼저 볼 수 있도록 구성한다.

권장 순서:

1. 생성자
2. Unreal 생명주기 함수 재정의
3. 공개 게임플레이·UI API
4. 보호된 보조 함수·API
5. 리플렉션 프로퍼티
6. 비공개 보조 함수
7. 비공개 상태

현재 프로젝트의 기존 스타일상 `public`, `protected`, `private` 섹션을 명시적으로 나누되, 새 코드에서는 같은 접근 지정자를 불필요하게 반복하지 않는다.

## UObject 참조와 메모리

- UObject 참조 필드는 `TObjectPtr<>`를 우선 사용한다.
- UObject가 아닌 값 타입은 값 또는 참조로 보관한다.
- `UPROPERTY`가 필요한 UObject 참조는 반드시 반영 필드로 둔다.
- 런타임 생성 UObject를 정적 포인터로만 보관하지 않는다. Outer와 GC 수명을 명확히 한다.
- 전역 데이터 접근은 가능하면 `UGameInstanceSubsystem`으로 모은다.

## UMG 위젯 규칙

- C++ 위젯의 `UPROPERTY(meta = (BindWidget))` 이름은 위젯 블루프린트의 위젯 이름과 정확히 일치해야 한다.
- 버튼 이벤트 바인딩은 `Initialize()` 또는 `NativeConstruct()` 중 하나로 일관되게 관리한다.
- 같은 위젯에서 `Initialize()`와 `NativeConstruct()`를 모두 사용할 때는 중복 바인딩이 생기지 않도록 한다.
- 델리게이트 콜백 함수는 `UFUNCTION()`으로 선언한다.
- WidgetSwitcher 상태 열거형의 순서는 블루프린트의 WidgetSwitcher 자식 순서와 맞아야 한다.
- UI에서 수정 중인 데이터와 최종 저장 데이터의 소유자를 분리한다.
  - 편집 중 임시값: 상세 위젯 또는 HUD
  - 최종 저장: `USKSaveGameSubsystem`
  - 리소스 목록 조회: `USKDataManagerSubsystem`

## 데이터 접근과 저장 책임

리소스 목록 조회와 제작 스킬 저장은 기존 서브시스템 책임에 맞춰 분리한다.

- 리소스 데이터는 Unreal Engine 데이터 테이블로 관리한다.
  - 현재 활성 목록은 무기·애니메이션·발사체이며 `USKDataManagerSubsystem`으로 조회한다.
  - `FSKWeaponData`, `FSKAnimationData`, `FSKProjectileData`는 활성 행 구조체로 유지한다. 레거시 객체인 `USKDataManager`와 혼동하지 않는다.
  - 기존 목록의 리소스 추가는 가능한 한 데이터 테이블 행와 에셋 참조 설정으로 처리한다.
- 제작 스킬은 `USKSaveGameSubsystem`과 `USKPlayerSkillSave`를 통해 저장·조회한다.
  - 신규 UI 저장 코드는 데이터 테이블 에셋 수정에 의존하지 않는다.
  - `DT_SkillData`는 현재 UI 저장소가 아니며, 자동 초기 데이터 주입·이관 기능이 있는 것으로 가정하지 않는다.
- SkillMaker UI는 선택 결과와 수치 값을 `FSKSkillData`에 구성한다. 상세 위젯의 편집 복사본과 HUD의 데이터를 구분하고, 저장 시 전달 경로를 확인한다.
- 스킬 실행 시스템은 저장 위치를 직접 알지 않고, 로드된 스킬 데이터만 받아 실행한다.

## 데이터 테이블 규칙

- 행 구조체는 `FTableRowBase`를 상속하고 `BlueprintType`을 붙인다.
- 행 키와 구조체 내부 ID의 의미를 명확히 분리한다.
  - 행 키: 데이터 테이블 행 이름
  - `SkillID`: 스킬의 안정적인 식별자
  - `SkillName`: 사용자 표시명
- `FName` ID 비교 시 `SkillID`와 `SkillName`을 혼용하지 않는다.
- 런타임 플레이어 저장 데이터와 사용자가 제작한 스킬 데이터는 데이터 테이블에 직접 저장하지 않는다.
- 리소스 데이터 테이블은 읽기 중심으로 사용하며, 제작 스킬의 영구 저장은 SaveGame API를 사용한다.
- 데이터 테이블 행에는 UI 표시명과 실제 에셋 참조를 함께 둘 수 있지만, 게임플레이 실행에 필요한 값은 저장된 스킬 데이터에 복사되거나 안정적인 ID로 참조되어야 한다.

## 서브시스템 규칙

- `UGameInstanceSubsystem`은 `GetGameInstance()->GetSubsystem<USK...>()`로 가져온다.
- `Cast<USKSubsystem>(GetGameInstance())` 패턴은 사용하지 않는다.
- 데이터 테이블 목록 제공, SaveGame 접근, 전역 상태 관리는 서브시스템 API로 노출한다.
- UI는 데이터 테이블 에셋을 직접 소유하기보다 서브시스템의 조회 API를 사용한다.
- 새 UI 저장/조회 코드는 레거시 `USKSkillManager` 또는 `USKDataManager`에 의존하지 않는다.

## 스킬 시스템 규칙

- 스킬 정의 데이터는 `FSKSkillData`를 기준으로 한다.
- 스킬 실행 진입점은 캐릭터의 `UseSkill(SkillID)`로 유지한다.
- 캐릭터는 실제 실행을 `USKSkillComponent`에 위임한다.
- 네트워크 API의 책임은 다음과 같이 유지한다. 권한 보유자의 직접 실행 분기와 쿨다운 검사 누락은 구현 현황 NET-01을 참고한다.
  - 클라이언트 요청 진입점: `ClientRequestUseSkill`
  - 서버 실행: `ServerUseSkill`
  - 멀티캐스트 효과: `MulticastExecuteSkill`
- 애님 노티파이가 스킬 효과를 실행해야 하는 경우, 몽타주 재생 전에 캐릭터의 `CurrentSkillData`가 유효해야 한다.
- 에디터 프리뷰 캐릭터도 `ASKBaseCharacter` 계층이므로, 스킬 실행 코드는 플레이어 캐릭터 전용 캐스팅에 의존하지 않는다.
- 미저장 프리뷰에는 임시 `PreviewSkill` ID를 사용할 수 있다. 현재 저장 UI는 ID가 없으면 GUID를 생성하고 기존 ID가 있으면 유지한다. 사용자 입력인 `SkillName`과 ID를 혼용하지 않는다. 편집 상태의 ID 동기화 문제는 구현 현황 UI-02를 참고한다.
- 쿨다운은 서버 권한 기준으로 적용한다.
- 스킬 슬롯 키는 컨트롤러가 관리하고, 스킬 데이터 맵은 캐릭터/스킬 컴포넌트가 관리한다.

## 전투 규칙

아래는 준수해야 할 규칙이며 현재 구현의 완료 설명이 아니다. 발사체 연결 누락과 타이머·이동 복구·멀티캐스트 처리의 차이는 구현 현황 COMBAT-01, COMBAT-02에 기록한다.

- 데미지와 상태이상 적용은 `USKCombatComponent`를 통해 처리한다.
- 상태이상 타이머는 반복 데미지 타이머와 해제 타이머를 분리한다.
- 이동 속도 변경은 원본 값을 보존한 뒤 복구한다.
- 서버 권한이 필요한 상태 변경은 서버에서 처리하고, 표현 동기화는 멀티캐스트 또는 복제로 분리한다.
- 사망 처리와 액터 소멸은 서버 권한에서 수행한다.

## SaveGame 규칙

- 플레이어 스킬셋 저장은 `USKSaveGameSubsystem`과 `USKPlayerSkillSave`를 통해 처리한다.
- 사용자가 SkillMaker에서 제작한 스킬 데이터는 SaveGame 저장 대상으로 본다.
- 저장 전에 SaveGame 객체가 없으면 생성 경로가 있어야 한다.
- 슬롯 이름과 사용자 인덱스는 호출마다 하드코딩하지 말고 한 곳에서 관리한다.
- 현재 기본 저장 슬롯은 서브시스템에서 관리하며, 새 저장 데이터가 없으면 `USKPlayerSkillSave`를 생성해 슬롯에 기록한다.
- `FSKSkillSet::Skills`는 `SkillID -> FSKSkillData` 맵으로 관리한다.
- `CurrentSkillSet`, `PlayerSkills`, `SkillID`의 키 의미를 변경할 때는 기존 저장 데이터 호환성을 검토한다.
- SaveGame에 직렬화되어야 하는 필드는 `UPROPERTY(SaveGame)` 대상으로 유지한다.
- SaveGame에는 런타임에 필요한 스킬 수치와 선택된 리소스 참조 또는 안정적인 리소스 ID를 저장한다.
- SaveGame 데이터 구조를 바꿀 때는 버전 필드 또는 마이그레이션 경로를 함께 고려한다.

## 에셋 명명 규칙

Epic의 권장 에셋 명명은 `[AssetTypePrefix]_[AssetName]_[Descriptor]_[OptionalVariant]` 형식이다. SkillMaker에서는 기존 `SK` 도메인명을 에셋 이름에 포함하는 현재 방식을 유지한다.

권장 접두사:

- 블루프린트: `BP_`
  - `BP_SKPlayerCharacter`
- 위젯 블루프린트: `WBP_`
  - `WBP_SKSkillMakerEditorMain`
- 데이터 테이블: `DT_`
  - `DT_SkillData`, `DT_WeaponData`
- 애니메이션 블루프린트: `ABP_`
  - `ABP_Manny`
- 애니메이션 몽타주: `AM_`
- 애니메이션 시퀀스: `AS_`
- 블렌드 스페이스: `BS_`
- 스태틱 메시: `SM_`
- 스켈레탈 메시: `SK_`
- 텍스처: `T_`
- 머티리얼: `M_`
- 머티리얼 인스턴스: `MI_`
- Niagara 시스템: `FXS_`
- Niagara 이미터: `FXE_`

새 프로젝트 전용 에셋은 가능하면 `Content/SkillMaker/...` 아래에 둔다.

## 로깅

- SkillMaker 코드에서는 `SK_LOG(LogSkillMaker, Verbosity, TEXT("..."))`를 사용한다.
- 임시 디버그 로그는 작업 완료 전에 제거하거나 Verbosity를 낮춘다.
- 사용자 액션, 저장/로드, 스킬 실행, 네트워크 요청은 식별 가능한 ID를 함께 로그로 남긴다.
- `UE_LOG(LogTemp, ...)`는 기존 코드 유지 외에는 새로 추가하지 않는다.

## 헤더 포함과 의존성

- 헤더에서는 필요한 헤더만 포함한다.
- `.cpp`에는 구현에 필요한 헤더를 직접 포함한다.
- 다른 헤더를 통해 간접적으로 포함된 선언에 의존하지 않는다.
- 새 모듈 의존성이 필요하면 `SkillMaker.Build.cs`에 추가하고 이유를 문서화한다.
- 런타임 모듈에 에디터 전용 API를 직접 넣지 않는다. 에디터 전용 기능이 필요하면 별도 모듈 또는 `#if WITH_EDITOR` 정책을 먼저 정한다.

## 코드 스타일

- 탭/스페이스는 기존 파일 스타일을 따른다.
- 한 줄에 여러 변수를 선언하지 않는다.
- 포인터·참조의 변경 가능 여부에 맞춰 `const`를 정확히 사용한다.
- 값을 변경하지 않는 함수는 `const`를 붙인다.
- 조기 반환을 사용해 중첩을 줄인다.
- 주석은 의도를 설명할 때만 쓴다. 코드가 그대로 말하는 내용을 반복하지 않는다.
- 새 코드에는 의미 없는 기본 주석을 남기지 않는다.

## 검증 규칙

C++ 변경 후 최소 검증:

```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  SkillMakerEditor Mac Development \
  -Project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

UI 변경 후 확인:

- 관련 위젯 블루프린트의 `BindWidget` 이름
- 버튼 델리게이트 중복 바인딩 여부
- WidgetSwitcher 자식 순서와 상태 열거형 순서
- `SkillMakingMap`에서 `WBP_SKSkillMakerEditorMain` 자동 표시 여부
- 스킬 프리뷰 로그에 `SkillID`, 몽타주, 발사체, 노티파이, 피해·범위 값이 출력되는지 여부

데이터 테이블 변경 후 확인:

- `DT_SkillData`
- `DT_WeaponData`
- `DT_AnimationData`
- `DT_ProjectileData`

스킬 실행 변경 후 확인:

- `SkillTrainingMap`에서 Q/E/R/F 슬롯 실행
- 몽타주 재생 여부
- 애님 노티파이 시점의 `CurrentSkillData` 유효성
- 발사체 스폰/충돌
- 상태이상/데미지 적용
- 쿨다운 적용과 해제

## 현재 프로젝트에서 특히 지켜야 할 규칙

- `USKDataManagerSubsystem`은 서브시스템으로 접근한다.
- `USKSkillManager`는 레거시 데이터 테이블 기반 코드로 보고, 신규 UI 저장/조회 경로에 다시 연결하지 않는다.
- `USKDataManagerSubsystem`은 리소스 데이터 테이블 목록 조회 책임만 갖는다.
- `USKSaveGameSubsystem`은 제작된 스킬의 저장/조회 책임을 갖는다.
- `SkillID`와 `SkillName`을 비교 대상으로 혼용하지 않는다.
- `SK_LOG(...)` 호출 뒤에는 항상 세미콜론을 붙인다.
- `InteractionTrigger`, 위젯 클래스, 데이터 테이블 같은 블루프린트에서 지정한 참조는 사용 전 널 여부를 확인한다.
- 공통 위젯은 전달받은 HUD와 편집 데이터의 소유자를 확인한다. 현재 훈련장 참조 설정 함수만으로 데이터 로드·저장 연결이 완료된 것으로 간주하지 않는다.

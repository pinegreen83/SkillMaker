// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerTrainHUD.h"

#include "Character/SKPlayerCharacter.h"
#include "Game/SKSaveGameSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Player/SKPlayerController.h"
#include "UI/UI-SkillMaker/Common/SKMapNavigationWidget.h"
#include "UI/UI-SkillMaker/Common/SkillSelect/SKSkillSelectionWidget.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillSlotAssignmentWidget.h"

ASKSkillMakerTrainHUD::ASKSkillMakerTrainHUD()
{
	static ConstructorHelpers::FClassFinder<USKSkillSelectionWidget> SkillSelectionWidgetClassFinder(
		TEXT("/Game/SkillMaker/UI/WBP_SKSkillSelection.WBP_SKSkillSelection_C"));
	if (SkillSelectionWidgetClassFinder.Class)
	{
		SkillSelectionWidgetClass = SkillSelectionWidgetClassFinder.Class;
	}
}

void ASKSkillMakerTrainHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	SK_LOG(LogSkillMaker, Log, TEXT("훈련장 HUD 초기화: PlayerController=%s"),
		PlayerController ? *PlayerController->GetName() : TEXT("None"));
	PlayerCharacter = PlayerController ? Cast<ASKPlayerCharacter>(PlayerController->GetPawn()) : nullptr;
	if (!PlayerCharacter)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("훈련장 플레이어 캐릭터를 찾을 수 없음."));
	}

	if (SkillSelectionWidgetClass && PlayerController)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 위젯 생성 시작: Class=%s"), *SkillSelectionWidgetClass->GetName());
		SkillSelectionWidget = CreateWidget<USKSkillSelectionWidget>(PlayerController, SkillSelectionWidgetClass);
		if (SkillSelectionWidget)
		{
			SkillSelectionWidget->OnSkillSelected.AddUniqueDynamic(this, &ASKSkillMakerTrainHUD::OnSkillSelected);
			SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 위젯 생성 완료: Widget=%s, DelegateBound=%s"),
				*SkillSelectionWidget->GetName(),
				SkillSelectionWidget->OnSkillSelected.IsAlreadyBound(this, &ASKSkillMakerTrainHUD::OnSkillSelected)
					? TEXT("True") : TEXT("False"));
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("스킬 선택 위젯 생성 실패."));
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 선택 위젯 생성 조건 실패: ClassValid=%s, ControllerValid=%s"),
			SkillSelectionWidgetClass ? TEXT("True") : TEXT("False"), PlayerController ? TEXT("True") : TEXT("False"));
	}

	if (PlayerController)
	{
		SkillSlotAssignmentWidget = CreateWidget<USKSkillSlotAssignmentWidget>(
			PlayerController, USKSkillSlotAssignmentWidget::StaticClass());
		if (SkillSlotAssignmentWidget)
		{
			SkillSlotAssignmentWidget->OnSkillSlotSelected.AddUniqueDynamic(this, &ASKSkillMakerTrainHUD::OnSkillSlotSelected);
			SkillSlotAssignmentWidget->OnAssignmentCancelled.AddUniqueDynamic(
				this, &ASKSkillMakerTrainHUD::OnSkillSlotAssignmentCancelled);
			SK_LOG(LogSkillMaker, Log, TEXT("슬롯 할당 위젯 생성 완료: Widget=%s, SlotDelegate=%s, CancelDelegate=%s"),
				*SkillSlotAssignmentWidget->GetName(),
				SkillSlotAssignmentWidget->OnSkillSlotSelected.IsAlreadyBound(this, &ASKSkillMakerTrainHUD::OnSkillSlotSelected)
					? TEXT("True") : TEXT("False"),
				SkillSlotAssignmentWidget->OnAssignmentCancelled.IsAlreadyBound(
					this, &ASKSkillMakerTrainHUD::OnSkillSlotAssignmentCancelled) ? TEXT("True") : TEXT("False"));
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("슬롯 할당 위젯 생성 실패: NativeClass=%s"),
				*USKSkillSlotAssignmentWidget::StaticClass()->GetName());
		}

		NavigationWidget = CreateWidget<USKMapNavigationWidget>(PlayerController, USKMapNavigationWidget::StaticClass());
		if (NavigationWidget)
		{
			NavigationWidget->Configure(NSLOCTEXT("MapNavigation", "OpenSkillMaking", "스킬 제작"),
				FName(TEXT("/Game/SkillMaker/Map/SkillMakingMap")));
			NavigationWidget->AddToViewport(20);
			NavigationWidget->SetPositionInViewport(FVector2D(24.0f, 24.0f), false);
			NavigationWidget->SetDesiredSizeInViewport(FVector2D(160.0f, 48.0f));
		}

	}

	ShowSkillSelection();
}

void ASKSkillMakerTrainHUD::ShowSkillSelection()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !SkillSelectionWidget)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 선택 화면 표시 실패: 컨트롤러 또는 선택 위젯 없음."));
		return;
	}

	const bool bWasInViewport = SkillSelectionWidget->IsInViewport();
	if (SkillSlotAssignmentWidget && SkillSlotAssignmentWidget->IsInViewport())
	{
		SkillSlotAssignmentWidget->RemoveFromParent();
		SK_LOG(LogSkillMaker, Log, TEXT("슬롯 할당 위젯을 Viewport에서 제거함."));
	}

	if (!bWasInViewport)
	{
		SkillSelectionWidget->AddToViewport(0);
	}
	else
	{
		SkillSelectionWidget->LoadSkillList();
	}
	SkillSelectionWidget->SetVisibility(ESlateVisibility::Visible);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 화면 표시 완료: WasInViewport=%s, IsInViewport=%s, Visibility=%d"),
		bWasInViewport ? TEXT("True") : TEXT("False"),
		SkillSelectionWidget->IsInViewport() ? TEXT("True") : TEXT("False"),
		static_cast<int32>(SkillSelectionWidget->GetVisibility()));

	PlayerController->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(SkillSelectionWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
}

void ASKSkillMakerTrainHUD::ShowSkillAssignmentOverview()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 매핑 현황 화면 열기 시작: SlotWidget=%s, PlayerCharacter=%s"),
		SkillSlotAssignmentWidget ? TEXT("Valid") : TEXT("None"),
		PlayerCharacter ? *PlayerCharacter->GetName() : TEXT("None"));

	if (!SkillSlotAssignmentWidget)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 매핑 현황 화면 열기 실패: 슬롯 위젯 없음."));
		return;
	}

	SkillSlotAssignmentWidget->ShowAssignmentOverview();
	RefreshSkillAssignmentOverview();
	ShowSkillSlotAssignment();
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 매핑 현황 화면 열기 완료."));
}

void ASKSkillMakerTrainHUD::InitializeNewSkill()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	CurrentEditingSkill = FSKSkillData();
	CurrentEditingSkill.SkillName = TEXT("NewSkill");
	SK_LOG(LogSkillMaker, Log, TEXT("새로운 스킬 초기화됨."));
}

void ASKSkillMakerTrainHUD::LoadSkillForEditing(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USKSaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<USKSaveGameSubsystem>())
		{
			FSKSkillData SkillData;
			if (SaveGameSubsystem->GetSkillDataByID(SkillID, SkillData))
			{
				CurrentEditingSkill = SkillData;
				SK_LOG(LogSkillMaker, Log, TEXT("스킬 로드 완료: SkillID=%s / SkillName=%s / DamageValue=%.2f"),
					*CurrentEditingSkill.SkillID.ToString(), *CurrentEditingSkill.SkillName,
					CurrentEditingSkill.DamageValue);
				return;
			}
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("USKSaveGameSubsystem을 찾을 수 없음."));
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("GameInstance를 찾을 수 없음."));
	}

	SK_LOG(LogSkillMaker, Log, TEXT("스킬을 찾을 수 없음. : %s"), *SkillID.ToString());
}

const FSKSkillData& ASKSkillMakerTrainHUD::GetCurrentSkillData() const
{
	return CurrentEditingSkill;
}

void ASKSkillMakerTrainHUD::SetCurrentSkillData(FSKSkillData SkillData)
{
	CurrentEditingSkill = SkillData;
}

void ASKSkillMakerTrainHUD::SetSkillName(const FString& SkillName)
{
	CurrentEditingSkill.SkillName = SkillName;
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 설정 : %s"), *SkillName);
}

void ASKSkillMakerTrainHUD::SetSkillMontage(const TSoftObjectPtr<UAnimMontage>& Montage)
{
	CurrentEditingSkill.SkillMontage = Montage;
	CurrentEditingSkill.SkillDuration = 0.0f;
	SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 설정 : %s"), *Montage.ToSoftObjectPath().ToString());
}

void ASKSkillMakerTrainHUD::LogCurrentSkillData()
{
	SK_LOG(LogSkillMaker, Log, TEXT("==== 현재 스킬 데이터 ===="));
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 : %s"), *CurrentEditingSkill.SkillName);
	SK_LOG(LogSkillMaker, Log, TEXT("무기 태그 : %s"), *CurrentEditingSkill.WeaponTag.ToString());
	if(!CurrentEditingSkill.SkillMontage.IsNull())
	{
		SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 : %s, 길이 : %f"), *CurrentEditingSkill.SkillMontage.ToSoftObjectPath().ToString(), CurrentEditingSkill.SkillDuration);
	}
	if(!CurrentEditingSkill.ProjectileActor.IsNull())
	{
		SK_LOG(LogSkillMaker, Log, TEXT("발사체 설정 완료."));
	}
	SK_LOG(LogSkillMaker, Log, TEXT("=================="));
}

void ASKSkillMakerTrainHUD::OnSkillSelected(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 이벤트 수신: SkillID=%s, SelectionWidget=%s, SlotWidget=%s"),
		*SkillID.ToString(), SkillSelectionWidget ? TEXT("Valid") : TEXT("None"),
		SkillSlotAssignmentWidget ? TEXT("Valid") : TEXT("None"));
	LoadSkillForEditing(SkillID);
	if (CurrentEditingSkill.SkillID != SkillID)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("테스트 스킬 선택 실패: %s"), *SkillID.ToString());
		return;
	}

	if (!SkillSlotAssignmentWidget)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬은 로드했지만 슬롯 할당 위젯이 없어 화면을 전환할 수 없음."));
		return;
	}

	SkillSlotAssignmentWidget->SetSelectedSkill(CurrentEditingSkill);
	ShowSkillSlotAssignment();

	SK_LOG(LogSkillMaker, Log, TEXT("테스트 스킬 선택: ID=%s / Name=%s"),
		*CurrentEditingSkill.SkillID.ToString(), *CurrentEditingSkill.SkillName);
}

void ASKSkillMakerTrainHUD::OnSkillSlotSelected(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= 4)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("슬롯 할당 실패: 잘못된 슬롯 인덱스 %d"), SlotIndex);
		return;
	}

	if (!PlayerCharacter || CurrentEditingSkill.SkillID.IsNone())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("슬롯 할당 실패: 플레이어 또는 선택 스킬 없음."));
		return;
	}

	ASKPlayerController* PlayerController = Cast<ASKPlayerController>(PlayerCharacter->GetController());
	if (!PlayerController)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("슬롯 할당 실패: ASKPlayerController를 찾을 수 없음."));
		return;
	}

	PlayerCharacter->SetSkillDataInMap(CurrentEditingSkill);
	SK_LOG(LogSkillMaker, Log, TEXT("플레이어 SkillMap 등록 완료: Character=%s, SkillID=%s"),
		*PlayerCharacter->GetName(), *CurrentEditingSkill.SkillID.ToString());
	PlayerController->SetSkillInSlot(SlotIndex, CurrentEditingSkill.SkillID);
	if (SkillSlotAssignmentWidget)
	{
		SkillSlotAssignmentWidget->SetAssignedSkill(SlotIndex, CurrentEditingSkill.SkillName);
	}

	static const TCHAR* SlotKeys[] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("F") };
	const TCHAR* SlotKey = SlotIndex >= 0 && SlotIndex < UE_ARRAY_COUNT(SlotKeys) ? SlotKeys[SlotIndex] : TEXT("Unknown");
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 슬롯 할당: Key=%s / SkillID=%s / SkillName=%s"),
		SlotKey, *CurrentEditingSkill.SkillID.ToString(), *CurrentEditingSkill.SkillName);

	CloseSkillAssignmentUI();
}

void ASKSkillMakerTrainHUD::OnSkillSlotAssignmentCancelled()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 화면 요청 수신: 저장 스킬 목록으로 이동."));
	ShowSkillSelection();
}

void ASKSkillMakerTrainHUD::ShowSkillSlotAssignment()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !SkillSlotAssignmentWidget)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 슬롯 선택 화면 표시 실패."));
		return;
	}

	SK_LOG(LogSkillMaker, Log, TEXT("슬롯 할당 화면 전환 시작: SelectionInViewport=%s, SlotInViewport=%s"),
		SkillSelectionWidget && SkillSelectionWidget->IsInViewport() ? TEXT("True") : TEXT("False"),
		SkillSlotAssignmentWidget->IsInViewport() ? TEXT("True") : TEXT("False"));

	if (SkillSelectionWidget && SkillSelectionWidget->IsInViewport())
	{
		SkillSelectionWidget->RemoveFromParent();
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 위젯을 Viewport에서 제거함."));
	}

	if (!SkillSlotAssignmentWidget->IsInViewport())
	{
		SkillSlotAssignmentWidget->AddToViewport(10);
	}
	SkillSlotAssignmentWidget->SetVisibility(ESlateVisibility::Visible);

	int32 ViewportWidth = 0;
	int32 ViewportHeight = 0;
	PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);
	const FVector2D ViewportCenter(
		static_cast<float>(ViewportWidth) * 0.5f,
		static_cast<float>(ViewportHeight) * 0.5f);

	SkillSlotAssignmentWidget->SetAnchorsInViewport(FAnchors(0.0f, 0.0f));
	SkillSlotAssignmentWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	SkillSlotAssignmentWidget->SetPositionInViewport(ViewportCenter, true);
	SkillSlotAssignmentWidget->SetDesiredSizeInViewport(FVector2D(520.0f, 420.0f));
	SkillSlotAssignmentWidget->SetIsEnabled(true);
	SkillSlotAssignmentWidget->ForceLayoutPrepass();

	PlayerController->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(SkillSlotAssignmentWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);

	SK_LOG(LogSkillMaker, Log,
		TEXT("슬롯 할당 화면 표시 완료: Viewport=%dx%d, Position=%s, IsInViewport=%s, Visibility=%d, Enabled=%s, DesiredSize=%s"),
		ViewportWidth, ViewportHeight, *ViewportCenter.ToString(),
		SkillSlotAssignmentWidget->IsInViewport() ? TEXT("True") : TEXT("False"),
		static_cast<int32>(SkillSlotAssignmentWidget->GetVisibility()),
		SkillSlotAssignmentWidget->GetIsEnabled() ? TEXT("True") : TEXT("False"),
		*SkillSlotAssignmentWidget->GetDesiredSize().ToString());
}

void ASKSkillMakerTrainHUD::RefreshSkillAssignmentOverview()
{
	ASKPlayerController* PlayerController = PlayerCharacter
		? Cast<ASKPlayerController>(PlayerCharacter->GetController()) : nullptr;
	USKSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()
		? GetGameInstance()->GetSubsystem<USKSaveGameSubsystem>() : nullptr;
	if (!PlayerController || !SkillSlotAssignmentWidget)
	{
		SK_LOG(LogSkillMaker, Error,
			TEXT("스킬 매핑 현황 갱신 실패: PlayerController=%s, SlotWidget=%s"),
			PlayerController ? TEXT("Valid") : TEXT("None"),
			SkillSlotAssignmentWidget ? TEXT("Valid") : TEXT("None"));
		return;
	}

	static const TCHAR* SlotKeys[] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("F") };
	for (int32 SlotIndex = 0; SlotIndex < UE_ARRAY_COUNT(SlotKeys); ++SlotIndex)
	{
		const FName SkillID = PlayerController->GetSkillInSlot(SlotIndex);
		FString SkillName;
		if (!SkillID.IsNone() && SaveGameSubsystem)
		{
			FSKSkillData SkillData;
			if (SaveGameSubsystem->GetSkillDataByID(SkillID, SkillData))
			{
				SkillName = SkillData.SkillName;
			}
			else
			{
				SkillName = SkillID.ToString();
				SK_LOG(LogSkillMaker, Warning,
					TEXT("슬롯 스킬 이름 조회 실패: Key=%s, SkillID=%s, ID로 표시함."),
					SlotKeys[SlotIndex], *SkillID.ToString());
			}
		}

		SkillSlotAssignmentWidget->SetAssignedSkill(SlotIndex, SkillName);
		SK_LOG(LogSkillMaker, Log, TEXT("현재 스킬 매핑: Key=%s, SlotIndex=%d, SkillID=%s, SkillName=%s"),
			SlotKeys[SlotIndex], SlotIndex, *SkillID.ToString(),
			SkillName.IsEmpty() ? TEXT("비어 있음") : *SkillName);
	}
}

void ASKSkillMakerTrainHUD::CloseSkillAssignmentUI()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 할당 UI 종료 시작."));
	if (SkillSelectionWidget && SkillSelectionWidget->IsInViewport())
	{
		SkillSelectionWidget->RemoveFromParent();
	}
	if (SkillSlotAssignmentWidget && SkillSlotAssignmentWidget->IsInViewport())
	{
		SkillSlotAssignmentWidget->RemoveFromParent();
	}

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 할당 UI 종료 완료: GameOnly 입력 복귀, MouseCursor=False"));
	}
}

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
	PlayerCharacter = PlayerController ? Cast<ASKPlayerCharacter>(PlayerController->GetPawn()) : nullptr;
	if (!PlayerCharacter)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("훈련장 플레이어 캐릭터를 찾을 수 없음."));
	}

	if (SkillSelectionWidgetClass && PlayerController)
	{
		SkillSelectionWidget = CreateWidget<USKSkillSelectionWidget>(PlayerController, SkillSelectionWidgetClass);
		if (SkillSelectionWidget)
		{
			SkillSelectionWidget->OnSkillSelected.AddUniqueDynamic(this, &ASKSkillMakerTrainHUD::OnSkillSelected);
			SkillSelectionWidget->AddToViewport(0);
		}
	}

	if (PlayerController)
	{
		SkillSlotAssignmentWidget = CreateWidget<USKSkillSlotAssignmentWidget>(
			PlayerController, USKSkillSlotAssignmentWidget::StaticClass());
		if (SkillSlotAssignmentWidget)
		{
			SkillSlotAssignmentWidget->OnSkillSlotSelected.AddUniqueDynamic(this, &ASKSkillMakerTrainHUD::OnSkillSlotSelected);
			SkillSlotAssignmentWidget->AddToViewport(10);
			SkillSlotAssignmentWidget->SetAnchorsInViewport(FAnchors(1.0f, 0.0f));
			SkillSlotAssignmentWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.0f));
			SkillSlotAssignmentWidget->SetPositionInViewport(FVector2D(-24.0f, 80.0f), false);
			SkillSlotAssignmentWidget->SetDesiredSizeInViewport(FVector2D(360.0f, 300.0f));
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

	SkillSelectionWidget->LoadSkillList();
	SkillSelectionWidget->SetVisibility(ESlateVisibility::Visible);
	if (SkillSlotAssignmentWidget)
	{
		SkillSlotAssignmentWidget->SetVisibility(ESlateVisibility::Visible);
	}

	PlayerController->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(SkillSelectionWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
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
				SK_LOG(LogSkillMaker, Log, TEXT("스킬 로드 완료 : %s"), *CurrentEditingSkill.SkillName);
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
	LoadSkillForEditing(SkillID);
	if (CurrentEditingSkill.SkillID != SkillID)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("테스트 스킬 선택 실패: %s"), *SkillID.ToString());
		return;
	}

	if (SkillSlotAssignmentWidget)
	{
		SkillSlotAssignmentWidget->SetSelectedSkill(CurrentEditingSkill);
	}

	SK_LOG(LogSkillMaker, Log, TEXT("테스트 스킬 선택: ID=%s / Name=%s"),
		*CurrentEditingSkill.SkillID.ToString(), *CurrentEditingSkill.SkillName);
}

void ASKSkillMakerTrainHUD::OnSkillSlotSelected(int32 SlotIndex)
{
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
	PlayerController->SetSkillInSlot(SlotIndex, CurrentEditingSkill.SkillID);
	if (SkillSlotAssignmentWidget)
	{
		SkillSlotAssignmentWidget->SetAssignedSkill(SlotIndex, CurrentEditingSkill.SkillName);
	}

	static const TCHAR* SlotKeys[] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("F") };
	const TCHAR* SlotKey = SlotIndex >= 0 && SlotIndex < UE_ARRAY_COUNT(SlotKeys) ? SlotKeys[SlotIndex] : TEXT("Unknown");
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 슬롯 할당: Key=%s / SkillID=%s / SkillName=%s"),
		SlotKey, *CurrentEditingSkill.SkillID.ToString(), *CurrentEditingSkill.SkillName);
}

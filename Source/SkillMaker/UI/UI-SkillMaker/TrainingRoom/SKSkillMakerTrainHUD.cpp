// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerTrainHUD.h"

#include "SKSkillMakerTrainMainWidget.h"
#include "Engine/Texture2D.h"
#include "Game/SKSaveGameSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Logging/SKLogSkillMakerMacro.h"

ASKSkillMakerTrainHUD::ASKSkillMakerTrainHUD()
{
	// static ConstructorHelpers::FClassFinder<USKSkillMakerTrainMainWidget> MainWidgetClassFinder(TEXT("/Game/SkillMaker/UI/WBP_SKSkillMakerMain.WBP_SKSkillMakerMain_C"));
	// if(MainWidgetClassFinder.Class)
	// {
	// 	MainWidgetClass = MainWidgetClassFinder.Class;
	// }
}

void ASKSkillMakerTrainHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainWidgetClass)
	{
		MainWidget = CreateWidget<USKSkillMakerTrainMainWidget>(GetWorld(), MainWidgetClass);
		if(MainWidget)
		{
			MainWidget->SetHUDReference(this);
		}
	}
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

void ASKSkillMakerTrainHUD::SetSkillMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		CurrentEditingSkill.SkillMontage = Montage;
		CurrentEditingSkill.SkillDuration = Montage->GetPlayLength();
		SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 설정 : %s (길이 : %f)"), *Montage->GetName(), CurrentEditingSkill.SkillDuration);
	}
}

void ASKSkillMakerTrainHUD::LogCurrentSkillData()
{
	SK_LOG(LogSkillMaker, Log, TEXT("==== 현재 스킬 데이터 ===="));
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 : %s"), *CurrentEditingSkill.SkillName);
	SK_LOG(LogSkillMaker, Log, TEXT("무기 타입 : %s"), *CurrentEditingSkill.WeaponType);
	if(CurrentEditingSkill.SkillMontage)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 : %s, 길이 : %f"), *CurrentEditingSkill.SkillMontage->GetName(), CurrentEditingSkill.SkillDuration);
	}
	if(CurrentEditingSkill.ProjectileActor)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("발사체 설정 완료."));
	}
	SK_LOG(LogSkillMaker, Log, TEXT("=================="));
}

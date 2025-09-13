// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerTrainHUD.h"
#include "Skill/SKSkillManager.h"
#include "Engine/Texture2D.h"
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

	// if(MainWidgetClass)
	// {
	// 	MainWidget = CreateWidget<USKSkillMakerEditorMainWidget>(GetWorld(), MainWidgetClass);
	// 	if(MainWidget)
	// 	{
	// 		MainWidget->SetHUDReference(this);
	// 	}
	// }
	
	// InitializeNewSkill();
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

void ASKSkillMakerTrainHUD::SetSkillWeaponType(const FString& WeaponType)
{
	CurrentEditingSkill.WeaponType = WeaponType;
	SK_LOG(LogSkillMaker, Log, TEXT("무기 타입 설정 : %s"), *WeaponType);
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

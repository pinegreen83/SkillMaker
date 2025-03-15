// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerHUD.h"
#include "SKSkillMakerMainWidget.h"
#include "Skill/SKSkillManager.h"
#include "Engine/Texture2D.h"

ASKSkillMakerHUD::ASKSkillMakerHUD()
{
	static ConstructorHelpers::FClassFinder<USKSkillMakerMainWidget> MainWidgetClassFinder(TEXT("/Game/SkillMaker/UI/WBP_SkillMakerMain.WBP_SkillMakerMain_C"));
	if(MainWidgetClassFinder.Class)
	{
		MainWidgetClass = MainWidgetClassFinder.Class;
	}
}

void ASKSkillMakerHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainWidgetClass)
	{
		MainWidget = CreateWidget<USKSkillMakerMainWidget>(GetWorld(), MainWidgetClass);
		if(MainWidget)
		{
			MainWidget->SetHUDReference(this);
			MainWidget->AddToViewport();
		}
	}

	InitializeNewSkill();
}

void ASKSkillMakerHUD::InitializeNewSkill()
{
	CurrentEditingSkill = FSKSkillData();
	CurrentEditingSkill.SkillName = TEXT("NewSkill");
	UE_LOG(LogTemp, Log, TEXT("새로운 스킬 초기화됨."));
}

void ASKSkillMakerHUD::LoadSkillForEditing(const FName& SkillID)
{
	if(TOptional<FSKSkillData> SkillData = USKSkillManager::Get()->GetSkillDataByID(SkillID))
	{
		CurrentEditingSkill = SkillData.GetValue();
		UE_LOG(LogTemp, Log, TEXT("스킬 로드 완료 : %s"), *CurrentEditingSkill.SkillName);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("스킬을 찾을 수 없음. : %s"), *SkillID.ToString());
	}
}

const FSKSkillData& ASKSkillMakerHUD::GetCurrentSkillData() const
{
	return CurrentEditingSkill;
}

void ASKSkillMakerHUD::SetCurrentSkillData(FSKSkillData SkillData)
{
	CurrentEditingSkill = SkillData;
}

void ASKSkillMakerHUD::SetSkillName(const FString& SkillName)
{
	CurrentEditingSkill.SkillName = SkillName;
	UE_LOG(LogTemp, Log, TEXT("스킬 이름 설정 : %s"), *SkillName);
}

void ASKSkillMakerHUD::SetSkillWeaponType(const FString& WeaponType)
{
	CurrentEditingSkill.WeaponType = WeaponType;
	UE_LOG(LogTemp, Log, TEXT("무기 타입 설정 : %s"), *WeaponType);
}

void ASKSkillMakerHUD::SetSkillMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		CurrentEditingSkill.SkillMontage = Montage;
		CurrentEditingSkill.SkillDuration = Montage->GetPlayLength();
		UE_LOG(LogTemp, Log, TEXT("애니메이션 설정 : %s (길이 : %f)"), *Montage->GetName(), CurrentEditingSkill.SkillDuration);
	}
}

// void ASKSkillMakerHUD::SetSelectedAnimation(UAnimMontage* AnimationMontage)
// {
// 	if(AnimationMontage)
// 	{
// 		CurrentEditingSkill.SkillMontage = AnimationMontage;
// 		CurrentEditingSkill.SkillDuration = AnimationMontage->GetPlayLength();
// 		UE_LOG(LogTemp, Log, TEXT("애니메이션 선택 : %s, 길이 : %f"), *AnimationMontage->GetName(), CurrentEditingSkill.SkillDuration);
// 	}
// }
//
// void ASKSkillMakerHUD::AddEffectToSkill(const FName& NotifyName, const FName& EffectName)
// {
// 	if(!CurrentEditingSkill.EffectNotifyNames.Contains(NotifyName))
// 	{
// 		CurrentEditingSkill.EffectNotifyNames.Add(NotifyName);
// 	}
// 	CurrentEditingSkill.EffectSoundMapping.Add(NotifyName, EffectName);
// 	UE_LOG(LogTemp, Log, TEXT("이펙트 추가 : %s -> %s"), *NotifyName.ToString(), *EffectName.ToString());
// }
//
// void ASKSkillMakerHUD::AddSoundToSkill(const FName& NotifyName, const FName& SoundName)
// {
// 	if(!CurrentEditingSkill.SoundNotifyNames.Contains(NotifyName))
// 	{
// 		CurrentEditingSkill.SoundNotifyNames.Add(NotifyName);
// 	}
// 	CurrentEditingSkill.EffectSoundMapping.Add(NotifyName, SoundName);
// 	UE_LOG(LogTemp, Log, TEXT("사운드 추가 : %s -> %s"), *NotifyName.ToString(), *SoundName.ToString());
// }

void ASKSkillMakerHUD::LogCurrentSkillData()
{
	UE_LOG(LogTemp, Log, TEXT("==== 현재 스킬 데이터 ===="));
	UE_LOG(LogTemp, Log, TEXT("스킬 이름 : %s"), *CurrentEditingSkill.SkillName);
	UE_LOG(LogTemp, Log, TEXT("무기 타입 : %s"), *CurrentEditingSkill.WeaponType);
	if(CurrentEditingSkill.SkillMontage)
	{
		UE_LOG(LogTemp, Log, TEXT("애니메이션 : %s, 길이 : %f"), *CurrentEditingSkill.SkillMontage->GetName(), CurrentEditingSkill.SkillDuration);
	}
	// UE_LOG(LogTemp, Log, TEXT("이펙트 목록 : "));
	// for(const auto& Effect : CurrentEditingSkill.EffectNotifyNames)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT(" - %s -> %s"), *Effect.ToString(), *CurrentEditingSkill.EffectSoundMapping[Effect].ToString());
	// }
	UE_LOG(LogTemp, Log, TEXT("=================="));
}
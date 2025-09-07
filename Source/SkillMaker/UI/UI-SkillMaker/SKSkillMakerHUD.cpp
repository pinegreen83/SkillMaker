// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerHUD.h"
#include "SKSkillMakerMainWidget.h"
#include "Skill/SKSkillManager.h"
#include "Skill/SKSkillEffectActor.h"
#include "Animation/SKSkillAnimNotify_Trigger.h"
#include "Engine/Texture2D.h"
#include "Character/SKPreviewCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"

ASKSkillMakerHUD::ASKSkillMakerHUD()
{
	static ConstructorHelpers::FClassFinder<USKSkillMakerMainWidget> MainWidgetClassFinder(TEXT("/Game/SkillMaker/UI/WBP_SKSkillMakerMain.WBP_SKSkillMakerMain_C"));
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

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PreviewCharacter = GetWorld()->SpawnActor<ASKPreviewCharacter>(ASKPreviewCharacter::StaticClass(), FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);

	if (PreviewCharacter)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("프리뷰 캐릭터 생성 완료."));
	}
}

void ASKSkillMakerHUD::InitializeNewSkill()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	CurrentEditingSkill = FSKSkillData();
	CurrentEditingSkill.SkillName = TEXT("NewSkill");
	SK_LOG(LogSkillMaker, Log, TEXT("새로운 스킬 초기화됨."));
}

void ASKSkillMakerHUD::LoadSkillForEditing(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(TOptional<FSKSkillData> SkillData = USKSkillManager::Get()->GetSkillDataByID(SkillID))
	{
		CurrentEditingSkill = SkillData.GetValue();
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 로드 완료 : %s"), *CurrentEditingSkill.SkillName);
	}
	else
	{
		SK_LOG(LogSkillMaker, Log, TEXT("스킬을 찾을 수 없음. : %s"), *SkillID.ToString());
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
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 설정 : %s"), *SkillName);
}

void ASKSkillMakerHUD::SetSkillWeaponType(const FString& WeaponType)
{
	CurrentEditingSkill.WeaponType = WeaponType;
	SK_LOG(LogSkillMaker, Log, TEXT("무기 타입 설정 : %s"), *WeaponType);
}

void ASKSkillMakerHUD::SetSkillMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		CurrentEditingSkill.SkillMontage = Montage;
		CurrentEditingSkill.SkillDuration = Montage->GetPlayLength();
		SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 설정 : %s (길이 : %f)"), *Montage->GetName(), CurrentEditingSkill.SkillDuration);
	}
}

void ASKSkillMakerHUD::LogCurrentSkillData()
{
	SK_LOG(LogSkillMaker, Log, TEXT("==== 현재 스킬 데이터 ===="));
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 : %s"), *CurrentEditingSkill.SkillName);
	SK_LOG(LogSkillMaker, Log, TEXT("무기 타입 : %s"), *CurrentEditingSkill.WeaponType);
	if(CurrentEditingSkill.SkillMontage)
	{
		UE_LOG(LogTemp, Log, TEXT("애니메이션 : %s, 길이 : %f"), *CurrentEditingSkill.SkillMontage->GetName(), CurrentEditingSkill.SkillDuration);
	}
	// UE_LOG(LogTemp, Log, TEXT("이펙트 목록 : "));
	// for(const auto& Effect : CurrentEditingSkill.EffectNotifyNames)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT(" - %s -> %s"), *Effect.ToString(), *CurrentEditingSkill.EffectSoundMapping[Effect].ToString());
	// }
	SK_LOG(LogSkillMaker, Log, TEXT("=================="));
}

void ASKSkillMakerHUD::PreviewSkillEffect(const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!PreviewCharacter || !SkillData.SkillMontage)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("미리보기 실행 불가: 캐릭터 또는 애니메이션 없음."));
		return;
	}

	PreviewCharacter->SetSkillData(SkillData);
	PreviewCharacter->UseSkill(SkillData.SkillID); 
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerEditorHUD.h"
#include "SKSkillMakerEditorMainWidget.h"
#include "Engine/Texture2D.h"
#include "Character/SKPreviewCharacter.h"
#include "Game/SKSaveGameSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Logging/SKLogSkillMakerMacro.h"

ASKSkillMakerEditorHUD::ASKSkillMakerEditorHUD()
{
	static ConstructorHelpers::FClassFinder<USKSkillMakerEditorMainWidget> MainWidgetClassFinder(TEXT("/Game/SkillMaker/UI/Editor/WBP_SKSkillMakerEditorMain.WBP_SKSkillMakerEditorMain_C"));
	if(MainWidgetClassFinder.Class)
	{
		MainWidgetClass = MainWidgetClassFinder.Class;
	}
}

void ASKSkillMakerEditorHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainWidgetClass)
	{
		MainWidget = CreateWidget<USKSkillMakerEditorMainWidget>(GetWorld(), MainWidgetClass);
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

void ASKSkillMakerEditorHUD::InitializeNewSkill()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	CurrentEditingSkill = FSKSkillData();
	CurrentEditingSkill.SkillName = TEXT("NewSkill");
	SK_LOG(LogSkillMaker, Log, TEXT("새로운 스킬 초기화됨."));
}

void ASKSkillMakerEditorHUD::LoadSkillForEditing(const FName& SkillID)
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

const FSKSkillData& ASKSkillMakerEditorHUD::GetCurrentSkillData() const
{
	return CurrentEditingSkill;
}

void ASKSkillMakerEditorHUD::SetCurrentSkillData(FSKSkillData SkillData)
{
	CurrentEditingSkill = SkillData;
}

void ASKSkillMakerEditorHUD::SetSkillName(const FString& SkillName)
{
	CurrentEditingSkill.SkillName = SkillName;
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 설정 : %s"), *SkillName);
}

void ASKSkillMakerEditorHUD::SetSkillWeaponType(const FString& WeaponType)
{
	CurrentEditingSkill.WeaponType = WeaponType;
	SK_LOG(LogSkillMaker, Log, TEXT("무기 타입 설정 : %s"), *WeaponType);
}

void ASKSkillMakerEditorHUD::SetSkillMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		CurrentEditingSkill.SkillMontage = Montage;
		CurrentEditingSkill.SkillDuration = Montage->GetPlayLength();
		SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 설정 : %s (길이 : %f)"), *Montage->GetName(), CurrentEditingSkill.SkillDuration);
	}
}

void ASKSkillMakerEditorHUD::LogCurrentSkillData()
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

void ASKSkillMakerEditorHUD::PreviewSkillEffect(const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!PreviewCharacter || !SkillData.SkillMontage)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("미리보기 실행 불가: 캐릭터 또는 애니메이션 없음."));
		return;
	}

	FSKSkillData PreviewSkillData = SkillData;
	if (PreviewSkillData.SkillID.IsNone())
	{
		PreviewSkillData.SkillID = TEXT("PreviewSkill");
	}

	SK_LOG(LogSkillMaker, Log, TEXT("==== 스킬 미리보기 데이터 ===="));
	SK_LOG(LogSkillMaker, Log, TEXT("SkillID: %s"), *PreviewSkillData.SkillID.ToString());
	SK_LOG(LogSkillMaker, Log, TEXT("SkillName: %s"), *PreviewSkillData.SkillName);
	SK_LOG(LogSkillMaker, Log, TEXT("SkillType: %d"), static_cast<int32>(PreviewSkillData.SkillType));
	SK_LOG(LogSkillMaker, Log, TEXT("WeaponType: %s"), *PreviewSkillData.WeaponType);
	SK_LOG(LogSkillMaker, Log, TEXT("Montage: %s"),
		PreviewSkillData.SkillMontage ? *PreviewSkillData.SkillMontage->GetName() : TEXT("None"));
	SK_LOG(LogSkillMaker, Log, TEXT("Projectile: %s"),
		PreviewSkillData.ProjectileActor ? *PreviewSkillData.ProjectileActor->GetName() : TEXT("None"));
	SK_LOG(LogSkillMaker, Log, TEXT("NotifyName: %s"), *PreviewSkillData.NotifyName.ToString());
	SK_LOG(LogSkillMaker, Log, TEXT("Damage: %.2f"), PreviewSkillData.DamageValue);
	SK_LOG(LogSkillMaker, Log, TEXT("Cooldown: %.2f"), PreviewSkillData.CooldownTime);
	SK_LOG(LogSkillMaker, Log, TEXT("Range: %.2f ~ %.2f"), PreviewSkillData.MinRange, PreviewSkillData.MaxRange);
	SK_LOG(LogSkillMaker, Log, TEXT("StatusEffect Count: %d"), PreviewSkillData.StatusEffects.Num());
	SK_LOG(LogSkillMaker, Log, TEXT("============================"));

	PreviewCharacter->SetSkillDataInMap(PreviewSkillData);
	PreviewCharacter->UseSkill(PreviewSkillData.SkillID);
}

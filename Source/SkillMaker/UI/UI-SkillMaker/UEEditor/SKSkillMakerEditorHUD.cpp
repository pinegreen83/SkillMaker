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
	OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
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
				OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
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

void ASKSkillMakerEditorHUD::SetCurrentSkillData(const FSKSkillData& SkillData)
{
	CurrentEditingSkill = SkillData;
	OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
}

void ASKSkillMakerEditorHUD::SetSkillName(const FString& SkillName)
{
	CurrentEditingSkill.SkillName = SkillName;
	OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 설정 : %s"), *SkillName);
}

void ASKSkillMakerEditorHUD::SetSkillWeaponTag(FGameplayTag WeaponTag)
{
	if (!WeaponTag.IsValid())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("유효하지 않은 무기 태그는 설정할 수 없음."));
		return;
	}
	if (CurrentEditingSkill.WeaponTag == WeaponTag)
	{
		return;
	}

	CurrentEditingSkill.WeaponTag = WeaponTag;
	CurrentEditingSkill.SkillMontage.Reset();
	CurrentEditingSkill.SkillDuration = 0.0f;
	CurrentEditingSkill.ProjectileActor.Reset();
	CurrentEditingSkill.NotifyName = NAME_None;
	CurrentEditingSkill.StatusEffects.Reset();
	OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
	SK_LOG(LogSkillMaker, Log, TEXT("무기 태그 설정 : %s"), *WeaponTag.ToString());
}

void ASKSkillMakerEditorHUD::SetSkillMontage(const TSoftObjectPtr<UAnimMontage>& Montage)
{
	if (Montage.IsNull())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("비어 있는 애니메이션은 설정할 수 없음."));
		return;
	}
	if (CurrentEditingSkill.SkillMontage == Montage)
	{
		return;
	}

	CurrentEditingSkill.SkillMontage = Montage;
	CurrentEditingSkill.SkillDuration = 0.0f;
	CurrentEditingSkill.NotifyName = NAME_None;
	OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
	SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 설정 : %s"), *Montage.ToSoftObjectPath().ToString());
}

bool ASKSkillMakerEditorHUD::SaveCurrentSkill(const FString& SkillName)
{
	if (SkillName.IsEmpty())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 이름이 입력되지 않음."));
		return false;
	}

	if (!CurrentEditingSkill.WeaponTag.IsValid() || CurrentEditingSkill.SkillMontage.IsNull())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("무기 또는 애니메이션이 설정되지 않아 저장할 수 없음."));
		return false;
	}
	if (CurrentEditingSkill.ProjectileActor.IsNull() || CurrentEditingSkill.NotifyName.IsNone())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("이펙트 또는 발동 지점이 설정되지 않아 저장할 수 없음."));
		return false;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("GameInstance를 찾을 수 없음."));
		return false;
	}

	USKSaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<USKSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("USKSaveGameSubsystem을 찾을 수 없음."));
		return false;
	}

	FSKSkillData SaveCandidate = CurrentEditingSkill;
	SaveCandidate.SkillName = SkillName;
	if (SaveCandidate.SkillID.IsNone())
	{
		SaveCandidate.SkillID = FName(*FGuid::NewGuid().ToString());
	}

	if (!SaveGameSubsystem->SaveSkillData(SaveCandidate.SkillID, SaveCandidate))
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 SaveGame 저장 실패: %s"), *SaveCandidate.SkillID.ToString());
		return false;
	}

	CurrentEditingSkill = MoveTemp(SaveCandidate);
	OnEditingSkillChanged.Broadcast(CurrentEditingSkill);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 저장 완료: %s"), *CurrentEditingSkill.SkillName);
	return true;
}

void ASKSkillMakerEditorHUD::LogCurrentSkillData()
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

void ASKSkillMakerEditorHUD::PreviewSkillEffect(const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!PreviewCharacter || SkillData.SkillMontage.IsNull())
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
	SK_LOG(LogSkillMaker, Log, TEXT("WeaponTag: %s"), *PreviewSkillData.WeaponTag.ToString());
	SK_LOG(LogSkillMaker, Log, TEXT("Montage: %s"),
		PreviewSkillData.SkillMontage.IsNull() ? TEXT("None") : *PreviewSkillData.SkillMontage.ToSoftObjectPath().ToString());
	SK_LOG(LogSkillMaker, Log, TEXT("Projectile: %s"),
		PreviewSkillData.ProjectileActor.IsNull() ? TEXT("None") : *PreviewSkillData.ProjectileActor.ToSoftObjectPath().ToString());
	SK_LOG(LogSkillMaker, Log, TEXT("NotifyName: %s"), *PreviewSkillData.NotifyName.ToString());
	SK_LOG(LogSkillMaker, Log, TEXT("Damage: %.2f"), PreviewSkillData.DamageValue);
	SK_LOG(LogSkillMaker, Log, TEXT("Cooldown: %.2f"), PreviewSkillData.CooldownTime);
	SK_LOG(LogSkillMaker, Log, TEXT("Range: %.2f ~ %.2f"), PreviewSkillData.MinRange, PreviewSkillData.MaxRange);
	SK_LOG(LogSkillMaker, Log, TEXT("StatusEffect Count: %d"), PreviewSkillData.StatusEffects.Num());
	SK_LOG(LogSkillMaker, Log, TEXT("============================"));

	PreviewCharacter->SetSkillDataInMap(PreviewSkillData);
	PreviewCharacter->UseSkill(PreviewSkillData.SkillID);
}

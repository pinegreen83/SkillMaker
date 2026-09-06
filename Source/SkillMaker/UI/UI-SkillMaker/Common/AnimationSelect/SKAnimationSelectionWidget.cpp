// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimationSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/SKAnimationData.h"
#include "SKAnimationCardWidget.h"
#include "Game/SKDataManagerSubsystem.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKAnimationSelectionWidget::LoadAnimationsForWeapon(FGameplayTag WeaponTag, const TSoftObjectPtr<UAnimMontage>& CurrentMontage)
{
	SK_LOG(LogSkillMaker, Log, TEXT("%s에 대한 애니메이션 로드"), *WeaponTag.ToString());

	if (!AnimationGridPanel)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("AnimationGridPanel이 nullptr임."));
		return;
	}

	AnimationGridPanel->ClearChildren();
	if (!WeaponTag.IsValid())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("유효하지 않은 무기 태그로 애니메이션을 요청함."));
		return;
	}
	
	if(USKDataManagerSubsystem* DataManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USKDataManagerSubsystem>())
	{
		TArray<FSKAnimationRow> AnimList = DataManagerSubsystem->GetAnimationsForWeapon(WeaponTag);
		
		int32 index = 0;
		for(const auto& AnimData : AnimList)
		{
			SK_LOG(LogSkillMaker, Log, TEXT("불러온 애니메이션 : %s"), *AnimData.RowName.ToString());

			CreateAnimationCard(AnimData, index, CurrentMontage);
			index++;
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("USKDataManagerSubsystem을 찾을 수 없음."));
	}
}

void USKAnimationSelectionWidget::CreateAnimationCard(const FSKAnimationRow& AnimationRow, const int32 AnimIndex, const TSoftObjectPtr<UAnimMontage>& CurrentMontage)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!AnimationGridPanel || !WBP_SKAnimationCard)
		return;

	if(USKAnimationCardWidget* AnimationCard = CreateWidget<USKAnimationCardWidget>(GetWorld(), WBP_SKAnimationCard))
	{
		if (!AnimationCard->SetAnimationInfo(AnimationRow.Data.AnimationName, AnimationRow.Data.AnimationMontage, AnimationRow.Data.AnimationThumbnail,
			AnimationRow.Data.AnimationMontage == CurrentMontage))
		{
			SK_LOG(LogSkillMaker, Warning, TEXT("몽타주가 없는 애니메이션 행: %s"), *AnimationRow.RowName.ToString());
			return;
		}
		AnimationCard->OnAnimationCardSelected.AddDynamic(this, &USKAnimationSelectionWidget::AnimationSelected);
		AnimationGridPanel->AddChildToUniformGrid(AnimationCard, AnimIndex/2, AnimIndex%2);
	}
}

void USKAnimationSelectionWidget::AnimationSelected(const TSoftObjectPtr<UAnimMontage>& SelectedMontage)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(SelectedMontage.IsNull())
		return;

	SK_LOG(LogSkillMaker, Log, TEXT("선택된 애니메이션 : %s"), *SelectedMontage.ToSoftObjectPath().ToString());

	OnAnimationSelected.Broadcast(SelectedMontage);
}

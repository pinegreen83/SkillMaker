// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimationSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/SKDataManager.h"
#include "Data/SKAnimationData.h"
#include "SKAnimationCardWidget.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKAnimationSelectionWidget::LoadAnimationsForWeapon(const FString& WeaponName)
{
	SK_LOG(LogSkillMaker, Log, TEXT("%s에 대한 애니메이션 로드"), *WeaponName);

	TArray<FSKAnimationRow> AnimList = USKDataManager::Get()->GetAnimationsForWeapon(WeaponName);

	int32 index = 0;
	for(const auto& AnimData : AnimList)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("불러온 애니메이션 : %s"), *AnimData.RowName.ToString());

		CreateAnimationCard(AnimData, index);
		index++;
	}
}

void USKAnimationSelectionWidget::CreateAnimationCard(const FSKAnimationRow& AnimationRow, const int32 AnimIndex)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!AnimationGridPanel || !WBP_SKAnimationCard)
		return;

	if(USKAnimationCardWidget* AnimationCard = CreateWidget<USKAnimationCardWidget>(GetWorld(), WBP_SKAnimationCard))
	{
		UAnimMontage* NowAnimMontage = AnimationRow.Data.AnimationMontage.LoadSynchronous();
		UTexture2D* NowAnimThumbNail = AnimationRow.Data.AnimationThumbnail.LoadSynchronous();
		
		AnimationCard->SetAnimationInfo(NowAnimMontage, NowAnimThumbNail);
		AnimationCard->OnAnimationCardSelected.AddDynamic(this, &USKAnimationSelectionWidget::AnimationSelected);
		AnimationGridPanel->AddChildToUniformGrid(AnimationCard, AnimIndex/2, AnimIndex%2);
	}
}

void USKAnimationSelectionWidget::AnimationSelected(UAnimMontage* SelectedMontage)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!SelectedMontage)
		return;

	SK_LOG(LogSkillMaker, Log, TEXT("선택된 애니메이션 : %s"), *SelectedMontage->GetName());

	OnAnimationSelected.Broadcast(SelectedMontage);
}

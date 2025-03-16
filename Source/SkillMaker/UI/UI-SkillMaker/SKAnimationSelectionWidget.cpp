// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimationSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/SKAnimationData.h"
#include "Data/SKDataManager.h"
#include "SKAnimationCardWidget.h"

void USKAnimationSelectionWidget::LoadAnimationsForWeapon(const FString& WeaponType)
{
	UE_LOG(LogTemp, Log, TEXT("%s에 대한 애니메이션 로드"), *WeaponType);

	TArray<FSKAnimationData> AnimList = USKDataManager::Get()->GetAnimationsForWeapon(WeaponType);

	int32 index = 0;
	for(const auto& AnimData : AnimList)
	{
		UE_LOG(LogTemp, Log, TEXT("불러온 애니메이션 : %s"), *AnimData.AnimationMontage->GetName());

		CreateAnimationCard(AnimData.AnimationMontage, AnimData.AnimationThumbnail, index);
		index++;
	}
}

void USKAnimationSelectionWidget::CreateAnimationCard(UAnimMontage* Montage, UTexture2D* Thumbnail, const int32 AnimIndex)
{
	if(!AnimationGridPanel || !WBP_SKAnimationCard)
		return;

	if(USKAnimationCardWidget* AnimationCard = CreateWidget<USKAnimationCardWidget>(GetWorld(), WBP_SKAnimationCard))
	{
		AnimationCard->SetAnimationInfo(Montage, Thumbnail);
		AnimationCard->OnAnimationCardSelected.AddDynamic(this, &USKAnimationSelectionWidget::AnimationSelected);
		AnimationGridPanel->AddChildToUniformGrid(AnimationCard, AnimIndex/2, AnimIndex%2);
	}
}

void USKAnimationSelectionWidget::AnimationSelected(UAnimMontage* SelectedMontage)
{
	if(!SelectedMontage)
		return;

	UE_LOG(LogTemp, Log, TEXT("선택된 애니메이션 : %s"), *SelectedMontage->GetName());

	OnAnimationSelected.Broadcast(SelectedMontage);
}

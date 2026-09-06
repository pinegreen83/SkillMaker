// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "SKAnimationSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimationSelected, const TSoftObjectPtr<UAnimMontage>&, SelectedMontage);

class UUniformGridPanel;
class USKAnimationCardWidget;
class UAnimMontage;
struct FSKAnimationRow;

UCLASS()
class SKILLMAKER_API USKAnimationSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Animation Selection")
	void LoadAnimationsForWeapon(FGameplayTag WeaponTag, const TSoftObjectPtr<UAnimMontage>& CurrentMontage);
	
	UPROPERTY(BlueprintAssignable, Category = "Animation Selection")
	FOnAnimationSelected OnAnimationSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> AnimationGridPanel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKAnimationCardWidget> WBP_SKAnimationCard;

	void CreateAnimationCard(const FSKAnimationRow& AnimationRow, const int32 AnimIndex, const TSoftObjectPtr<UAnimMontage>& CurrentMontage);

	UFUNCTION()
	void AnimationSelected(const TSoftObjectPtr<UAnimMontage>& SelectedMontage);
};

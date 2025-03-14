// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKAnimationSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimationSelected, UAnimMontage*, SelectedMontage);

class UUniformGridPanel;
class USKAnimationCardWidget;
class UAnimMontage;

UCLASS()
class SKILLMAKER_API USKAnimationSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Animation Selection")
	void LoadAnimationsForWeapon(const FString& WeaponName);
	
	UPROPERTY(BlueprintAssignable, Category = "Animation Selection")
	FOnAnimationSelected OnAnimationSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> AnimationGridPanel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKAnimationCardWidget> WBP_SKAnimationCard;

	void CreateAnimationCard(UAnimMontage* Montage, UTexture2D* Thumbnail, const int32 AnimIndex);

	UFUNCTION()
	void AnimationSelected(UAnimMontage* SelectedMontage);
};

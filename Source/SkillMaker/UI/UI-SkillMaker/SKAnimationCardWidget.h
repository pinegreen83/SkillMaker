// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKAnimationCardWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimationCardSelected, UAnimMontage*, SelectedMontage);

class UButton;
class UImage;
class UTextBlock;
class UAnimMontage;

UCLASS()
class SKILLMAKER_API USKAnimationCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable, Category = "Animation Selection")
	void SetAnimationInfo(UAnimMontage* Montage, UTexture2D* Thumbnail);

	UPROPERTY(BlueprintAssignable, Category = "Animation Selection")
	FOnAnimationCardSelected OnAnimationCardSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AnimationButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AnimationThumbnail;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AnimationNameText;

	UPROPERTY()
	TObjectPtr<UAnimMontage> SelectedMontage;

	UFUNCTION()
	void HandleAnimationSelected();
};

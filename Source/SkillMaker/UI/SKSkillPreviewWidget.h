// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillPreviewWidget.generated.h"

class USlider;
class UButton;
class UTextBlock;
class ASKPreviewCharacter;

UCLASS()
class SKILLMAKER_API USKSkillPreviewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetPreviewCharacter(ASKPreviewCharacter* InPreviewCharacter);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> ScrubSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTimeText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalTimeText;

	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnScrubValueChanged(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Preview Character")
	TObjectPtr<ASKPreviewCharacter> PreviewCharacter;
};

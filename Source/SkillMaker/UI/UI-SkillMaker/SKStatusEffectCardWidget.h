// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKStatusEffectCardWidget.generated.h"

class UCheckBox;
class UEditableTextBox;
class USlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStatusEffectToggled, EStatusEffect, EffectType, bool, bIsChecked, float, Duration, float, DamageOverTime);

UCLASS()
class SKILLMAKER_API USKStatusEffectCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void InitializeEffectEntry(EStatusEffect EffectType);

	UPROPERTY(BlueprintAssignable, Category = "Status Effect")
	FOnStatusEffectToggled OnStatusEffectToggled;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> EffectCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> DurationTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> DOTSlider;

	EStatusEffect CurrentEffectType;

	UFUNCTION()
	void OnEffectToggled(bool bIsChecked);

	UFUNCTION()
	void OnDurationChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnDOTValueChanged(float Value);
};
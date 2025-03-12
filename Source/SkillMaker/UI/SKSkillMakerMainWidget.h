// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillMakerMainWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class USKWeaponSelectionWidget;
class USKAnimationSelectionWidget;
class UEditableTextBox;

UENUM(BlueprintType)
enum class ESKSkillMakerState : uint8
{
	ChooseAction,
	ChooseWeapon,
	ChooseAnimation,
	EditSkill,
	SaveSkill
};

UCLASS()
class SKILLMAKER_API USKSkillMakerMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetSkillMakerState(ESKSkillMakerState NewState);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> SkillMakerSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ModifySkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateSkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKWeaponSelectionWidget> WeaponSelectionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKAnimationSelectionWidget> AnimationSelectionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveSkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> SkillNameInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UFUNCTION()
	void OnModifySkillClicked();

	UFUNCTION()
	void OnCreateSkillClicked();

	FString SelectedWeaponType;
	
	UFUNCTION()
	void OnWeaponSelected(const FString& WeaponName);

	UFUNCTION()
	void OnSaveSkillClicked();

	UFUNCTION()
	void OnBackClicked();

private:
	ESKSkillMakerState CurrentState;
};

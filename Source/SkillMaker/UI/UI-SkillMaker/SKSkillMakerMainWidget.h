// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillMakerMainWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class USKWeaponSelectionWidget;
class USKAnimationSelectionWidget;
class USKSkillSelectionWidget;
class USKSkillDetailWidget;
class UEditableTextBox;
class ASKSkillMakerHUD;

UENUM(BlueprintType)
enum class ESKSkillMakerState : uint8
{
	ChooseAction,
	ChooseSkill,
	ChooseWeapon,
	ChooseAnimation,
	SkillDetail,
	SaveSkill
};

UCLASS()
class SKILLMAKER_API USKSkillMakerMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetHUDReference(ASKSkillMakerHUD* InHUD);
	
	void SetSkillMakerState(ESKSkillMakerState NewState, bool bFromBackNavigation);

	void GoBackToPreviousState();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> SkillMakerSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKSkillSelectionWidget> SkillSelectionWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKWeaponSelectionWidget> WeaponSelectionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKAnimationSelectionWidget> AnimationSelectionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKSkillDetailWidget> SkillDetailWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ModifySkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateSkillButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FinishEditingSkillButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveSkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> SkillNameInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY()
	TObjectPtr<ASKSkillMakerHUD> HUDReference;

	FString SelectedWeaponType;

private:
	UFUNCTION()
	void OnModifySkillClicked();

	UFUNCTION()
	void OnCreateSkillClicked();

	UFUNCTION()
	void OnSkillSelected(const FName& SkillID);
	
	UFUNCTION()
	void OnWeaponSelected(const FString& WeaponName);

	UFUNCTION()
	void OnAnimationSelected(UAnimMontage* AnimationMontage);

	UFUNCTION()
	void OnFinishSkillEditing();
	
	UFUNCTION()
	void OnSaveSkillClicked();

	UFUNCTION()
	void OnBackClicked();

	ESKSkillMakerState CurrentState;
	TArray<ESKSkillMakerState> PreviousStates;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "SKSkillMakerEditorMainWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillDataFromTable);

class UWidgetSwitcher;
class UButton;
class USKWeaponSelectionWidget;
class USKAnimationSelectionWidget;
class USKSkillSelectionWidget;
class USKSkillDetailWidget;
class UEditableTextBox;
class ASKSkillMakerEditorHUD;

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

enum class ESKSkillEditMode : uint8
{
	None,
	Create,
	Modify
};

UCLASS()
class SKILLMAKER_API USKSkillMakerEditorMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetHUDReference(ASKSkillMakerEditorHUD* InHUD);
	
	void SetSkillMakerState(ESKSkillMakerState NewState, bool bFromBackNavigation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FOnSkillDataFromTable OnSkillDataFromTable;
	
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
	TObjectPtr<ASKSkillMakerEditorHUD> HUDReference;

private:
	void GoBackToPreviousState();
	const TCHAR* GetEditModeLogText() const;
	
	UFUNCTION()
	void OnModifySkillClicked();

	UFUNCTION()
	void OnCreateSkillClicked();

	UFUNCTION()
	void OnSkillSelected(const FName& SkillID);
	
	UFUNCTION()
	void OnWeaponSelected(FGameplayTag WeaponTag);

	UFUNCTION()
	void OnAnimationSelected(const TSoftObjectPtr<UAnimMontage>& AnimationMontage);

	UFUNCTION()
	void OnFinishSkillEditing();
	
	UFUNCTION()
	void OnSaveSkillClicked();

	UFUNCTION()
	void OnBackClicked();

	ESKSkillMakerState CurrentState = ESKSkillMakerState::ChooseAction;
	ESKSkillEditMode CurrentEditMode = ESKSkillEditMode::None;
	TArray<ESKSkillMakerState> PreviousStates;
};

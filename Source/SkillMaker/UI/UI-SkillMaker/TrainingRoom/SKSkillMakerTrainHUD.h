// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Skill/SKSkillData.h"
#include "Save/SKPlayerSkillSave.h"
#include "SKSkillMakerTrainHUD.generated.h"

class USKMapNavigationWidget;
class USKSkillSelectionWidget;
class USKSkillSlotAssignmentWidget;
class ASKPlayerCharacter;
struct FSKSkillSet;
struct FSKSkillData;

UCLASS()
class SKILLMAKER_API ASKSkillMakerTrainHUD : public AHUD
{
	GENERATED_BODY()
public:
	ASKSkillMakerTrainHUD();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowSkillSelection();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowSkillAssignmentOverview();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void InitializeNewSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LoadSkillForEditing(const FName& SkillID);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	const FSKSkillData& GetCurrentSkillData() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetCurrentSkillData(FSKSkillData SkillData);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillName(const FString& SkillName);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillMontage(const TSoftObjectPtr<UAnimMontage>& Montage);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LogCurrentSkillData();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USKSkillSelectionWidget> SkillSelectionWidgetClass;

	UPROPERTY()
	TObjectPtr<USKSkillSelectionWidget> SkillSelectionWidget;

	UPROPERTY()
	TObjectPtr<USKSkillSlotAssignmentWidget> SkillSlotAssignmentWidget;

	UPROPERTY()
	TObjectPtr<USKMapNavigationWidget> NavigationWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FSKSkillSet CurrentEditingSkillSet;
	
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FSKSkillData CurrentEditingSkill;

	UPROPERTY()
	TObjectPtr<ASKPlayerCharacter> PlayerCharacter;

private:
	UFUNCTION()
	void OnSkillSelected(const FName& SkillID);

	UFUNCTION()
	void OnSkillSlotSelected(int32 SlotIndex);

	UFUNCTION()
	void OnSkillSlotAssignmentCancelled();

	void ShowSkillSlotAssignment();
	void RefreshSkillAssignmentOverview();
	void CloseSkillAssignmentUI();
};

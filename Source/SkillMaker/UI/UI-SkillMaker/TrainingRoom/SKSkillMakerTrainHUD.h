// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Skill/SKSkillData.h"
#include "Save/SKPlayerSkillSave.h"
#include "SKSkillMakerTrainHUD.generated.h"

class USKSkillMakerTrainMainWidget;
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
	void SetSkillMontage(UAnimMontage* Montage);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LogCurrentSkillData();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USKSkillMakerTrainMainWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<USKSkillMakerTrainMainWidget> MainWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FSKSkillSet CurrentEditingSkillSet;
	
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FSKSkillData CurrentEditingSkill;

	UPROPERTY()
	TObjectPtr<ASKPlayerCharacter> PreviewCharacter;
};

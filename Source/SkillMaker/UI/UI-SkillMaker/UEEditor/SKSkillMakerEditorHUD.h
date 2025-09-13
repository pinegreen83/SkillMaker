// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Skill/SKSkillData.h"
#include "SKSkillMakerEditorHUD.generated.h"

class USKSkillMakerEditorMainWidget;
class ASKPreviewCharacter;

UCLASS()
class SKILLMAKER_API ASKSkillMakerEditorHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASKSkillMakerEditorHUD();
	
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
	void SetSkillName(const FString& WeaponName);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillWeaponType(const FString& WeaponType);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillMontage(UAnimMontage* AnimationMontage);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LogCurrentSkillData();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void PreviewSkillEffect(const FSKSkillData& SkillData);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USKSkillMakerEditorMainWidget> MainWidgetClass;

	UPROPERTY()
	TObjectPtr<USKSkillMakerEditorMainWidget> MainWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FSKSkillData CurrentEditingSkill;

	UPROPERTY()
	TObjectPtr<ASKPreviewCharacter> PreviewCharacter;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Skill/SKSkillData.h"
#include "SKSkillMakerHUD.generated.h"

class USKSkillMakerMainWidget;

UCLASS()
class SKILLMAKER_API ASKSkillMakerHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASKSkillMakerHUD();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void InitializeNewSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LoadSkillForEditing(const FName& SkillID);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	const FSKSkillData& GetCurrentSkillData() const;
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillName(const FString& WeaponName);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillWeaponType(const FString& WeaponType);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillMontage(UAnimMontage* AnimationMontage);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LogCurrentSkillData();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USKSkillMakerMainWidget> MainWidgetClass;

	UPROPERTY()
	TObjectPtr<USKSkillMakerMainWidget> MainWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FSKSkillData CurrentEditingSkill;
};

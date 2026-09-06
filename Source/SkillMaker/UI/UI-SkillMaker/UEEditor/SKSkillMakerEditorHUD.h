// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Skill/SKSkillData.h"
#include "SKSkillMakerEditorHUD.generated.h"

class USKSkillMakerEditorMainWidget;
class ASKPreviewCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditingSkillChanged, const FSKSkillData&, SkillData);

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
	void SetCurrentSkillData(const FSKSkillData& SkillData);

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnEditingSkillChanged OnEditingSkillChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillName(const FString& SkillName);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillWeaponTag(FGameplayTag WeaponTag);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillMontage(const TSoftObjectPtr<UAnimMontage>& AnimationMontage);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool SaveCurrentSkill(const FString& SkillName);
	
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

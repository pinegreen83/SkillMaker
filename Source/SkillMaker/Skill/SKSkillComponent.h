// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SKSkillData.h"
#include "SKSkillComponent.generated.h"

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLMAKER_API USKSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USKSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseSkill(const FString& SkillName);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool IsSkillAvailable(const FString& SkillName) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<FString> ActiveSkills;

private:
	void HandleSkillExecution(const FSKSkillData& SkillData);

	void ApplyCooldown(const FString& SkillName, float CooldownTime);

	UPROPERTY()
	TMap<FString, float> SkillCooldowns;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SKBaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSKCharacter, Log, All);

UCLASS()
class SKILLMAKER_API ASKBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASKBaseCharacter();

	// 임시로 에디터에서 할당
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<class USKSkillData> DefaultSkillData;

	UFUNCTION(BlueprintCallable)
	void UseSkillAsset(USKSkillData* SkillDataAsset);

protected:
	virtual void BeginPlay() override;
};

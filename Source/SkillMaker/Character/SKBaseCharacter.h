// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Skill/SKSkillComponent.h"
#include "SKBaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSKCharacter, Log, All);

UCLASS()
class SKILLMAKER_API ASKBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASKBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<USKSkillComponent> SkillComponent;
	
};

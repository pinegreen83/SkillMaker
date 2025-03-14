// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SKSkillManager.generated.h"

struct SKSkillData;

/**
 * 
 */
UCLASS()
class SKILLMAKER_API USKSkillManager : public UObject
{
	GENERATED_BODY()

public:
	static USKSkillManager* Get();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> SkillDataTable;

	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FSKSkillData> GetSkillList();

	UFUNCTION(BlueprintCallable, Category = "Data")
	TOptional<FSKSkillData> GetSkillDataByName(const FString& SkillName);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SaveSkillData(const FSKSkillData& SkillData);
	
protected:
	static USKSkillManager* Instance;
	
};

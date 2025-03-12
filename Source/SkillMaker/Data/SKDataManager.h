// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SKDataManager.generated.h"

struct SKWeaponData;
struct SKAnimationData;

UCLASS()
class SKILLMAKER_API USKDataManager : public UObject
{
	GENERATED_BODY()

public:
	static USKDataManager* Get();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> AnimationDataTable;

	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FSKWeaponData> GetWeaponList();

	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FSKAnimationData> GetAnimationsForWeapon(const FString& WeaponType);

protected:
	static USKDataManager* Instance;
};

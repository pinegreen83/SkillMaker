// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/SKAnimationData.h"
#include "Data/SKWeaponData.h"
#include "Data/SKProjectileData.h"
#include "Engine/DataTable.h"
#include "SKDataManagerSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSKWeaponRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintREadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSKWeaponData Data;

	FSKWeaponRow() {}

	FSKWeaponRow(FName InName, const FSKWeaponData& InData)
		: RowName(InName), Data(InData)
	{}
};

USTRUCT(BlueprintType)
struct FSKAnimationRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintREadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSKAnimationData Data;

	FSKAnimationRow() {}

	FSKAnimationRow(FName InName, const FSKAnimationData& InData)
		: RowName(InName), Data(InData)
	{}
};

USTRUCT(BlueprintType)
struct FSKProjectileRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintREadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSKProjectileData Data;

	FSKProjectileRow() {}

	FSKProjectileRow(FName InName, const FSKProjectileData& InData)
		: RowName(InName), Data(InData)
	{}
};

UCLASS()
class SKILLMAKER_API USKDataManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FSKWeaponRow> GetWeaponList();

	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FSKAnimationRow> GetAnimationsForWeapon(const FString& WeaponType);

	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FSKProjectileRow> GetProjectileList();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> AnimationDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> ProjectileDataTable;
};

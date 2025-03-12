// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SKAnimationManager.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class SKILLMAKER_API USKAnimationManager : public UObject
{
	GENERATED_BODY()

public:
	static TArray<UAnimMontage*> GetMontagesFromFolder(const FString& FolderPath);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimationManager.h"
#include "Engine/AssetManager.h"
#include "Animation/AnimMontage.h"

TArray<UAnimMontage*> USKAnimationManager::GetMontagesFromFolder(const FString& FolderPath)
{
	TArray<UAnimMontage*> Animations;
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FARFilter Filter;
	Filter.PackagePaths.Add(*FolderPath);
	Filter.ClassPaths.Add(UAnimSequence::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;

	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	for(const FAssetData& Data : AssetData)
	{
		if(UAnimMontage* Animation = Cast<UAnimMontage>(Data.GetAsset()))
		{
			Animations.Add(Animation);
		}
	}

	return Animations;
}

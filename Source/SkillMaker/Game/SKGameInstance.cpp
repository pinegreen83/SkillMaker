// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SKGameInstance.h"
#include "Data/SKDataManager.h"

void USKGameInstance::Init()
{
	Super::Init();

	USKDataManager::Get();
}

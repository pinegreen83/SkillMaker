// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SKGameMode.h"

ASKGameMode::ASKGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/BP_SKPreviewCharacter.BP_SKPreviewCharacter_C'"));
	if(DefaultPawnClassRef.Class != nullptr)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/SkillMaker.SKPlayerController"));
	if(PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void ASKGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("SKGameMode : BeginPlay start"));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SKSkillMakerController.h"

void ASKSkillMakerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	SetInputMode(InputMode);
}

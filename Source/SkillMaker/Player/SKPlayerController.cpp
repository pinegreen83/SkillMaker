// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SKPlayerController.h"

void ASKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

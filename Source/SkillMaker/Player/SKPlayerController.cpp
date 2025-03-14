// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/SKPlayerCharacter.h"

void ASKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if(DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			UE_LOG(LogTemp, Log, TEXT("DefaultMappingContext 설정 완료."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext가 설정되지 않음."));
		}
	}	
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void ASKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Moving
		if(MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASKPlayerController::Move);
		}
		
		// Looking
		if(LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASKPlayerController::Look);
		}

		// Jumping
		if(JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASKPlayerController::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASKPlayerController::StopJumping);
		}

		// Skill
		if(SkillAction)
		{
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &ASKPlayerController::UseSkill);
		}
	}
}

void ASKPlayerController::Move(const FInputActionValue& Value)
{
	if(ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Move(Value);
	}
}

void ASKPlayerController::Look(const FInputActionValue& Value)
{
	if(ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Look(Value);
	}
}

void ASKPlayerController::Jump()
{
	if(ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Jump();
	}
}

void ASKPlayerController::StopJumping()
{
	if(ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->StopJumping();
	}
}

void ASKPlayerController::UseSkill()
{
	if(ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->UseSkillAction();
	}
}

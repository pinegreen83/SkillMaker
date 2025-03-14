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
	
	// 기본 스킬 슬롯 초기화
	SkillSlots.SetNum(4);
	SkillSlots[0] = TEXT("Test1");  // 1번 스킬
	SkillSlots[1] = TEXT("Test2");  // 2번 스킬
	SkillSlots[2] = TEXT("Test3");  // 3번 스킬
	SkillSlots[3] = TEXT("Test4");  // 4번 스킬

	// 키 매핑
	KeyToSkillSlotMap.Add(EKeys::Q, 0);
	KeyToSkillSlotMap.Add(EKeys::E, 1);
	KeyToSkillSlotMap.Add(EKeys::R, 2);
	KeyToSkillSlotMap.Add(EKeys::F, 3);
	
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
		// if(SkillAction)
		// {
		// 	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &ASKPlayerController::UseSkill);
		// }
		
		for (const TPair<FKey, int32>& KeyMapping : KeyToSkillSlotMap)
		{
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &ASKPlayerController::UseSkillByKey, KeyMapping.Key);
		}
	}
}

void ASKPlayerController::UseSkillByKey(FKey PressedKey)
{
	if (KeyToSkillSlotMap.Contains(PressedKey))
	{
		int32 SlotIndex = KeyToSkillSlotMap[PressedKey];
		
		if (SlotIndex >= 0 && SkillSlots.IsValidIndex(SlotIndex))
		{
			FName SkillID = GetSkillInSlot(SlotIndex);
			if (ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
			{
				PlayerCharacter->UseSkill(SkillID);
				UE_LOG(LogTemp, Log, TEXT("스킬 사용 : %s (키 %s, 슬롯 %d)"), *SkillID.ToString(), *PressedKey.ToString(), SlotIndex);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("스킬 슬롯이 비어 있음 : 키 %s"), *PressedKey.ToString());
		}
	}
}

void ASKPlayerController::SetSkillInSlot(int32 SlotIndex, FName SkillID)
{
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		SkillSlots[SlotIndex] = SkillID;
		UE_LOG(LogTemp, Log, TEXT("스킬 변경 : 슬롯 %d -> %s"), SlotIndex, *SkillID.ToString());
	}
}

FName ASKPlayerController::GetSkillInSlot(int32 SlotIndex) const
{
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		return SkillSlots[SlotIndex];
	}
	return NAME_None;
}

void ASKPlayerController::SetKeyForSkillSlot(FKey NewKey, int32 SlotIndex)
{
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		// 기존 키 제거 후 새로운 키 등록
		for (TPair<FKey, int32>& Pair : KeyToSkillSlotMap)
		{
			if (Pair.Value == SlotIndex)
			{
				Pair.Value = -1;
				break;
			}
		}

		KeyToSkillSlotMap.Add(NewKey, SlotIndex);
		UE_LOG(LogTemp, Log, TEXT("키 변경 : %s -> 슬롯 %d"), *NewKey.ToString(), SlotIndex);
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
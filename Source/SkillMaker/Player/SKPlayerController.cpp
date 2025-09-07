// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/SKPlayerCharacter.h"
#include "Logging/SKLogSkillMakerMacro.h"

void ASKPlayerController::BeginPlay()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	Super::BeginPlay();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if(DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			SK_LOG(LogSkillMaker, Log, TEXT("DefaultMappingContext 설정 완료."));
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("DefaultMappingContext가 설정되지 않음."));
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
		if (KeyToSkillSlotMap.Num() == 0)
		{
			SkillInputSetup();
		}
		
		SK_LOG(LogSkillMaker, Log, TEXT("KeyToSkillSlotMap Size : %d"), KeyToSkillSlotMap.Num());
		
		for (const TPair<FKey, TObjectPtr<UInputAction>>& KeyMapping : KeyToSkillSlotMap)
		{
			EnhancedInputComponent->BindAction(KeyMapping.Value, ETriggerEvent::Triggered, this, &ASKPlayerController::UseSkillByKey, KeyMapping.Key);
		}
	}
}

void ASKPlayerController::UseSkillByKey(FKey PressedKey)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (KeyToSkillSlotMap.Contains(PressedKey))
	{
		int32 SlotIndex = KeyToSkillIndexMap[PressedKey];
		
		if (SlotIndex >= 0 && SkillSlots.IsValidIndex(SlotIndex))
		{
			FName SkillID = GetSkillInSlot(SlotIndex);
			if (ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
			{
				SK_LOG(LogSkillMaker, Log, TEXT("스킬 사용 : %s (키 %s, 슬롯 %d)"), *SkillID.ToString(), *PressedKey.ToString(), SlotIndex);
				PlayerCharacter->UseSkill(SkillID);
			}
		}
		else
		{
			SK_LOG(LogSkillMaker, Warning, TEXT("스킬 슬롯이 비어 있음 : 키 %s"), *PressedKey.ToString());
		}
	}
}

void ASKPlayerController::SetSkillInSlot(int32 SlotIndex, FName SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		SkillSlots[SlotIndex] = SkillID;
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 변경 : 슬롯 %d -> %s"), SlotIndex, *SkillID.ToString());
	}
}

FName ASKPlayerController::GetSkillInSlot(int32 SlotIndex) const
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		return SkillSlots[SlotIndex];
	}
	return NAME_None;
}

void ASKPlayerController::SetKeyForSkillSlot(FKey NewKey, int32 SlotIndex)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		// 기존 키 제거 후 새로운 키 등록
		for (TPair<FKey, int32>& Pair : KeyToSkillIndexMap)
		{
			if (Pair.Value == SlotIndex)
			{
				Pair.Value = -1;
				break;
			}
		}

		KeyToSkillIndexMap.Add(NewKey, SlotIndex);
		UE_LOG(LogTemp, Log, TEXT("키 변경 : %s -> 슬롯 %d"), *NewKey.ToString(), SlotIndex);
	}
}

void ASKPlayerController::SkillInputSetup()
{
	// 기본 스킬 슬롯 초기화
	SkillSlots.SetNum(4);
	SkillSlots[0] = TEXT("Test1");  // 1번 스킬
	SkillSlots[1] = TEXT("Test2");  // 2번 스킬
	SkillSlots[2] = TEXT("Test3");  // 3번 스킬
	SkillSlots[3] = TEXT("Test4");  // 4번 스킬

	// 키 세팅
	KeyToSkillSlotMap.Add(EKeys::Q, SkillActionQ);
	KeyToSkillSlotMap.Add(EKeys::E, SkillActionE);
	KeyToSkillSlotMap.Add(EKeys::R, SkillActionR);
	KeyToSkillSlotMap.Add(EKeys::F, SkillActionF);
	
	// 스킬 세팅
	KeyToSkillIndexMap.Add(EKeys::Q, 0);
	KeyToSkillIndexMap.Add(EKeys::E, 1);
	KeyToSkillIndexMap.Add(EKeys::R, 2);
	KeyToSkillIndexMap.Add(EKeys::F, 3);
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
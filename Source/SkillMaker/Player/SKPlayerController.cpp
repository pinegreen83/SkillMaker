// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character/SKPlayerCharacter.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Prop/SKInteractableActor.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h"

ASKPlayerController::ASKPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> OpenSkillChangeUIActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkillMaker/Input/Actions/IA_OpenSkillChangeUI.IA_OpenSkillChangeUI'"));
	if (OpenSkillChangeUIActionFinder.Succeeded())
	{
		OpenSkillChangeUIAction = OpenSkillChangeUIActionFinder.Object;
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 변경 UI InputAction 로드 완료: %s"),
			*OpenSkillChangeUIAction->GetPathName());
	}
	else
	{
		SK_LOG(LogSkillMaker, Error,
			TEXT("스킬 변경 UI InputAction 로드 실패: /Game/SkillMaker/Input/Actions/IA_OpenSkillChangeUI"));
	}
}

void ASKPlayerController::BeginPlay()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	Super::BeginPlay();
	SkillInputSetup();

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

		InstallRuntimeSkillInputMapping(Subsystem);
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
		SkillInputSetup();
		
		SK_LOG(LogSkillMaker, Log, TEXT("KeyToSkillSlotMap Size : %d"), KeyToSkillSlotMap.Num());
		
		for (const TPair<FKey, TObjectPtr<UInputAction>>& KeyMapping : KeyToSkillSlotMap)
		{
			if (KeyMapping.Value)
			{
				EnhancedInputComponent->BindAction(
					KeyMapping.Value, ETriggerEvent::Started, this, &ASKPlayerController::UseSkillByKey, KeyMapping.Key);
				SK_LOG(LogSkillMaker, Log, TEXT("스킬 InputAction 바인딩 완료: Key=%s, Action=%s, Trigger=Started"),
					*KeyMapping.Key.ToString(), *KeyMapping.Value->GetName());
			}
			else
			{
				SK_LOG(LogSkillMaker, Error, TEXT("스킬 InputAction 바인딩 실패: Key=%s, Action=None"),
					*KeyMapping.Key.ToString());
			}
		}

		if (OpenSkillChangeUIAction)
		{
			EnhancedInputComponent->BindAction(
				OpenSkillChangeUIAction, ETriggerEvent::Started, this, &ASKPlayerController::OpenSkillChangeUI);
			SK_LOG(LogSkillMaker, Log, TEXT("스킬 변경 UI InputAction 바인딩 완료: Key=Zero, Action=%s, Trigger=Started"),
				*OpenSkillChangeUIAction->GetName());
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("스킬 변경 UI InputAction 바인딩 실패: Action=None"));
		}

		// Interact
		if(InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASKPlayerController::TryInteract);
		}
	}
}

void ASKPlayerController::UseSkillByKey(FKey PressedKey)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (const int32* SlotIndex = KeyToSkillIndexMap.Find(PressedKey))
	{
		if (*SlotIndex >= 0 && SkillSlots.IsValidIndex(*SlotIndex))
		{
			const FName SkillID = GetSkillInSlot(*SlotIndex);
			if (SkillID.IsNone())
			{
				SK_LOG(LogSkillMaker, Warning, TEXT("스킬 슬롯이 비어 있음 : 키 %s, 슬롯 %d"),
					*PressedKey.ToString(), *SlotIndex);
				return;
			}

			if (ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(GetPawn()))
			{
				SK_LOG(LogSkillMaker, Log, TEXT("스킬 사용 : %s (키 %s, 슬롯 %d)"),
					*SkillID.ToString(), *PressedKey.ToString(), *SlotIndex);
				PlayerCharacter->UseSkill(SkillID);
			}
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("키의 슬롯 인덱스가 잘못됨 : 키 %s, 슬롯 %d"),
				*PressedKey.ToString(), *SlotIndex);
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("등록되지 않은 스킬 키 입력 : %s"), *PressedKey.ToString());
	}
}

void ASKPlayerController::SetSkillInSlot(const int32& SlotIndex, const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		SkillSlots[SlotIndex] = SkillID;
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 변경 : 슬롯 %d -> %s"), SlotIndex, *SkillID.ToString());
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 슬롯 저장 실패: SlotIndex=%d, SlotCount=%d, SkillID=%s"),
			SlotIndex, SkillSlots.Num(), *SkillID.ToString());
	}
}

FName ASKPlayerController::GetSkillInSlot(const int32& SlotIndex) const
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		return SkillSlots[SlotIndex];
	}
	return NAME_None;
}

void ASKPlayerController::SetKeyForSkillSlot(const FKey& NewKey, const int32& SlotIndex)
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
	EnsureRuntimeSkillInputObjects();

	// 기본 스킬 슬롯 초기화
	SkillSlots.SetNum(4); // 4번 스킬
	KeyToSkillSlotMap.Reset();
	KeyToSkillIndexMap.Reset();

	// 키 세팅
	KeyToSkillSlotMap.Add(EKeys::Q, RuntimeSkillActionQ);
	KeyToSkillSlotMap.Add(EKeys::E, RuntimeSkillActionE);
	KeyToSkillSlotMap.Add(EKeys::R, RuntimeSkillActionR);
	KeyToSkillSlotMap.Add(EKeys::F, RuntimeSkillActionF);
	
	// 스킬 세팅
	KeyToSkillIndexMap.Add(EKeys::Q, 0);
	KeyToSkillIndexMap.Add(EKeys::E, 1);
	KeyToSkillIndexMap.Add(EKeys::R, 2);
	KeyToSkillIndexMap.Add(EKeys::F, 3);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 슬롯 테이블 초기화 완료: SlotCount=%d, ActionCount=%d, KeyIndexCount=%d"),
		SkillSlots.Num(), KeyToSkillSlotMap.Num(), KeyToSkillIndexMap.Num());
}

void ASKPlayerController::EnsureRuntimeSkillInputObjects()
{
	auto CreateSkillAction = [this](TObjectPtr<UInputAction>& SkillAction, const FName& ObjectName)
	{
		if (!SkillAction)
		{
			SkillAction = NewObject<UInputAction>(this, ObjectName);
			SkillAction->ValueType = EInputActionValueType::Boolean;
		}
	};

	CreateSkillAction(RuntimeSkillActionQ, TEXT("IA_RuntimeSkillQ"));
	CreateSkillAction(RuntimeSkillActionE, TEXT("IA_RuntimeSkillE"));
	CreateSkillAction(RuntimeSkillActionR, TEXT("IA_RuntimeSkillR"));
	CreateSkillAction(RuntimeSkillActionF, TEXT("IA_RuntimeSkillF"));

	if (!RuntimeSkillMappingContext)
	{
		RuntimeSkillMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_RuntimeSkillSlots"));
		RuntimeSkillMappingContext->MapKey(RuntimeSkillActionQ, EKeys::Q);
		RuntimeSkillMappingContext->MapKey(RuntimeSkillActionE, EKeys::E);
		RuntimeSkillMappingContext->MapKey(RuntimeSkillActionR, EKeys::R);
		RuntimeSkillMappingContext->MapKey(RuntimeSkillActionF, EKeys::F);
		SK_LOG(LogSkillMaker, Log, TEXT("런타임 스킬 입력 객체 생성 완료: Context=%s"),
			*RuntimeSkillMappingContext->GetName());
	}
}

void ASKPlayerController::InstallRuntimeSkillInputMapping(UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
{
	if (!InputSubsystem)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 입력 매핑 실패: Enhanced Input 서브시스템 없음."));
		return;
	}

	EnsureRuntimeSkillInputObjects();
	InputSubsystem->AddMappingContext(RuntimeSkillMappingContext, 1);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 입력 매핑 완료: Q=슬롯0, E=슬롯1, R=슬롯2, F=슬롯3"));
}

void ASKPlayerController::OpenSkillChangeUI()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 변경 UI 입력 수신: Key=Zero, HUD=%s, MouseCursorBefore=%s"),
		GetHUD() ? *GetHUD()->GetName() : TEXT("None"), bShowMouseCursor ? TEXT("True") : TEXT("False"));

	ASKSkillMakerTrainHUD* TrainHUD = Cast<ASKSkillMakerTrainHUD>(GetHUD());
	if (!TrainHUD)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 변경 UI 열기 실패: ASKSkillMakerTrainHUD를 찾을 수 없음."));
		return;
	}

	TrainHUD->ShowSkillAssignmentOverview();
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 변경 UI 열기 요청 완료: MouseCursorAfter=%s"),
		bShowMouseCursor ? TEXT("True") : TEXT("False"));
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

void ASKPlayerController::TryInteract()
{
	FVector StartLocation = GetPawn()->GetActorLocation();
	FVector EndLocation = StartLocation + GetPawn()->GetActorForwardVector() * 200.f;

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	if(GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		if(ASKInteractableActor* InteractableActor = Cast<ASKInteractableActor>(Hit.GetActor()))
		{
			InteractableActor->OnInteract();
		}
	}
}

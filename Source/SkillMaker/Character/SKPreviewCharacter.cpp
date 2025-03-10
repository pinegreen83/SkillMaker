// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPreviewCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASKPreviewCharacter::ASKPreviewCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/SkillMaker/Input/IMC_Default.IMC_Default'"));
	if(InputMappingContext.Object)
	{
		DefaultMappingContext = InputMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJump(TEXT("/Script/EnhancedInput.InputAction'/Game/SkillMaker/Input/Actions/IA_Jump.IA_Jump'"));
	if(InputActionJump.Object)
	{
		JumpAction = InputActionJump.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMove(TEXT("/Script/EnhancedInput.InputAction'/Game/SkillMaker/Input/Actions/IA_Move.IA_Move'"));
	if(InputActionMove.Object)
	{
		MoveAction = InputActionMove.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLook(TEXT("/Script/EnhancedInput.InputAction'/Game/SkillMaker/Input/Actions/IA_Look.IA_Look'"));
	if(InputActionLook.Object)
	{
		LookAction = InputActionLook.Object;
	}
}

void ASKPreviewCharacter::PlayPreviewAnimation(UAnimMontage* Montage)
{
	if(!Montage || !GetMesh() || !GetMesh()->GetAnimInstance()) return;

	PreviewMontage = Montage;
	GetMesh()->GetAnimInstance()->Montage_Play(PreviewMontage, 1.0f);
}

void ASKPreviewCharacter::SetPreviewAnimationTime(float Time)
{
	if(!PreviewMontage || !GetMesh() || !GetMesh()->GetAnimInstance()) return;

	GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Default"), PreviewMontage);
	GetMesh()->GetAnimInstance()->Montage_SetPosition(PreviewMontage, Time);
}

float ASKPreviewCharacter::GetPreviewAnimationTime() const
{
	if(!PreviewMontage || !GetMesh() || !GetMesh()->GetAnimInstance()) return -1.0f;

	return GetMesh()->GetAnimInstance()->Montage_GetPosition(PreviewMontage);
}

UAnimMontage* ASKPreviewCharacter::GetPreviewMontage() const
{
	return PreviewMontage;
}

void ASKPreviewCharacter::SetPreviewMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		PreviewMontage = Montage;
	}
}

void ASKPreviewCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if(!PlayerController || !DefaultMappingContext) return;
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASKPreviewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASKPreviewCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASKPreviewCharacter::Look);
	}
}

void ASKPreviewCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASKPreviewCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
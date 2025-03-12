// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPreviewCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"

ASKPreviewCharacter::ASKPreviewCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PreviewMontage = nullptr;
}

void ASKPreviewCharacter::PlayPreviewAnimation(UAnimMontage* Montage)
{
	if(!Montage || !GetMesh() || !GetMesh()->GetAnimInstance())
		return;

	SetPreviewMontage(Montage);
	GetMesh()->GetAnimInstance()->Montage_Play(PreviewMontage, 1.0f);
}

void ASKPreviewCharacter::SetPreviewAnimationTime(float Time)
{
	if(!PreviewMontage || !GetMesh() || !GetMesh()->GetAnimInstance())
		return;

	GetMesh()->GetAnimInstance()->Montage_SetPosition(PreviewMontage, Time);
}

float ASKPreviewCharacter::GetPreviewAnimationTime() const
{
	if(!PreviewMontage || !GetMesh() || !GetMesh()->GetAnimInstance())
		return -1.0f;

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
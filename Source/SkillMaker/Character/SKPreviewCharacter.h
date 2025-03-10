// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/SKBaseCharacter.h"
#include "SKPreviewCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class SKILLMAKER_API ASKPreviewCharacter : public ASKBaseCharacter
{
	GENERATED_BODY()

public:
	ASKPreviewCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "Skill Preview")
	void PlayPreviewAnimation(UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable, Category = "Skill Preview")
	void SetPreviewAnimationTime(float Time);

	UFUNCTION(BlueprintCallable, Category = "Skill Preview")
	float GetPreviewAnimationTime() const;

	UFUNCTION(BlueprintCallable, Category = "Skill Preview")
	UAnimMontage* GetPreviewMontage() const;

	UFUNCTION(BlueprintCallable, Category = "Skill Preview")
	void SetPreviewMontage(UAnimMontage* Montage);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Preview")
	TObjectPtr<UAnimMontage> PreviewMontage;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	/*
	 * 언리얼 엔진의 Input System에서 입력 액션과 선언한 Move, Look 함수를 서로 매핑시켜주는 역할을 함.
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
};
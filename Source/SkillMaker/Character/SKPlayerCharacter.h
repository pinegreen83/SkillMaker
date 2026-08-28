// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SKBaseCharacter.h"
#include "SKPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class SKILLMAKER_API ASKPlayerCharacter : public ASKBaseCharacter
{
	GENERATED_BODY()

public:
	ASKPlayerCharacter();

	virtual void BeginPlay() override;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	FString WeaponName;
	
public:
	/** 이동 */
	void Move(const FInputActionValue& Value);

	/** 카메라 회전 */
	void Look(const FInputActionValue& Value);

	void SetInteractableTarget(AActor* Target);

	void ClearInteractableTarget(AActor* Target);
	
	/** 스킬 사용 */
	virtual void UseSkill(const FName& SkillID) override;
};
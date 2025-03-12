// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKBaseCharacter.h"
#include "SKPreviewCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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
};
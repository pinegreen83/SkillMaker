// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "SKPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASKPlayerCharacter;

UCLASS()
class SKILLMAKER_API ASKPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	/** Jumping Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** 스킬 사용 액션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionQ;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionE;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionR;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionF;

	/** 캐릭터의 스킬 슬롯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> SkillSlots;

	/** 키-스킬 슬롯 매핑 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TMap<FKey, TObjectPtr<UInputAction>> KeyToSkillSlotMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TMap<FKey, int32> KeyToSkillIndexMap;

public:
	/** 특정 슬롯의 스킬을 변경하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillInSlot(int32 SlotIndex, FName SkillID);

	/** 특정 슬롯의 스킬을 가져오는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	FName GetSkillInSlot(int32 SlotIndex) const;

	/** 키를 특정 스킬 슬롯에 매핑 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetKeyForSkillSlot(FKey NewKey, int32 SlotIndex);

protected:
	/** 스킬 입력 세팅 */
	void SkillInputSetup();
	
	/** 캐릭터 이동 */
	void Move(const FInputActionValue& Value);

	/** 시점 이동 */
	void Look(const FInputActionValue& Value);

	void Jump();
	void StopJumping();
	
	/** 특정 키로 스킬 사용 */
	void UseSkillByKey(FKey PressedKey);
};
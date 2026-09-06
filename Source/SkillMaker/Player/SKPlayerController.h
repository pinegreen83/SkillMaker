// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "SKPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class ASKPlayerCharacter;

UCLASS()
class SKILLMAKER_API ASKPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASKPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
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

	/** Interact Props*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	/** 스킬 사용 액션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionQ;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionE;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionR;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillActionF;

	/** 숫자 0으로 스킬 변경 UI를 여는 액션 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OpenSkillChangeUIAction;

	/** 캐릭터의 스킬 슬롯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> SkillSlots;

	/** 키-스킬 슬롯 매핑 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TMap<FKey, TObjectPtr<UInputAction>> KeyToSkillSlotMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TMap<FKey, int32> KeyToSkillIndexMap;

	/** C++에서 Q/E/R/F 매핑을 보장하는 런타임 입력 객체 */
	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> RuntimeSkillMappingContext;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> RuntimeSkillActionQ;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> RuntimeSkillActionE;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> RuntimeSkillActionR;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> RuntimeSkillActionF;

public:
	/** 특정 슬롯의 스킬을 변경하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillInSlot(const int32& SlotIndex, const FName& SkillID);

	/** 특정 슬롯의 스킬을 가져오는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	FName GetSkillInSlot(const int32& SlotIndex) const;

	/** 키를 특정 스킬 슬롯에 매핑 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetKeyForSkillSlot(const FKey& NewKey, const int32& SlotIndex);

protected:
	/** 스킬 입력 세팅 */
	void SkillInputSetup();
	void EnsureRuntimeSkillInputObjects();
	void InstallRuntimeSkillInputMapping(UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
	
	/** 캐릭터 이동 */
	void Move(const FInputActionValue& Value);

	/** 시점 이동 */
	void Look(const FInputActionValue& Value);

	void Jump();
	void StopJumping();

	void TryInteract();
	
	/** 특정 키로 스킬 사용 */
	void UseSkillByKey(FKey PressedKey);

	/** 저장 스킬 선택·슬롯 변경 UI 열기 */
	void OpenSkillChangeUI();
};

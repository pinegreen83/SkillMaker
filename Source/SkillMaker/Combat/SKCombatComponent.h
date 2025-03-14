// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/SKSkillData.h"
#include "SKCombatComponent.generated.h"

class ASKBaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLMAKER_API USKCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USKCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** 체력 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float CurrentHealth;

	/** 피해 처리 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float DamageAmount);

	/** 상태 이상 적용 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyStatusEffect(EStatusEffect EffectType, float Duration);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	/** 서버에서 피해 처리 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTakeDamage(float DamageAmount);

	bool ServerTakeDamage_Validate(float DamageAmount);
	void ServerTakeDamage_Implementation(float DamageAmount);

	/** 서버에서 상태 이상 적용 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyStatusEffect(EStatusEffect EffectType, float Duration);

	bool ServerApplyStatusEffect_Validate(EStatusEffect EffectType, float Duration);
	void ServerApplyStatusEffect_Implementation(EStatusEffect EffectType, float Duration);

	/** 클라이언트들에게 상태 이상 적용을 동기화 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastApplyStatusEffect(EStatusEffect EffectType, float Duration);

	void MulticastApplyStatusEffect_Implementation(EStatusEffect EffectType, float Duration);

	/** 사망 처리 */
	void HandleDeath();
	
	/** 상태 이상 관리용 타이머 */
	TMap<EStatusEffect, FTimerHandle> StatusEffectTimers;
	
	UPROPERTY()
	TObjectPtr<ASKBaseCharacter> OwnerCharacter;
};

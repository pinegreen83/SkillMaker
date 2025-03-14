// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/SKCombatComponent.h"
#include "Character/SKBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

USKCombatComponent::USKCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	OwnerCharacter = nullptr;
}

// Called when the game starts
void USKCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ASKBaseCharacter>(GetOwner());
}

void USKCombatComponent::TakeDamage(float DamageAmount)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerTakeDamage(DamageAmount);
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 체력 감소 -> 현재 체력 : %f"), CurrentHealth);

	if(CurrentHealth <= 0.0f)
	{
		HandleDeath();
		UE_LOG(LogTemp, Log, TEXT("캐릭터 사망 : %s"), *OwnerCharacter->GetName());
	}
}

bool USKCombatComponent::ServerTakeDamage_Validate(float DamageAmount)
{
	return DamageAmount > 0;
}

void USKCombatComponent::ServerTakeDamage_Implementation(float DamageAmount)
{
	TakeDamage(DamageAmount);
}

void USKCombatComponent::ApplyStatusEffect(EStatusEffect EffectType, float Duration)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerApplyStatusEffect(EffectType, Duration);
		return;
	}

	MulticastApplyStatusEffect(EffectType, Duration);
}

bool USKCombatComponent::ServerApplyStatusEffect_Validate(EStatusEffect EffectType, float Duration)
{
	return Duration > 0.0f;
}

void USKCombatComponent::ServerApplyStatusEffect_Implementation(EStatusEffect EffectType, float Duration)
{
	MulticastApplyStatusEffect(EffectType, Duration);
}

void USKCombatComponent::MulticastApplyStatusEffect_Implementation(EStatusEffect EffectType, float Duration)
{
	if(!OwnerCharacter)
		return;

	switch(EffectType)
	{
	case EStatusEffect::Stun:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 스턴 상태 적용"));
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		break;
		
	case EStatusEffect::Slow:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 이동 속도 감소"));
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
		break;

	case EStatusEffect::Burn:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 화상 효과 적용"));
		GetWorld()->GetTimerManager().SetTimer(StatusEffectTimers[EffectType], [this]()
		{
			TakeDamage(5.0f);
		}, 1.0f, true);
		break;

	case EStatusEffect::Freeze:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 빙결 효과 적용"));
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		break;

	case EStatusEffect::Poison:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 중독 효과 적용"));
		GetWorld()->GetTimerManager().SetTimer(StatusEffectTimers[EffectType], [this]()
		{
			TakeDamage(5.0f);
		}, 1.0f, true);
		break;
		
	default:
		break;
	}

	GetWorld()->GetTimerManager().SetTimer(StatusEffectTimers[EffectType], [this, EffectType]()
	{
		if (!OwnerCharacter) return;

		if (EffectType == EStatusEffect::Stun || EffectType == EStatusEffect::Freeze)
		{
			OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		else if (EffectType == EStatusEffect::Slow)
		{
			OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
		}

		if (EffectType == EStatusEffect::Burn)
		{
			GetWorld()->GetTimerManager().ClearTimer(StatusEffectTimers[EffectType]);
		}

		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트: 상태 이상 해제"));

	}, Duration, false);
}

void USKCombatComponent::HandleDeath()
{
	if (!OwnerCharacter) return;
	UE_LOG(LogTemp, Log, TEXT("캐릭터 사망: %s"), *OwnerCharacter->GetName());
	OwnerCharacter->Destroy();
}

void USKCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USKCombatComponent, MaxHealth);
	DOREPLIFETIME(USKCombatComponent, CurrentHealth);
}

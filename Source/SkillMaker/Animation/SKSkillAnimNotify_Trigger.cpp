// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SKSkillAnimNotify_Trigger.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/SKBaseCharacter.h"
#include "Combat/SKCombatComponent.h"
#include "Kismet/GameplayStatics.h"

void USKSkillAnimNotify_Trigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(!MeshComp)
		return;
	
	ASKBaseCharacter* Character = Cast<ASKBaseCharacter>(MeshComp->GetOwner());
	if(!Character)
		return;
	
	TOptional<FSKSkillData> SkillDataOpt = Character->GetCurrentSkillData();
	if (!SkillDataOpt.IsSet())
		return;
	const FSKSkillData& SkillData = SkillDataOpt.GetValue();
		
	if (!EffectName.IsNone())
	{
		UE_LOG(LogTemp, Log, TEXT("이펙트 실행: %s"), *EffectName.ToString());
	}

	if (!SoundName.IsNone())
	{
		UE_LOG(LogTemp, Log, TEXT("사운드 실행: %s"), *SoundName.ToString());
	}

	OnSkillNotifyTriggered.Broadcast(EffectName);

	ApplyAOEEffect(Character, SkillData);
	SpawnProjectile(Character, SkillData);
}

FString USKSkillAnimNotify_Trigger::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void USKSkillAnimNotify_Trigger::ApplyAOEEffect(ASKBaseCharacter* Character, const FSKSkillData& SkillData)
{
	if(!Character)
		return;

	float EffectRadius = SkillData.MaxRange; 
	FVector EffectLocation = Character->GetActorLocation();

	TArray<AActor*> AffectedActors;
	UKismetSystemLibrary::SphereOverlapActors(
		Character->GetWorld(),
		EffectLocation,
		EffectRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_Pawn)},
		ASKBaseCharacter::StaticClass(),
		TArray<AActor*>(),
		AffectedActors
	);

	for (AActor* Target : AffectedActors)
	{
		if (ASKBaseCharacter* TargetCharacter = Cast<ASKBaseCharacter>(Target))
		{
			UE_LOG(LogTemp, Log, TEXT("광역 효과 적용 대상: %s"), *TargetCharacter->GetName());

			USKCombatComponent* CombatComponent = TargetCharacter->FindComponentByClass<USKCombatComponent>();
			if (!CombatComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("광역 효과 적용 실패 - 대상에 전투 컴포넌트 없음: %s"), *TargetCharacter->GetName());
				continue;
			}

			for (const FStatusEffectData& StatusEffectData : SkillData.StatusEffects)
			{
				CombatComponent->ApplyStatusEffect(StatusEffectData.EffectType, StatusEffectData.Duration);
			}
		}
	}
}

void USKSkillAnimNotify_Trigger::SpawnProjectile(ASKBaseCharacter* Character, const FSKSkillData& SkillData)
{
	if(!Character || !SkillData.ProjectileClass)
		return;

	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = Character->GetActorRotation();

	AActor* SpawnedProjectile = Character->GetWorld()->SpawnActor<AActor>(SkillData.ProjectileClass, SpawnLocation, SpawnRotation);
	if (SpawnedProjectile)
	{
		UE_LOG(LogTemp, Log, TEXT("발사체 생성: %s"), *SpawnedProjectile->GetName());
	}
}

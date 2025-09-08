// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SKSkillAnimNotify_Trigger.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/SKBaseCharacter.h"
#include "Combat/SKCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKProjectileActor.h"

void USKSkillAnimNotify_Trigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
										const FAnimNotifyEventReference& EventReference)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	ASKBaseCharacter* Character = Cast<ASKBaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	TOptional<FSKSkillData> SkillDataOpt = Character->GetCurrentSkillData();
	if (!SkillDataOpt.IsSet()) 
	{
		SK_LOG(LogSkillMaker, Error, TEXT("❌ 스킬 데이터가 없음!"));
		return;
	}

	const FSKSkillData& SkillData = SkillDataOpt.GetValue();
	
	SK_LOG(LogSkillMaker, Log, TEXT("Now Skill : %s"), *SkillData.SkillName);

	// ✅ 저장된 NotifyName과 비교
	if (NotifyTriggerName == SkillData.NotifyName)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 실행됨: %s"), *SkillData.SkillName);
		if (SkillData.ProjectileActor)
		{
			SK_LOG(LogSkillMaker, Log, TEXT("발사체 : %s 스폰"), *SkillData.ProjectileActor->GetName());
			SpawnProjectile(Character, SkillData.ProjectileActor);
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 실행 실패: %s, 이름이 다름 : %s != %s"), *SkillData.SkillName, *NotifyTriggerName.ToString(), *SkillData.NotifyName.ToString());
	}
	
	// OnSkillNotifyTriggered.Broadcast(EffectName);
	
	// SpawnSkillEffect(Character, SkillData);
	// ApplyAOEEffect(Character, SkillData);
	// SpawnProjectile(Character, SkillData);
}

FString USKSkillAnimNotify_Trigger::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void USKSkillAnimNotify_Trigger::ApplyAOEEffect(ASKBaseCharacter* Character, const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
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

	SK_LOG(LogSkillMaker, Log, TEXT("영향 받은 대상의 수 : %d"), AffectedActors.Num());
	for (AActor* Target : AffectedActors)
	{
		if (ASKBaseCharacter* TargetCharacter = Cast<ASKBaseCharacter>(Target))
		{
			SK_LOG(LogSkillMaker, Log, TEXT("광역 효과 적용 대상: %s"), *TargetCharacter->GetName());

			USKCombatComponent* CombatComponent = TargetCharacter->FindComponentByClass<USKCombatComponent>();
			if (!CombatComponent)
			{
				SK_LOG(LogSkillMaker, Warning, TEXT("광역 효과 적용 실패 - 대상에 전투 컴포넌트 없음: %s"), *TargetCharacter->GetName());
				continue;
			}

			for (const FStatusEffectData& StatusEffectData : SkillData.StatusEffects)
			{
				CombatComponent->ApplyStatusEffect(StatusEffectData.EffectType, StatusEffectData.Duration);
			}
		}
	}
}

void USKSkillAnimNotify_Trigger::SpawnProjectile(ASKBaseCharacter* Character, const TSubclassOf<ASKProjectileActor> ProjectileClass)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if(!Character || !ProjectileClass) return;

	UWorld* World = Character->GetWorld();
	if(!World) return;

	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = Character->GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Character;
	SpawnParameters.Instigator = Character;

	ASKProjectileActor* Projectile = World->SpawnActor<ASKProjectileActor>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParameters
	);

	if(Projectile)
	{
		Projectile->StartProject(Character);
	}
}

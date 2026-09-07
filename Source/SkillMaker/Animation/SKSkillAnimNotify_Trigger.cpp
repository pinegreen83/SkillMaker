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
		if (!SkillData.ProjectileActor.IsNull())
		{
			TSubclassOf<ASKProjectileActor> ProjectileClass = SkillData.ProjectileActor.LoadSynchronous();
			if (!ProjectileClass)
			{
				SK_LOG(LogSkillMaker, Error, TEXT("발사체 클래스 로드 실패: %s"), *SkillData.ProjectileActor.ToSoftObjectPath().ToString());
				return;
			}

			SK_LOG(LogSkillMaker, Log, TEXT("발사체 : %s 스폰"), *ProjectileClass->GetName());
			SpawnProjectile(Character, ProjectileClass, SkillData);
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 실행 실패: %s, 이름이 다름 : %s != %s"), *SkillData.SkillName, *NotifyTriggerName.ToString(), *SkillData.NotifyName.ToString());
	}
	
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

void USKSkillAnimNotify_Trigger::SpawnProjectile(ASKBaseCharacter* Character,
	const TSubclassOf<ASKProjectileActor> ProjectileClass, const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if(!Character || !ProjectileClass) return;

	UWorld* World = Character->GetWorld();
	if(!World) return;

	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = Character->GetActorRotation();

	const FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	ASKProjectileActor* Projectile = World->SpawnActorDeferred<ASKProjectileActor>(
		ProjectileClass, SpawnTransform, Character, Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!Projectile)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("발사체 SpawnActor 실패: %s"), *GetNameSafe(ProjectileClass.Get()));
		return;
	}

	const FSKSkillImpactData ImpactData(SkillData, Character);
	SK_LOG(LogSkillMaker, Log,
		TEXT("발사체 공격 정보 생성: Projectile=%s / Source=%s / SkillID=%s / SkillName=%s / BaseDamage=%.2f / Element=%s / StatusEffectCount=%d"),
		*Projectile->GetName(), *Character->GetName(), *ImpactData.SkillID.ToString(), *ImpactData.SkillName,
		ImpactData.DamageValue, *ImpactData.ElementTag.ToString(), ImpactData.StatusEffects.Num());
	SK_LOG(LogSkillMaker, Log,
		TEXT("발사체 공격 정보 상세: SkillType=%d / TargetingType=%d / Weapon=%s / Duration=%.2f / CanMove=%s / Cooldown=%.2f / Cost=%.2f / Range=%.2f~%.2f / AffectEnemies=%s / AffectAllies=%s"),
		static_cast<int32>(ImpactData.SkillType), static_cast<int32>(ImpactData.TargetingType),
		*ImpactData.WeaponTag.ToString(), ImpactData.SkillDuration,
		ImpactData.bCanMoveWhileChanneling ? TEXT("True") : TEXT("False"), ImpactData.CooldownTime,
		ImpactData.Cost, ImpactData.MinRange, ImpactData.MaxRange,
		ImpactData.bAffectEnemies ? TEXT("True") : TEXT("False"),
		ImpactData.bAffectAllies ? TEXT("True") : TEXT("False"));
	Projectile->InitializeProjectile(Character, ImpactData);
	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	SK_LOG(LogSkillMaker, Log, TEXT("발사체 생성 완료: %s / Location=%s / Rotation=%s / ImpactInitialized=True"),
		*Projectile->GetName(), *Projectile->GetActorLocation().ToString(), *Projectile->GetActorRotation().ToString());
	Projectile->StartProject();
}

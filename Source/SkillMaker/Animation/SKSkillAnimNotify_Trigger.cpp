// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SKSkillAnimNotify_Trigger.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/SKBaseCharacter.h"
#include "Combat/SKCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillEffectActor.h"

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

	// ✅ SkillData 내부 확인 로그 추가
	SK_LOG(LogSkillMaker, Log, TEXT("✔ SkillData 확인 - 이펙트: %s, 사운드: %s"),
		SkillData.EffectSoundData.EffectClass ? *SkillData.EffectSoundData.EffectClass->GetName() : TEXT("None"),
		SkillData.EffectSoundData.Sound ? *SkillData.EffectSoundData.Sound->GetName() : TEXT("None")
	);

	// ✅ 현재 애니메이션 진행 시간 가져오기
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	float CurrentMontageTime = AnimInstance->Montage_GetPosition(AnimInstance->GetCurrentActiveMontage());
	SK_LOG(LogSkillMaker, Log, TEXT("Time diff comp Now Montage Time : %f / SetupMontage Time : %f"), CurrentMontageTime, SkillData.EffectSoundData.NotifyTime);
	
	// ✅ 저장된 NotifyTime과 비교
	if (FMath::IsNearlyEqual(SkillData.EffectSoundData.NotifyTime, CurrentMontageTime, 0.1f))
	{
		SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 실행됨: %s (%.2f초)"), *EffectName.ToString(), CurrentMontageTime);

		SpawnSkillEffect(Character, SkillData);
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

void USKSkillAnimNotify_Trigger::SpawnProjectile(ASKBaseCharacter* Character, const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!Character || !SkillData.ProjectileClass)
		return;

	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = Character->GetActorRotation();

	AActor* SpawnedProjectile = Character->GetWorld()->SpawnActor<AActor>(SkillData.ProjectileClass, SpawnLocation, SpawnRotation);
	if (SpawnedProjectile)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("발사체 생성: %s"), *SpawnedProjectile->GetName());
	}
}

void USKSkillAnimNotify_Trigger::SpawnSkillEffect(ASKBaseCharacter* Character, const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("USKSkillAnimNotify_Trigger::SpawnSkillEffect() Begin"));
	
	if (!Character || !SkillData.EffectSoundData.EffectClass) return;

	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 50.0f;
	FRotator SpawnRotation = Character->GetActorRotation();

	ASKSkillEffectActor* SpawnedEffect = Character->GetWorld()->SpawnActor<ASKSkillEffectActor>(
		SkillData.EffectSoundData.EffectClass,
		SpawnLocation,
		SpawnRotation
	);

	if (SpawnedEffect)
	{
		SpawnedEffect->PlayEffect(Character, SkillData);
		SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이에서 이펙트 생성: %s"), *SpawnedEffect->GetName());
	}
}

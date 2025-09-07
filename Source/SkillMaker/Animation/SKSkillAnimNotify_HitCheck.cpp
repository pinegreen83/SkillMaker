// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillAnimNotify_HitCheck.h"
#include "Character/SKBaseCharacter.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKSkillAnimNotify_HitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp) return;
	
	AActor* Owner = MeshComp->GetOwner();
	ASKBaseCharacter* Character = Cast<ASKBaseCharacter>(Owner);
	if (!Character) return;

	// 스킬 데이터 가져오기
	TOptional<FSKSkillData> SkillDataOpt = Character->GetCurrentSkillData();
	if (!SkillDataOpt.IsSet()) return;
	const FSKSkillData& SkillData = SkillDataOpt.GetValue();

	// 근접 타격 판정 실행
	PerformMeleeAttack(Character, SkillData);
}

void USKSkillAnimNotify_HitCheck::PerformMeleeAttack(ASKBaseCharacter* Character, const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!Character) return;

	FVector Start = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector End = Start + (ForwardVector * SkillData.MaxRange);

	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SkillData.MaxRange);

	bool bHit = Character->GetWorld()->SweepMultiByChannel(
		HitResults, Start, End, FQuat::Identity,
		ECC_Pawn, CollisionShape
	);

	SK_LOG(LogSkillMaker, Log, TEXT("AttackHit"));
	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			ASKBaseCharacter* TargetCharacter = Cast<ASKBaseCharacter>(Hit.GetActor());
			if (TargetCharacter && TargetCharacter != Character)
			{
				// TODO: 피해 적용 로직 추가
				SK_LOG(LogSkillMaker, Log, TEXT("근접 타격 성공: %s"), *TargetCharacter->GetName());
			}
		}
	}
}

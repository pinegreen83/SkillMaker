// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillEffectActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASKSkillEffectActor::ASKSkillEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASKSkillEffectActor::OnHit);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(RootComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->InitialSpeed = 1000.0f;
	ProjectileComponent->MaxSpeed = 2000.0f;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ASKSkillEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(5.0f);
}

void ASKSkillEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASKSkillEffectActor::PlayEffect(ACharacter* InSkillOwner, const FSKSkillData& SkillData)
{
	SkillOwner = InSkillOwner;

	if (SkillData.EffectSoundData.Sound && SkillOwner)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SkillData.EffectSoundData.Sound, SkillOwner->GetActorLocation());
	}
	
	if (!ProjectileComponent || ProjectileComponent->InitialSpeed == 0.0f)
	{
		SetLifeSpan(3.0f);
	}
}

void ASKSkillEffectActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != SkillOwner)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
		if (HitCharacter)
		{
			ApplyStatusEffect(HitCharacter);
		}
		Destroy();
	}
}

void ASKSkillEffectActor::ApplyStatusEffect(ACharacter* TargetCharacter)
{
	if (!SkillOwner) return;

	// 스킬 데이터에서 상태이상 가져오기
	// for (const FStatusEffectData& Effect : SkillOwner->CurrentSkillData.StatusEffects)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("%s 가 상태이상 %d 를 적용받음. 지속시간: %.1f초"),
	// 		   *TargetCharacter->GetName(), (int32)Effect.EffectType, Effect.Duration);
	// }
}


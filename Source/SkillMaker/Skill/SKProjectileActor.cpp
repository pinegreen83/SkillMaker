// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"

// Sets default values
ASKProjectileActor::ASKProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASKProjectileActor::OnOverlap);

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
void ASKProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(5.0f);
}

void ASKProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASKProjectileActor::StartProject(ACharacter* InSkillOwner)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!InSkillOwner)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("발사체 시작 실패: 시전자 없음."));
		return;
	}

	SkillOwner = InSkillOwner;
	if (CollisionComponent)
	{
		CollisionComponent->IgnoreActorWhenMoving(InSkillOwner, true);
	}

	bool bHasVisualEffect = false;
	if (ParticleComponent && ParticleComponent->GetFXSystemAsset())
	{
		ParticleComponent->SetVisibility(true);
		ParticleComponent->SetHiddenInGame(false);
		ParticleComponent->ActivateSystem(true);
		bHasVisualEffect = true;
	}

	if (NiagaraComponent && NiagaraComponent->GetAsset())
	{
		NiagaraComponent->SetVisibility(true);
		NiagaraComponent->SetHiddenInGame(false);
		NiagaraComponent->Activate(true);
		bHasVisualEffect = true;
	}

	if (!bHasVisualEffect)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("발사체에 출력할 시각 이펙트가 없음: %s"), *GetName());
	}

	if (ProjectileSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileSound, GetActorLocation());
	}
	else
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("발사체에 출력할 사운드가 없음: %s"), *GetName());
	}

	if (!ProjectileComponent || ProjectileComponent->InitialSpeed == 0.0f)
	{
		SetLifeSpan(3.0f);
	}
}

void ASKProjectileActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == SkillOwner || OtherActor == GetOwner())
	{
		return;
	}

	SK_LOG(LogSkillMaker, Log, TEXT("발사체 충돌: Projectile=%s / OtherActor=%s / OtherComponent=%s"),
		*GetName(), *GetNameSafe(OtherActor), *GetNameSafe(OtherComp));

	ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
	if (HitCharacter)
	{
		ApplyStatusEffect(HitCharacter);
	}
	Destroy();
}

void ASKProjectileActor::ApplyStatusEffect(ACharacter* TargetCharacter)
{
	if (!SkillOwner) return;

	// 스킬 데이터에서 상태이상 가져오기
	// for (const FStatusEffectData& Effect : SkillOwner->CurrentSkillData.StatusEffects)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("%s 가 상태이상 %d 를 적용받음. 지속시간: %.1f초"),
	// 		   *TargetCharacter->GetName(), (int32)Effect.EffectType, Effect.Duration);
	// }
}

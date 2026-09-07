// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Character/SKBaseCharacter.h"
#include "Combat/SKCombatComponent.h"
#include "Components/SphereComponent.h"
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

void ASKProjectileActor::InitializeProjectile(ASKBaseCharacter* InSkillOwner, const FSKSkillImpactData& InImpactData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("발사체 전투 정보 초기화 시작: Projectile=%s"), *GetName());

	if (!InSkillOwner)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("발사체 전투 정보 초기화 실패: 시전자 없음."));
		return;
	}

	SkillOwner = InSkillOwner;
	ImpactData = InImpactData;
	bHasImpactData = !ImpactData.SkillID.IsNone() && ImpactData.SourceCharacter != nullptr;
	SK_LOG(LogSkillMaker, Log,
		TEXT("발사체 공격 정보 저장: Projectile=%s / Valid=%s / Source=%s / SkillID=%s / SkillName=%s / BaseDamage=%.2f / Element=%s / StatusEffectCount=%d"),
		*GetName(), bHasImpactData ? TEXT("True") : TEXT("False"), *GetNameSafe(ImpactData.SourceCharacter),
		*ImpactData.SkillID.ToString(), *ImpactData.SkillName, ImpactData.DamageValue,
		*ImpactData.ElementTag.ToString(), ImpactData.StatusEffects.Num());
	if (CollisionComponent)
	{
		CollisionComponent->IgnoreActorWhenMoving(InSkillOwner, true);
	}
}

void ASKProjectileActor::StartProject()
{
	SK_LOG(LogSkillMaker, Log, TEXT("발사체 표현 시작: Projectile=%s / HasImpactData=%s / Source=%s"),
		*GetName(), bHasImpactData ? TEXT("True") : TEXT("False"), *GetNameSafe(SkillOwner));

	if (!bHasImpactData || !SkillOwner)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("발사체 표현 시작 실패: 전투 정보 또는 시전자 없음. Projectile=%s"),
			*GetName());
		Destroy();
		return;
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

	ASKBaseCharacter* HitCharacter = Cast<ASKBaseCharacter>(OtherActor);
	if (HitCharacter)
	{
		ApplySkillImpact(HitCharacter);
	}
	else
	{
		SK_LOG(LogSkillMaker, Log, TEXT("발사체 충돌 대상은 ASKBaseCharacter가 아님: OtherActor=%s"),
			*GetNameSafe(OtherActor));
	}
	Destroy();
}

void ASKProjectileActor::ApplySkillImpact(ASKBaseCharacter* TargetCharacter)
{
	if (!TargetCharacter)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 충격 전달 실패: 대상 캐릭터 없음."));
		return;
	}

	if (!bHasImpactData || !SkillOwner || !ImpactData.SourceCharacter)
	{
		SK_LOG(LogSkillMaker, Error,
			TEXT("스킬 충격 전달 실패: Projectile=%s / HasImpactData=%s / SkillOwner=%s / Source=%s / SkillID=%s"),
			*GetName(), bHasImpactData ? TEXT("True") : TEXT("False"), *GetNameSafe(SkillOwner),
			*GetNameSafe(ImpactData.SourceCharacter), *ImpactData.SkillID.ToString());
		return;
	}

	if (!TargetCharacter->HasAuthority())
	{
		SK_LOG(LogSkillMaker, Log,
			TEXT("클라이언트 발사체는 시각 충돌만 처리함: Projectile=%s / Target=%s / SkillID=%s"),
			*GetName(), *TargetCharacter->GetName(), *ImpactData.SkillID.ToString());
		return;
	}

	USKCombatComponent* CombatComponent = TargetCharacter->FindComponentByClass<USKCombatComponent>();
	if (!CombatComponent)
	{
		SK_LOG(LogSkillMaker, Warning,
			TEXT("스킬 충격 전달 실패: 대상에 CombatComponent 없음. Target=%s / SkillID=%s / Source=%s"),
			*TargetCharacter->GetName(), *ImpactData.SkillID.ToString(), *GetNameSafe(ImpactData.SourceCharacter));
		return;
	}

	SK_LOG(LogSkillMaker, Log,
		TEXT("CombatComponent로 스킬 충격 전달: Projectile=%s / Source=%s / Target=%s / SkillID=%s / BaseDamage=%.2f"),
		*GetName(), *GetNameSafe(ImpactData.SourceCharacter), *TargetCharacter->GetName(),
		*ImpactData.SkillID.ToString(), ImpactData.DamageValue);
	CombatComponent->ApplySkillImpact(ImpactData);
}

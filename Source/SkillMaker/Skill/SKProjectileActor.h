// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Skill/SKSkillData.h"
#include "SKProjectileActor.generated.h"

class ASKBaseCharacter;
class UNiagaraComponent;
class USphereComponent;
class UParticleSystemComponent;
class USoundBase;
class UProjectileMovementComponent;

UCLASS()
class SKILLMAKER_API ASKProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASKProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** 충돌이 활성화되기 전에 시전자와 전투 정보를 저장 */
	void InitializeProjectile(ASKBaseCharacter* InSkillOwner, const FSKSkillImpactData& InImpactData);

	/** 생성 완료 후 이펙트와 사운드 실행 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void StartProject();
	
	/** 스킬을 사용한 캐릭터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<ASKBaseCharacter> SkillOwner;

	/** 발사 순간에 복사한 전투 판정 정보 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	FSKSkillImpactData ImpactData;

protected:
	/** 파티클 시스템 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UParticleSystemComponent> ParticleComponent;

	/** 나이아가라 시스템 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	/** 충돌 감지를 위한 콜리전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;

	/** 투사체 이동을 위한 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileComponent;

	/** 발사체 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<USoundBase> ProjectileSound;
	
	/** 충돌 처리 함수 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/** 대상의 전투 컴포넌트에 스킬 충격 정보를 전달 */
	void ApplySkillImpact(ASKBaseCharacter* TargetCharacter);

	bool bHasImpactData = false;
};

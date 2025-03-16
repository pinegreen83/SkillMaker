// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKSkillData.h"
#include "SKSkillEffectActor.generated.h"

class ACharacter;
class UNiagaraComponent;
class USphereComponent;
class UParticleSystemComponent;
class USoundBase;
class UProjectileMovementComponent;

UCLASS()
class SKILLMAKER_API ASKSkillEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASKSkillEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** 파티클 시스템 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<UParticleSystemComponent> ParticleComponent;

	/** 나이아가라 시스템 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	/** 충돌 감지를 위한 콜리전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<USphereComponent> CollisionComponent;

	/** 투사체 이동을 위한 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<UProjectileMovementComponent> ProjectileComponent;
	
	/** 스킬을 사용한 캐릭터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<ACharacter> SkillOwner;

	/** 이펙트 실행 함수 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffect(ACharacter* InSkillOwner, const FSKSkillData& SkillData);
	
protected:
	/** 충돌 처리 함수 */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/** 상태이상 적용 함수 */
	void ApplyStatusEffect(ACharacter* TargetCharacter);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKInteractableActor.generated.h"

UCLASS()
class SKILLMAKER_API ASKInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
	ASKInteractableActor();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<class UBoxComponent> InteractionTrigger;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionRange = 200.f;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnPlayerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnPlayerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/SKInteractableActor.h"

#include "Character/SKPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
ASKInteractableActor::ASKInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASKInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASKInteractableActor::OnPlayerEnter);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &ASKInteractableActor::OnPlayerExit);
}

void ASKInteractableActor::OnInteract()
{
}

void ASKInteractableActor::OnPlayerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(OtherActor))
	{
		PlayerCharacter->SetInteractableTarget(this);
	}
}

void ASKInteractableActor::OnPlayerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(OtherActor))
	{
		PlayerCharacter->ClearInteractableTarget(this);
	}
}


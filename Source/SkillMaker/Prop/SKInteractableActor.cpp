// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/SKInteractableActor.h"

// Sets default values
ASKInteractableActor::ASKInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASKInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASKInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_GrabbableActor.h"
#include "Engine/Engine.h"


// Sets default values
AVR_GrabbableActor::AVR_GrabbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GrabType = EVR_GrabTypeB::VR_NULL;
}

// Called when the game starts or when spawned
void AVR_GrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVR_GrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVR_GrabbableActor::OnGrabbed_Implementation()
{
}

void AVR_GrabbableActor::OnReleased_Implementation()
{	
}

void AVR_GrabbableActor::OnInteract_Implementation()
{
}
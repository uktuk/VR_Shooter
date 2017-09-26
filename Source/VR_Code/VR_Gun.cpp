// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_Gun.h"
#include "Engine/Engine.h"
#include <Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include "VR_GrabbableActor.h"

AVR_Gun::AVR_Gun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComp;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

	GrabType = EVR_GrabTypeB::VR_SnapToSocket;
}


 void AVR_Gun::OnInteract_Implementation()
 {
 	 UWorld* const World = GetWorld();
 	 if (World && ProjectileClass)
 	 { 	 			
		World->SpawnActor<AActor>(ProjectileClass, SkeletalMesh->GetSocketTransform("Muzzle"));		
 	 }
 }

void AVR_Gun::OnGrabbed_Implementation()
{	
}

void AVR_Gun::OnReleased_Implementation()
{
}
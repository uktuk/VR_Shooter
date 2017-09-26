// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_GrabComponent.h"
#include "Engine/Engine.h"
#include "VR_MotionController.h"


UVR_GrabComponent::UVR_GrabComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GrabType = EVR_GrabType::VR_NULL;
	bIsHeld = false;
}

void UVR_GrabComponent::AttachComponentToSocket(USkeletalMeshComponent* Mesh, FName socket)
{
	// GetOwner()->SetActorLocation(ParentSocketTransform->GetLocation() + RelativeLocation);	
	GetOwner()->SetActorRotation(Mesh->GetSocketTransform(socket).GetRotation());
	GetOwner()->AddActorLocalRotation(RelativeRotation);

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector ComponentLocation = GetComponentLocation();
	FVector RelativeLocation = OwnerLocation - ComponentLocation;

	GetOwner()->SetActorLocation(Mesh->GetSocketTransform(socket).GetLocation() + RelativeLocation);
	
	GetOwner()->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);
}
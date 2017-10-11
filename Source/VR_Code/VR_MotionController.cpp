// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_MotionController.h"
#include <MotionControllerComponent.h>
#include "Engine/Engine.h"
#include <Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include "HandAnimInstance.h"
#include "VR_GrabComponent.h"


// Sets default values
AVR_MotionController::AVR_MotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the MotionControllerComponents and attach to Root (this is what tracks the motion controllers)
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	RootComponent = MotionController;

	// Create SceneComponent as root so the child components will translate relative to parent actor
	VROrigin = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	VROrigin->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Creates Left Hand mesh
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	HandMesh->AttachToComponent(VROrigin, FAttachmentTransformRules::KeepRelativeTransform);
	
	// Setup collision sphere
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->AttachToComponent(MotionController, FAttachmentTransformRules::KeepRelativeTransform);

	HeldComponent = nullptr;
}

// Called when the game starts or when spawned
void AVR_MotionController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVR_MotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HeldComponent)
	{
		HeldComponent->UpdateHandTransform(MotionController->GetComponentTransform());
	}
}

void AVR_MotionController::SetHand(EControllerHand hand)
{
	MotionController->Hand = hand;
	if (hand == EControllerHand::Left)
	{
		HandMesh->SetWorldScale3D(FVector(1.0, 1.0, -1.0));		
	}
}

const EControllerHand AVR_MotionController::GetHand() const
{
	return MotionController->Hand;	
}

void AVR_MotionController::SetHandAxisVal(float val)
{
	UHandAnimInstance* handAnim = Cast<UHandAnimInstance>(HandMesh->GetAnimInstance());

	if (handAnim)
	{
		handAnim->HandGripLevel = val;		
	}	
}

UVR_GrabComponent* AVR_MotionController::CheckOverlappedComponent()
{
	// Check if SphereComponent is overlapping any grabbable actors
	TArray<UPrimitiveComponent*> overlappingComponents;
	SphereComponent->GetOverlappingComponents(overlappingComponents);

	UVR_GrabComponent* closestGrabComponent = nullptr;

	for (int i = 0; i < overlappingComponents.Num(); ++i)
	{	
		UVR_GrabComponent* overlappingGrabComponent = Cast<UVR_GrabComponent>(overlappingComponents[i]);
		if (overlappingGrabComponent)
		{
			// Get closest overlapping grab component
			if (closestGrabComponent == nullptr ||
				(FVector::DistSquared(SphereComponent->GetComponentLocation(), overlappingGrabComponent->GetComponentLocation()) < FVector::DistSquared(SphereComponent->GetComponentLocation(), closestGrabComponent->GetComponentLocation())))
			{
				closestGrabComponent = overlappingGrabComponent;
			}
		}
	}
	return closestGrabComponent;
}

void AVR_MotionController::GrabComponent(UVR_GrabComponent* ComponentToGrab, bool isLeftHand)
{
	if (ComponentToGrab)
	{
		FName socket = isLeftHand ? "Grasp_Point_Inverted" : "Grasp_Point";
		switch (ComponentToGrab->GrabType)
		{
			case EVR_GrabType::VR_NULL :
			{
				break;
			}
			case EVR_GrabType::VR_FreeGrab :
			{				
				ComponentToGrab->OnVRGrabbed();
				ComponentToGrab->GetOwner()->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepWorldTransform);
				HeldComponent = ComponentToGrab;
				break;
			}
			case EVR_GrabType::VR_SnapToSocket :
			{				
				ComponentToGrab->OnVRGrabbed();				
				ComponentToGrab->AttachComponentToSocket(HandMesh, socket);
				HeldComponent = ComponentToGrab;
				break;
			}
			case EVR_GrabType::VR_RestrictedGrab :
			{
				ComponentToGrab->OnVRGrabbed();				
				AttachHandToComponent(ComponentToGrab, GetHand());
				HeldComponent = ComponentToGrab;
				break;
			}
			default:
				break;
		}
	}	
}

void AVR_MotionController::ReleaseComponent()
{
	if (HeldComponent != nullptr)
	{
		if (HeldComponent->GrabType == EVR_GrabType::VR_RestrictedGrab)
		{
			ReattachHandToMotionController();
			HeldComponent->OnVRReleased();
			HeldComponent->bIsHeld = false;
		}
		else
		{
			// Get the actor that the HeldComponent's root actor is attached to (should be a motion controller)
			AVR_MotionController* parentActor = Cast<AVR_MotionController>(HeldComponent->GetOwner()->GetAttachParentActor());
			if (parentActor && parentActor == this)
			{			
				HeldComponent->OnVRReleased();
				HeldComponent->bIsHeld = false;
				HeldComponent->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			}
		}
	}
	HeldComponent = nullptr;
}

void AVR_MotionController::AttachHandToComponent(USceneComponent* parentComponent, EControllerHand HandType)
{
	if (parentComponent)
	{
		VROrigin->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		FRotator parentComponentRotation = parentComponent->GetComponentRotation();

		VROrigin->SetWorldRotation(parentComponent->GetComponentRotation());
		VROrigin->SetWorldLocation(parentComponent->GetComponentLocation());

		UVR_GrabComponent* parentGrabComponent = Cast<UVR_GrabComponent>(parentComponent);
		if (parentGrabComponent)
		{
			switch (HandType)
			{
			case EControllerHand::Left:
			{
				VROrigin->AddLocalOffset(parentGrabComponent->AttachPointTransformForLeftHand.GetLocation() * parentGrabComponent->AttachPointTransformForRightHand.GetScale3D());
				VROrigin->AddLocalRotation(parentGrabComponent->AttachPointTransformForLeftHand.GetRotation());
				break;
			}
			case EControllerHand::Right:
			{
				VROrigin->AddLocalOffset(parentGrabComponent->AttachPointTransformForRightHand.GetLocation() * parentGrabComponent->AttachPointTransformForRightHand.GetScale3D());
				
				VROrigin->AddLocalRotation(parentGrabComponent->AttachPointTransformForRightHand.GetRotation());
				break;			
			}
			default:
				break;
			}			
		}
		VROrigin->AttachToComponent(parentComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AVR_MotionController::ReattachHandToMotionController()
{
	VROrigin->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	VROrigin->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);	
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_MotionController.h"
#include <MotionControllerComponent.h>
#include "Engine/Engine.h"
#include <Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include "HandAnimInstance.h"
#include "VR_GrabbableActor.h"


// Sets default values
AVR_MotionController::AVR_MotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create SceneComponent as root so the child components will translate relative to parent actor
	VROrigin = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	// RootComponent = VROrigin;

	// Create the MotionControllerComponents and attach to Root (this is what tracks the motion controllers)
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");	
	// MotionController->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);	
	RootComponent = MotionController;

	// Creates Left Hand mesh
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	HandMesh->AttachToComponent(MotionController, FAttachmentTransformRules::KeepRelativeTransform);		
	
	// Setup collision sphere
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->AttachToComponent(MotionController, FAttachmentTransformRules::KeepRelativeTransform);
	
	HeldActor = nullptr;	
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

AVR_GrabbableActor* AVR_MotionController::CheckOverlappedActor()
{
	// Check if SphereComponent is overlapping any grabbable actors
	TArray<AActor*> overlappedActors;	
	SphereComponent->GetOverlappingActors(overlappedActors, AVR_GrabbableActor::StaticClass());
	if (overlappedActors.Num() > 0)
	{
		// TODO:: return overlapped actors in order of distance from hand
		AVR_GrabbableActor* grabbedActor = Cast<AVR_GrabbableActor>(overlappedActors[0]);
		if (grabbedActor)
		{
			return grabbedActor;
		}
	}
	return nullptr;
}

void AVR_MotionController::GrabActor(AVR_GrabbableActor* ActorToGrab, bool isLeftHand)
{
	if (ActorToGrab)
	{
		FName socket = isLeftHand ? "Grasp_Point_Inverted" : "Grasp_Point";
		switch (ActorToGrab->GrabType)
		{
			case EVR_GrabType::VR_NULL :
			{
				break;
			}
			case EVR_GrabType::VR_FreeGrab :
			{
				ActorToGrab->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepWorldTransform);
				ActorToGrab->OnGrabbed();
				HeldActor = ActorToGrab;
				break;
			}
			case EVR_GrabType::VR_SnapToSocket :
			{
				ActorToGrab->AttachToComponent(HandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socket);
				ActorToGrab->OnGrabbed();
				HeldActor = ActorToGrab;
				break;
			}
			case EVR_GrabType::VR_RestrictedGrab :
			{
				break;
			}
			default:
				break;
		}
	}
}

void AVR_MotionController::ReleaseActor()
{
	if (HeldActor != nullptr)
	{
		AVR_MotionController* parentActor = Cast<AVR_MotionController>(HeldActor->GetAttachParentActor());
		if (parentActor && parentActor == this)
		{
			HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
	HeldActor = nullptr;
}
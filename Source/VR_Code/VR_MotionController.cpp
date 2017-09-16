// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_MotionController.h"
#include <MotionControllerComponent.h>
#include "Engine/Engine.h"
#include <Engine/SkeletalMesh.h>
#include <Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include "HandAnimInstance.h"


// Sets default values
AVR_MotionController::AVR_MotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create SceneComponent as root so the child components will translate relative to parent actor
	VROrigin = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = VROrigin;

	// Create the MotionControllerComponents and attach to Root (this is what tracks the motion controllers)
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");	
	MotionController->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);	

	// Creates Left Hand mesh
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	HandMesh->AttachToComponent(MotionController, FAttachmentTransformRules::KeepRelativeTransform);	
	
	

	//gripVal = 0.0;
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
		FMath::Clamp(handAnim->HandGripLevel, 0.0f, 1.0f);
	}	
}
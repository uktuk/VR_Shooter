// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_PlayerPawn.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include <Camera/CameraComponent.h>
#include "VR_MotionController.h"
#include "VR_GrabbableActor.h"
#include <ConstructorHelpers.h>


// Sets default values
AVR_PlayerPawn::AVR_PlayerPawn(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create SceneComponent as root so the child components will translate relative to parent actor
	VROrigin = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	VROrigin->AttachTo(RootComponent);

	// Create Camera and have it lock to HMD (sets tracking to head mounted display)
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->bLockToHmd = true;	
	Camera->AttachToComponent(VROrigin, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AVR_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawn and setup both left and right motion controllers
	UWorld* const World = GetWorld();
	if (World)
	{
		LeftVR_MotionController = World->SpawnActor<AVR_MotionController>(LeftMotionControllerBlueprint);
		LeftVR_MotionController->SetHand(EControllerHand::Left);
		LeftVR_MotionController->AttachToComponent(VROrigin, FAttachmentTransformRules::KeepRelativeTransform);
	
		RightVR_MotionController = World->SpawnActor<AVR_MotionController>(RightMotionControllerBlueprint);
		RightVR_MotionController->SetHand(EControllerHand::Right);
		RightVR_MotionController->AttachToComponent(VROrigin, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called every frame
void AVR_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVR_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("DropMag", IE_Pressed, this, &AVR_PlayerPawn::Func_ONE);
	PlayerInputComponent->BindAction("PickupMag", IE_Pressed, this, &AVR_PlayerPawn::Func_TWO);

	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &AVR_PlayerPawn::OnRightHandGrab);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &AVR_PlayerPawn::OnRightHandRelease);
	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &AVR_PlayerPawn::OnLeftHandGrab);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &AVR_PlayerPawn::OnLeftHandRelease);

	PlayerInputComponent->BindAction("RightInteract", IE_Pressed, this, &AVR_PlayerPawn::OnRightInteract);
	PlayerInputComponent->BindAction("LeftInteract", IE_Pressed, this, &AVR_PlayerPawn::OnLeftInteract);

	PlayerInputComponent->BindAxis("RightGripAxis", this, &AVR_PlayerPawn::SetRightHandGripVal);
	PlayerInputComponent->BindAxis("LeftGripAxis", this, &AVR_PlayerPawn::SetLeftHandGripVal);
}

UFUNCTION(CPF_BlueprintCallable)
void AVR_PlayerPawn::Func_ONE()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAG EJECT"));
	}
}

UFUNCTION(CPF_BlueprintCallable)
void AVR_PlayerPawn::Func_TWO()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAG RELOAD"));
	}	
}

void AVR_PlayerPawn::SetRightHandGripVal(float val)
{	
	FMath::Clamp(val, 0.0f, 1.0f);
	RightVR_MotionController->SetHandAxisVal(val);
}

void AVR_PlayerPawn::SetLeftHandGripVal(float val)
{
	FMath::Clamp(val, 0.0f, 1.0f);
	LeftVR_MotionController->SetHandAxisVal(val);
}


void AVR_PlayerPawn::OnRightHandGrab()
{
	if (RightVR_MotionController)
	{
		AVR_GrabbableActor* overlappedActor = RightVR_MotionController->CheckOverlappedActor();
		if (overlappedActor)
		{
			RightVR_MotionController->GrabActor(overlappedActor, false);
		}
	}
}


void AVR_PlayerPawn::OnRightHandRelease()
{	
	if (RightVR_MotionController)
	{
		RightVR_MotionController->ReleaseActor();
	}
}


void AVR_PlayerPawn::OnLeftHandGrab()
{
	if (LeftVR_MotionController)
	{
		AVR_GrabbableActor* overlappedActor = LeftVR_MotionController->CheckOverlappedActor();
		if (overlappedActor)
		{
			LeftVR_MotionController->GrabActor(overlappedActor, true);
		}
	}
}


void AVR_PlayerPawn::OnLeftHandRelease()
{
	if (LeftVR_MotionController)
	{
		LeftVR_MotionController->ReleaseActor();
	}
}

void AVR_PlayerPawn::OnRightInteract()
{
	if (RightVR_MotionController && RightVR_MotionController->HeldActor)
	{
		RightVR_MotionController->HeldActor->OnInteract();
	}
}

void AVR_PlayerPawn::OnLeftInteract()
{
	if (LeftVR_MotionController && LeftVR_MotionController->HeldActor)
	{
		LeftVR_MotionController->HeldActor->OnInteract();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_PlayerPawn.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include <Camera/CameraComponent.h>
#include "VR_MotionController.h"
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
	PlayerInputComponent->BindAxis("RightTriggerAxis", this, &AVR_PlayerPawn::SetRightHandGripVal);
	PlayerInputComponent->BindAxis("LeftTriggerAxis", this, &AVR_PlayerPawn::SetLeftHandGripVal);
}

UFUNCTION(CPF_BlueprintCallable)
void AVR_PlayerPawn::Func_ONE()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAG EJECT"));
	}
	// LeftHandMesh->DetachFromParent(true);
	// LeftHandMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

UFUNCTION(CPF_BlueprintCallable)
void AVR_PlayerPawn::Func_TWO()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAG RELOAD"));
	}
	// LeftHandMesh->AttachToComponent(LeftMotionController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AVR_PlayerPawn::SetRightHandGripVal(float val)
{	
	RightVR_MotionController->SetHandAxisVal(val);
}

void AVR_PlayerPawn::SetLeftHandGripVal(float val)
{
	LeftVR_MotionController->SetHandAxisVal(val);	
}

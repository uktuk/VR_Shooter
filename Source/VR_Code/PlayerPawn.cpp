// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include <MotionControllerComponent.h>
#include <Components/SkeletalMeshComponent.h>

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create SceneComponent as root so the child components will translate relative to parent actor
	VROrigin = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = VROrigin;

	// Create the MotionControllerComponents and attach to Root (this is what tracks the motion controllers)
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>("LMotionController");
	LeftMotionController->Hand = EControllerHand::Left;
	LeftMotionController->AttachTo(RootComponent);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>("RMotionController");
	RightMotionController->Hand = EControllerHand::Right;
	RightMotionController->AttachTo(RootComponent);
		
	// Create Left Hand mesh and invert its Z-scale axis (to create a left hand from the right mesh)
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LeftHandMesh");
	LeftHandMesh->AttachTo(LeftMotionController);	
	

	// Creates Right Hand mesh
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RightHandMesh");	
	RightHandMesh->AttachTo(RightMotionController);
	// RightHandMesh->AttachToComponent(RightMotionController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();	
	
	LeftHandMesh->SetWorldScale3D(FVector(1.0, 1.0, -1.0));
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("DropMag", IE_Pressed, this, &APlayerPawn::Func_ONE);
	PlayerInputComponent->BindAction("PickupMag", IE_Pressed, this, &APlayerPawn::Func_TWO);
}

UFUNCTION(CPF_BlueprintCallable)
void APlayerPawn::Func_ONE()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAG EJECT"));
	}
	LeftHandMesh->DetachFromParent(true);
}

UFUNCTION(CPF_BlueprintCallable)
void APlayerPawn::Func_TWO()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAG RELOAD"));
	}
	LeftHandMesh->AttachToComponent(LeftMotionController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
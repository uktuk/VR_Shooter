// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VR_GrabbableActor.h"
#include "VR_MotionController.generated.h"

class UMotionControllerComponent;
class USphereComponent;

UCLASS()
class VR_CODE_API AVR_MotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVR_MotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// void SetRightHandGripVal(float val);
	void SetHand(EControllerHand hand);

	UFUNCTION(BlueprintCallable, Category = "VR MotionController")
	const EControllerHand GetHand() const;

	UFUNCTION(BlueprintCallable, Category = "VR MotionControllers")
	void SetHandAxisVal(float val);

	// Check if motion controller is overlapping a grabbable actor
	AVR_GrabbableActor* CheckOverlappedActor();

	// Attempt to grab actor (specify if left hand for correct socket snapping)
	void GrabActor(AVR_GrabbableActor* ActorToGrab, bool isLeftHand);

	// Attempt to release held actor
	void ReleaseActor();

public:
	// Skeletal Mesh for the hand
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR Meshes")
	USkeletalMeshComponent* HandMesh;

	// The current actor held in the hand
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Interaction")
	AVR_GrabbableActor* HeldActor;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	USceneComponent* VROrigin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VR MotionController")
	UMotionControllerComponent* MotionController;

	// Collision sphere component for grasping/punching
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR Interaction")
	USphereComponent* SphereComponent;
};

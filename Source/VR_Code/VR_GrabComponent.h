// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "VR_GrabComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabComponentDelegate);

UENUM(BlueprintType)
enum class EVR_GrabType : uint8
{
	VR_NULL				UMETA(DisplayName = "NULL"),					// NULL, GrabType must be specified in object
	VR_FreeGrab			UMETA(DisplayName = "FreeGrab"),			// Object can be grabbed at any angle/rotation and will maintain world position
	VR_SnapToSocket		UMETA(DisplayName = "SnapToSocket"),		// Object will snap to the socket in player's hand
	VR_RestrictedGrab	UMETA(DisplayName = "RestrictedGrab"),		// Object will be restricted to some world restraint but movable along some axis (think doors, levers, etc)
};

// Forward declarations
class AVR_MotionController;

/**
 * 
 */
UCLASS(ClassGroup = "Custom", meta = (DisplayName = "Grab Component", BlueprintSpawnableComponent))
class VR_CODE_API UVR_GrabComponent : public UCapsuleComponent
{
	GENERATED_BODY()

	UVR_GrabComponent(const FObjectInitializer& ObjectInitializer);

public:
	// How the actor should be handled in regards to attachment and movement of the actor
	UPROPERTY(EditDefaultsOnly, Category = "VR Interaction")
	EVR_GrabType GrabType;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Interaction")
	bool bIsHeld;

public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void AttachComponentToSocket(USkeletalMeshComponent* motionController, FName socket);

	void UpdateHandTransform(FTransform newHandTransform);

	UFUNCTION(BlueprintCallable, Category = "VR Interaction")
	void SetCurrentGrabbedHandTransform(FTransform handTransform);

	inline void OnVRGrabbed() { bIsHeld = true; OnVRGrabed_delegate.Broadcast(); }
	inline void OnVRReleased() { bIsHeld = false; OnVRReleased_delegate.Broadcast(); }
	inline void OnVRInteract() { OnVRInteract_delegate.Broadcast(); }

	// Attach point for the Left hand (acts as a socket for the grab component)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "VR Interaction")
	FTransform AttachPointTransformForLeftHand;

	// Attach point for the Right hand (acts as a socket for the grab component)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR Interaction")
	FTransform AttachPointTransformForRightHand;	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "VR Interaction")
	FTransform LastGrabbedHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = "VR Interaction")
	FTransform CurrentGrabbedHandTransform;

private:
	UPROPERTY(BlueprintAssignable, Category = "VR Interaction")
	FGrabComponentDelegate OnVRGrabed_delegate;

	UPROPERTY(BlueprintAssignable, Category = "VR Interaction")
	FGrabComponentDelegate OnVRReleased_delegate;

	UPROPERTY(BlueprintAssignable, Category = "VR Interaction")
	FGrabComponentDelegate OnVRInteract_delegate;	
};

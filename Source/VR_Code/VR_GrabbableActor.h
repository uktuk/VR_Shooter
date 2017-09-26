// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VR_GrabbableActor.generated.h"



UENUM(BlueprintType)
enum class EVR_GrabTypeB : uint8
{
	VR_NULL				UMETA(DisplayName="NULL"),					// NULL, GrabType must be specified in object
	VR_FreeGrab			UMETA(DisplayName = "FreeGrab"),			// Object can be grabbed at any angle/rotation and will maintain world position
	VR_SnapToSocket		UMETA(DisplayName = "SnapToSocket"),		// Object will snap to the socket in player's hand
	VR_RestrictedGrab	UMETA(DisplayName = "RestrictedGrab"),		// Object will be restricted to some world restraint but movable along some axis (think doors, levers, etc)
};

UCLASS()
class VR_CODE_API AVR_GrabbableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVR_GrabbableActor();

	// How the actor should be handled in regards to attachment and movement of the actor
	UPROPERTY(EditDefaultsOnly, Category = "VR Interaction")
	EVR_GrabTypeB GrabType;

	// Virtual function called on interaction of the actor (if not blueprint overridden, _Implementation version will be called in code)
	UFUNCTION(BlueprintNativeEvent, Category="VR Interaction")
	void OnInteract();
	virtual void OnInteract_Implementation();

	// Virtual function called upon grabbing of the actor (if not blueprint overridden, _Implementation version will be called in code)
	UFUNCTION(BlueprintNativeEvent, Category = "VR Interaction")
	void OnGrabbed();
	virtual void OnGrabbed_Implementation();

	// Virtual function called upon release of the actor (if not blueprint overridden, _Implementation version will be called in code)
	UFUNCTION(BlueprintNativeEvent, Category = "VR Interaction")
	void OnReleased();
	virtual void OnReleased_Implementation();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

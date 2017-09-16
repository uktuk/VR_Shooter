// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VR_PlayerPawn.generated.h"

class AVR_MotionController;
class UCameraComponent;

UCLASS()
class VR_CODE_API AVR_PlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVR_PlayerPawn(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Func_ONE();
	void Func_TWO();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Sets the axis value for the Left Hand's SkeletalMesh animation for gripping
	// Set from bound input axis
	void SetLeftHandGripVal(float val);

	// Sets the axis value for the Right Hand's SkeletalMesh animation for gripping
	// Set from bound input axis
	void SetRightHandGripVal(float val);

public:
	// The Left Hand (Blueprint overridden) VR_MotionController to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VR MotionController")
	TSubclassOf<AVR_MotionController> LeftMotionControllerBlueprint;

	// The Right Hand (Blueprint overridden) VR_MotionController to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR MotionController")
	TSubclassOf<AVR_MotionController> RightMotionControllerBlueprint;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Defaults")
	USceneComponent* VROrigin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VR Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR MotionController")
	AVR_MotionController* LeftVR_MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR MotionController")
	AVR_MotionController* RightVR_MotionController;
};

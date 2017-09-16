// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VR_MotionController.generated.h"

class UMotionControllerComponent;

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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR Meshes")
	USkeletalMeshComponent* HandMesh;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	USceneComponent* VROrigin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VR MotionController")
	UMotionControllerComponent* MotionController;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR MotionController")
	// float gripVal;
};

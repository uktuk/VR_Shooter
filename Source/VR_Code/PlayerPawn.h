// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UMotionControllerComponent;

UCLASS()
class VR_CODE_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Func_ONE();
	void Func_TWO();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VR Meshes")
		USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VR Meshes")
		USkeletalMeshComponent* RightHandMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR Meshes")
	UClass* HandMeshClass;

protected:
	USceneComponent* VROrigin;

	UMotionControllerComponent* LeftMotionController;
	UMotionControllerComponent* RightMotionController;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR_GrabComponent.h"
#include "VR_Gun.generated.h"

/**
 * 
 */
UCLASS()
class VR_CODE_API AVR_Gun : public AActor
{
	GENERATED_BODY()
	
public:
	AVR_Gun();

	UFUNCTION(BlueprintCallable, Category="Default")
	void Fire();
	
public:
	USceneComponent* SceneComp;

	// The class of projectile to spawn when gun is fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo")
	TSubclassOf<AActor> ProjectileClass;

	// Skeletal Mesh for the gun
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Mesh")
	USkeletalMeshComponent* SkeletalMesh;
};

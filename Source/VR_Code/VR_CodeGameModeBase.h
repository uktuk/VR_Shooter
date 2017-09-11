// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VR_CodeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VR_CODE_API AVR_CodeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	virtual void StartPlay() override;

};

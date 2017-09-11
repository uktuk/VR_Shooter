// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_CodeGameModeBase.h"
#include "Engine/Engine.h"

void AVR_CodeGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("USING THE CUSTOM GAMEMODE"));
	}
}
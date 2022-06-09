// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyPawn.h"
#include "HeroPawn.h"
#include "AutoRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AUTORPG_API AAutoRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
		
		//BAD! Causes crashing on close if referenceing this for some reason!!
	//AHeroPawn* myHero = nullptr;

};

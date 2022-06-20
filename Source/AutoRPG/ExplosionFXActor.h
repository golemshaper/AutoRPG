// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionFXActor.generated.h"

UCLASS()
class AUTORPG_API AExplosionFXActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosionFXActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
		float lifeTime = 2.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

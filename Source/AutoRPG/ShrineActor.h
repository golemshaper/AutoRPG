// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShrineActor.generated.h"

UCLASS()
class AUTORPG_API AShrineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShrineActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Stats")
		int RestorePercentage = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	bool markAsUsed = false;
	void OnReset();
	void OnShrineUsed();
//public:	
//	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

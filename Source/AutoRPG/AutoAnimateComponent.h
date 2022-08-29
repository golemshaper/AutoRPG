// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoAnimateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTORPG_API UAutoAnimateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	float timeSinceStart=0.0f;
	FVector initScale= FVector(1,1,1);
	// Sets default values for this component's properties
	UAutoAnimateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

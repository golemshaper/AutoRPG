// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAnimateComponent.h"

// Sets default values for this component's properties
UAutoAnimateComponent::UAutoAnimateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoAnimateComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* myActor = GetOwner();
	initScale = myActor->GetActorScale();
}


// Called every frame
void UAutoAnimateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	timeSinceStart += DeltaTime;
	AActor* myActor = GetOwner();
	FVector nScale = myActor->GetActorScale();
	nScale.X = (FMath::Sin(timeSinceStart * 10) * 0.015f) + initScale.X;
	nScale.Y = (FMath::Sin(timeSinceStart * 10) * 0.015f) + initScale.Y;
	nScale.Z = (FMath::Cos(timeSinceStart * 10) * 0.015f) + initScale.Z;

	myActor->SetActorScale3D(nScale);


	// ...
}


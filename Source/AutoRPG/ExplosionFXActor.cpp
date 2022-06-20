// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionFXActor.h"

// Sets default values
AExplosionFXActor::AExplosionFXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplosionFXActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosionFXActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	lifeTime -= DeltaTime;
	if (lifeTime <= 0)
	{
		Destroy();
	}
}


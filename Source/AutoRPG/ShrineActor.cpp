// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "HeroPawn.h"
#include "ShrineActor.h"

// Sets default values
AShrineActor::AShrineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AShrineActor::BeginPlay()
{
	markAsUsed = false;
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (playerPawn == nullptr)return;
	AHeroPawn* playerPawnAsHero = Cast<AHeroPawn>(playerPawn);
	if (playerPawnAsHero != nullptr)
	{
		playerPawnAsHero->shrineAry.Add(this);
	}
}

void AShrineActor::OnReset()
{
	markAsUsed = false;
	//todo: hide flag graphic
}
void AShrineActor::OnShrineUsed()
{
	markAsUsed = true;
	//todo: show a flag or something to mark the shrine as used.
}
// Called every frame
/*void AShrineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


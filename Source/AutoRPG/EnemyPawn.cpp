// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "AutoRPGGameModeBase.h"


// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	speed = 50;
	MovementDirection.X = 1;
	MovementDirection.Y = 0;
	initPosition = GetActorLocation();
	initLife = curLife;
	
	

	
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{

	//---------


	if (isInitialized == false)
	{
		//----------------------------
		//RESETTER SETUP
		/*UWorld* world = GetWorld();
		if (world != NULL)
		{
			//AAutoRPGGameModeBase* GameMode = (AAutoRPGGameModeBase*)world->GetAuthGameMode();
			if (GameMode != NULL) { 
				if (GameMode->myHero != NULL){
					GameMode->myHero->enemyActors.Add(this);
					isInitialized = true;
				}
			}
		}*/
		//----------------------------
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (playerPawn == nullptr)return;
		AHeroPawn* playerPawnAsHero = Cast<AHeroPawn>(playerPawn);
		if (playerPawnAsHero != nullptr)
		{
			playerPawnAsHero->enemyActors.Add(this);
			initPosition = GetActorLocation();
			isInitialized = true;
		}

	}


	//---------
	Super::Tick(DeltaTime);
	elapsedTime += DeltaTime;
	dirChangeCounter += DeltaTime;
	float HopVal = FMath::Sin((elapsedTime * 12));
	if (dirChangeCounter >= 0.25f)
	{
		//elapsedTime = 0;
		dirChangeCounter = 0;
		int directionX = FMath::RandRange(-1, 1);
		int directionY= FMath::RandRange(-1, 1);
		MovementDirection.X = directionX;
		MovementDirection.Y = directionY;
		
	}
	const FVector nPosition = GetActorLocation() + MovementDirection.GetSafeNormal(0.001f) * DeltaTime * speed;
	
	
	//ZAlter = FVector(0, 0, HopVal * 4.0f);  //*
	//SetActorLocation(nPosition + ZAlter, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	

	SetActorLocation(nPosition, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);

}

// Called to bind functionality to input
void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemyPawn::ResetGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("Enemy reset")));
	curLife = initLife;
	SetActive(true);
	SetActorLocation(initPosition, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}
void AEnemyPawn::DamageMe(int dmgAmount)
{
	curLife -= dmgAmount;
	if (curLife <= 0)
	{
		SetActive(false);
	}
}

void AEnemyPawn::SetActive(bool isActive)
{
	activeSelf = isActive;
	SetActorHiddenInGame(!activeSelf);
	SetActorEnableCollision(activeSelf);
	SetActorTickEnabled(activeSelf);
	
}

bool AEnemyPawn::IsActive()
{
	return activeSelf;
}

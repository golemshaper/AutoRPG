// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPawn.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "EnemyPawn.h"
#include "AutoRPGGameModeBase.h"
// Sets default values
AHeroPawn::AHeroPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/Components/
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	//SphereComponent->OnComponentHit.AddDynamic(this, &AHeroPawn::OnCompHit);
	SphereComponent->OnComponentHit.AddDynamic(this, &AHeroPawn::OnCompHit);
	resetLimitOnce = false;
	
	//..
	//UCameraComponent* CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CAM"));



}

// Called when the game starts or when spawned
void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();
	
	initLifeValue = maxLife;
	initPosition = GetActorLocation();
	
	//this is the source of the crash I think
	//this is the source of the crash I think
	//this is the source of the crash I think
	//this is the source of the crash I think
	/*UWorld* world = GetWorld();
	if (world != nullptr)
	{
		AAutoRPGGameModeBase* GameMode = (AAutoRPGGameModeBase*)world->GetAuthGameMode();
		if (GameMode != NULL)GameMode->myHero = this;
	}*/
	//this is the source of the crash I think
	//this is the source of the crash I think
	//this is the source of the crash I think
	//this is the source of the crash I think


/*
https://stackoverflow.com/questions/57095288/how-do-i-get-ucameracomponent-from-acharacter-aplayercontroller
TArray<UCameraComponent> CameraComps;
	GetComponents<UCameraComponent>(CameraComps);
	
	*/


}

// Called every frame
void AHeroPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timeSinceStart += DeltaTime;
	//Premovement stop:
	if (screenLoc.X < 0)
	{
		MovementDirection.X = 0;
	}
	if (screenLoc.X > 1)
	{
		MovementDirection.X = 0;
	}
	if (screenLoc.Y < 0)
	{
		MovementDirection.Y = 0;
	}
	if (screenLoc.Y > 1)
	{
		MovementDirection.Y = 0;
	}

	//Movement:
	const FVector previousLoc = GetActorLocation();
	BounceVector.Z = 0;
	
	FVector nPosition = GetActorLocation() + (MovementDirection.GetSafeNormal(0.001f)+BounceVector)* DeltaTime * speed;
	float HopVal = FMath::Sin((timeSinceStart * 30));
	BounceVector = FMath::Lerp(BounceVector, BounceVector*0.9f,0.5f);
	
	//const FVector zAlter = FVector(0, 0, FMath::Sin((timeSinceStart * 30) * MovementDirection.Length()) * 4.0f);  //*
	const FVector zAlter = FVector(0,0, HopVal * MovementDirection.Length())*4.0f;  //*

	if (MovementDirection.Length() <= 0.01f)
	{
		nPosition.Z = initPosition.Z;
	}

	SetActorLocation(nPosition+ zAlter, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->ProjectWorldLocationToScreen(nPosition, screenLoc, true);


	//Screen Collision:
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	screenLoc.X = screenLoc.X / ViewportSize.X;
	screenLoc.Y = screenLoc.Y / ViewportSize.Y;

	/*if (screenLoc.X < 0)
	{
		SetActorLocation(previousLoc, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	}
	if (screenLoc.X > 1)
	{
		SetActorLocation(previousLoc, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	}
	if (screenLoc.Y < 0)
	{

		SetActorLocation(previousLoc, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	}
	if (screenLoc.Y > 1)
	{
		SetActorLocation(previousLoc, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	}*/
	
	if (curLife <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString(TEXT("HERO LIFE")) + FString::FromInt(curLife));

		SetActive(false);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Location is: %s"), *screenLoc.ToString()));
}

// Called to bind functionality to input
void AHeroPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(TEXT("Vertical"), this, &AHeroPawn::MoveForward);
	InputComponent->BindAxis(TEXT("Horizontal"), this, &AHeroPawn::MoveRight);
	InputComponent->BindAxis(TEXT("ResetGame"), this, &AHeroPawn::ResetGame);
}
//axis is all Effed up until I get reletive to camera
void AHeroPawn::MoveForward(float moveY)
{

	//MovementDir.X = moveY;
	MovementDirection.X = -moveY;
}

void AHeroPawn::MoveRight(float moveX)
{
	//MovementDir.Y = moveX;
	MovementDirection.Y = -moveX;
}

void AHeroPawn::ResetGame(float whatever)
{
	if (whatever > 0)
	{
		if (resetLimitOnce)return;
		maxLife = initLifeValue;
		curLife = maxLife;
		SetActorLocation(initPosition, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
		SetActive(true);	

		
		if (enemyActors.IsEmpty() == false)
		{
			for (auto& enemy : enemyActors)
			{
				if (enemy == nullptr)continue;
				enemy->ResetGame();
			}
			return;
		}
		

		//todo: send message to all enemies 
		resetLimitOnce = true;
	}
	else
	{
		resetLimitOnce = false;
	}
	
}


void AHeroPawn::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//(this->GetLifeSpan()-5)
	AEnemyPawn* enemyITouched = Cast<AEnemyPawn>(OtherActor);
	
	
	if (enemyITouched!=NULL)
	{
		int enemyLife = enemyITouched->curLife;
		//int heroLife = self->curLife;
		//found an enemy to attack!
		enemyITouched->DamageMe(this->atk);

		this->curLife -= enemyITouched->atk;
		this->BounceVector = this->GetActorLocation() - enemyITouched->GetActorLocation();
		this->BounceVector.Normalize();
		this->BounceVector = this->BounceVector * 2.0f;
		if (enemyITouched->curLife <= 0)
		{
			this->LevelUp(enemyITouched->rewardHP);
		}


		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString(TEXT("ENEMY LIFE")) + FString::FromInt(enemyLife));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString(TEXT("HERO LIFE"))+FString::FromInt(this->curLife));

	}
}

void AHeroPawn::LevelUp(int rewardHp)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("level up")));

	maxLife += rewardHp;
	curLife += rewardHp;
	atk += FMath::RandRange(1, 3);
}

void AHeroPawn::SetActive(bool isActive)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString(TEXT("ACTIVE STATUS CHANGED")));

	activeSelf = isActive;
	SetActorHiddenInGame(!activeSelf);
	SetActorEnableCollision(activeSelf);
	SetActorTickEnabled(activeSelf);
}

bool AHeroPawn::IsActive()
{
	return activeSelf;
}




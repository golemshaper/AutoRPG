// Fill out your copyright notice in the Description page of Project Settings.
#include "HeroPawn.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "DamageDisplayActor.h"
#include "EnemyPawn.h"
#include "ShrineActor.h"
#include "AutoRPGGameModeBase.h"

// Sets default values
AHeroPawn::AHeroPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->OnComponentHit.AddDynamic(this, &AHeroPawn::OnCompHit);
	resetLimitOnce = false;
}

// Called when the game starts or when spawned
void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();
	initLifeValue = maxLife;
	initPosition = GetActorLocation();
}

// Called every frame
void AHeroPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timeSinceStart += DeltaTime;
	DrainHPOverTime(DeltaTime);
	MovmentUpdate(DeltaTime);
	
	if (curLife <= 0)
	{
		curLife = 0;
		SetActive(false);
		if (SpawnOnDeath)
		{
			UWorld* world = GetWorld();
			if (world)
			{
				world->SpawnActor<AExplosionFXActor>(SpawnOnDeath, this->GetActorLocation(), this->GetActorRotation());
			}
		}
	}
}
void AHeroPawn::MovmentUpdate(float DeltaTime)
{
	//Movement:
	const FVector previousLoc = GetActorLocation();
	BounceVector.Z = 0;
	//Rotation
	RotationUpdate(DeltaTime);

	FVector nPosition = GetActorLocation() + (MovementDirection.GetSafeNormal(0.001f) + BounceVector) * DeltaTime * speed;
	//hop over time using sine
	float HopVal = FMath::Sin((timeSinceStart * 30)) * 0.5f;
	//bounce off of enemies with this vector. Cut vector size over time.
	BounceVector = FMath::Lerp(BounceVector, BounceVector * 0.9f, 0.5f);

	const FVector zAlter = FVector(0, 0, HopVal * MovementDirection.Length()) * 4.0f;  //*

	if (MovementDirection.Length() <= 0.01f)
	{
		nPosition.Z = initPosition.Z;
	}
	SetActorLocation(nPosition + zAlter, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->ProjectWorldLocationToScreen(nPosition, screenLoc, true);
}
void AHeroPawn::RotationUpdate(float DeltaTime)
{
	float stickX = MovementDirection.X;
	float stickY = MovementDirection.Y;
	FVector stickVector = FVector(stickX, stickY, 0);
	RotationVectorOverTime = FMath::Lerp(RotationVectorOverTime, stickVector, 4 * DeltaTime);
	FQuat lastRotation = RotationVectorOverTime.Rotation().Quaternion();
	if (lastRotation.Size() > 0.01f)
	{
		SetActorRotation(lastRotation, ETeleportType::TeleportPhysics);
	}
}

void AHeroPawn::DrainHPOverTime(float DeltaTime)
{
	deathTimer += DeltaTime;
	//die over time to keep the player moving
	if (deathTimer > 1)
	{
		deathTimer = 0;
		curLife--;
		if (curLife <= 0)
		{
			curLife = 0;
			SetActive(false);
			UWorld* world = GetWorld();
			if (world)
			{
				world->SpawnActor<AExplosionFXActor>(SpawnOnDeath, this->GetActorLocation(), this->GetActorRotation());
			}
		}
	}
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
	MovementDirection.X = -moveY;
}

void AHeroPawn::MoveRight(float moveX)
{
	MovementDirection.Y = -moveX;
}

void AHeroPawn::ResetGame(float whatever)
{
	if (whatever > 0)
	{
		if (resetLimitOnce)return;
		//Shrines
		for (auto& shrine : shrineAry)
		{
			if (shrine == nullptr)continue;
			//reset all registered enemies
			shrine->OnReset();
		}
		//Player Stats
		maxLife = initLifeValue;
		curLife = maxLife;
		atk = 1;
		SetActorLocation(initPosition, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
		SetActive(true);
		//Enemies
		if (enemyActors.IsEmpty() == false)
		{
			for (auto& enemy : enemyActors)
			{
				if (enemy == nullptr)continue;
				//reset all registered enemies
				enemy->ResetGame();
			}
			return;
		}
		resetLimitOnce = true;
	}
	else
	{
		resetLimitOnce = false;
	}
	
}

//Collision
void AHeroPawn::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AEnemyPawn* enemyITouched = Cast<AEnemyPawn>(OtherActor);
	if (enemyITouched!=NULL)
	{
		BattleEnemy(enemyITouched);
		return;
	}
	AShrineActor* shrineTouched = Cast<AShrineActor>(OtherActor);
	if (shrineTouched != NULL)
	{
		if (shrineTouched->markAsUsed)
		{
			//shrine can only be used once.
			return;
		}
		int restoreAmount = shrineTouched->RestorePercentage;
		shrineTouched->OnShrineUsed();
		this->curLife = (restoreAmount * this->maxLife)/100;
	}
}
void AHeroPawn::BattleEnemy(AEnemyPawn* enemyITouched)
{
	//Required variables
	const FRotator spawn_rotation = FRotator(0,0,0);
	FVector enemy_origin;
	FVector enemy_bounds;
	enemy_bounds.X = 0;
	enemy_bounds.Y = 0;

	//Damage hand out.
	enemyITouched->DamageMe(this->atk);
	this->curLife -= enemyITouched->atk;

	//Bounce the hero
	this->BounceVector = this->GetActorLocation() - enemyITouched->GetActorLocation();
	this->BounceVector.Normalize();
	this->BounceVector = this->BounceVector * 2.0f;

	//Hurt player display
	auto playerDamage = GetWorld()->SpawnActor<ADamageDisplayActor>(ADamageDisplayActor::StaticClass(), this->GetActorLocation(), spawn_rotation);
	playerDamage->SetDamageValue(enemyITouched->atk);

	//Get enemy bounds for different size monsters
	enemyITouched->GetActorBounds(false, enemy_origin, enemy_bounds, false);

	//Hurt enemy display
	auto enemyDamage = GetWorld()->SpawnActor<ADamageDisplayActor>(ADamageDisplayActor::StaticClass(), enemyITouched->GetActorLocation() + enemy_bounds, spawn_rotation);
	enemyDamage->SetDamageValue(this->atk);


	//Level up if the enemy is defeated
	if (enemyITouched->curLife <= 0)
	{
		//death fx
		if (SpawnOnDeath)
		{
			UWorld* world = GetWorld();
			if (world)
			{
				world->SpawnActor<AExplosionFXActor>(SpawnOnDeath, enemyITouched->GetActorLocation(), spawn_rotation);
			}
		}
		//level up
		this->LevelUp(enemyITouched->rewardHP);
		//level up fx
		if (SpawnOnLevelUp)
		{
			UWorld* world = GetWorld();
			if (world)
			{
				world->SpawnActor<AExplosionFXActor>(SpawnOnLevelUp, this->GetActorLocation(), spawn_rotation);
			}
		}
	}
}	

FString AHeroPawn::GetStatsAsString()
{
	FString result = "HP:";
	result.Append(FString::FromInt(curLife));
	result.Append("/");
	result.Append(FString::FromInt(maxLife));
	result.Append("\n");
	result.Append("ATK:");
	result.Append(FString::FromInt(atk));
	return result;
}

void AHeroPawn::LevelUp(int rewardHp)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("level up")));
	maxLife += rewardHp;
	curLife += rewardHp;
	atk += FMath::RandRange(1, 3);
}

void AHeroPawn::SetActive(bool isActive)
{
	activeSelf = isActive;
	SetActorHiddenInGame(!activeSelf);
	SetActorEnableCollision(activeSelf);
	SetActorTickEnabled(activeSelf);
}

bool AHeroPawn::IsActive()
{
	return activeSelf;
}




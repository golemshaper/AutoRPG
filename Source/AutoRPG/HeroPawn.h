// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawn.h"
#include "HeroPawn.generated.h"

UCLASS()
class AUTORPG_API AHeroPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHeroPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Stats")
		int curLife = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Stats")
		int maxLife = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Stats")
		int atk = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Stats")

		
	

	float speed = 500;
	int initLifeValue = 50;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool activeSelf;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	void MoveForward(float moveY);
	void MoveRight(float moveX);
	void ResetGame(float whatever);
	void SetActive(bool isActive);
	void LevelUp(int rewardHp);
	bool IsActive();

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	TArray<AEnemyPawn*> enemyActors;
	UFUNCTION(BlueprintCallable, Category = "Hero Stats")
		FString GetStatsAsString();

private:
	UPROPERTY()//probably don't need these to be UPROPERTY anymore. I was seeing if it was the cause of a crash (it wasn't)
	FVector initPosition;
	UPROPERTY()
	FVector MovementDirection;
	UPROPERTY()
	FVector BounceVector;

	FVector2D screenLoc;
	double timeSinceStart = 0;
	int32 sizeX = 0;
	int32 sizeY = 0;
	bool resetLimitOnce;
	float deathTimer = 0;

};

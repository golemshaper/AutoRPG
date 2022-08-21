// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawn.generated.h"


UCLASS()
class AUTORPG_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int curLife=10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int atk = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int rewardHP = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float skitishness = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float speed =50.0f;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY()
	FVector MovementDirection;
	UPROPERTY()
	FVector ZAlter;
	
	float dirChangeCounter;
	double elapsedTime;
	UPROPERTY()
	int initLife=0;
	UPROPERTY()
	bool isInitialized = false;
	void DamageMe(int dmgAmount);
	//probably needn't be virtual...
	void SetActive(bool isActive);
	bool IsActive();
	void ResetGame();
	UPROPERTY()
		bool activeSelf;
private:
	FVector initPosition;
};

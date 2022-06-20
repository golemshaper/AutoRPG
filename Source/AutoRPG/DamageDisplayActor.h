// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "DamageDisplayActor.generated.h"

UCLASS()
class AUTORPG_API ADamageDisplayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UTextRenderComponent* textReference = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Display")
	UTextRenderComponent* textTEST;
	// Sets default values for this actor's properties
	ADamageDisplayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float timer = 0.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Game Display")
	void SetDamageValue(int dmg);
	FVector offset;
	void SetActive(bool isActive);
	bool IsActive();
	UPROPERTY()
		bool activeSelf;
};

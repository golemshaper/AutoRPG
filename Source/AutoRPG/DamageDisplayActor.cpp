// Fill out your copyright notice in the Description page of Project Settings.
#include "DamageDisplayActor.h"
#include "Components/TextRenderComponent.h"
// Sets default values
ADamageDisplayActor::ADamageDisplayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	//RootComponent = SphereComponent;

	UTextRenderComponent* textComponent = CreateAbstractDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	textReference = textComponent;
	
}

// Called when the game starts or when spawned
void ADamageDisplayActor::BeginPlay()
{
	Super::BeginPlay();
	SetDamageValue(32);
}

// Called every frame
void ADamageDisplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer += DeltaTime;
	if (timer >= 1.0f)
	{
		timer = 0.0f;
		Destroy();
		//SetActive(false);
	}
}

void ADamageDisplayActor::SetDamageValue(int dmg)
{
	if (textReference == nullptr)return;
	SetActive(true);
	textReference->SetText(FText::FromString(FString::FromInt(dmg)));
}


void ADamageDisplayActor::SetActive(bool isActive)
{
	activeSelf = isActive;
	SetActorHiddenInGame(!activeSelf);
	SetActorEnableCollision(activeSelf);
	SetActorTickEnabled(activeSelf);
}

bool ADamageDisplayActor::IsActive()
{
	return activeSelf;
}

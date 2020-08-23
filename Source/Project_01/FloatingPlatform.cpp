// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
	: StartPoint{ FVector(0.f) }, EndPoint{ FVector(0.f) }, InterpSpeed{ 4.f }, WaitTime{ 1.f }, bIsMoving{ false } {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	bIsMoving = false;
	//GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleMovement, WaitTime);
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleMovement, WaitTime);

}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving) {
		// Move the platform towards the End Point
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);

		// Check if the platform has reached the End Point
		if ((GetActorLocation() - EndPoint).Size() < 0.5f) {
			ToggleMovement();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleMovement, WaitTime);
			SwapElements(StartPoint, EndPoint);
		}

	}

}

void AFloatingPlatform::ToggleMovement() {
	bIsMoving = !bIsMoving;
}

template <typename T>
void AFloatingPlatform::SwapElements(T& Element1, T& Element2) {
	T Temp{ Element1 };
	Element1 = Element2;
	Element2 = Temp;
}
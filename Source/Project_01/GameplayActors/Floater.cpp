// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"

// Sets default values
AFloater::AFloater()
	: InitialLocation{ FVector(0.0f) }, 
	  PlacedLocation{ FVector(0.0f) },
	  WorldOrigin{ FVector(0.0f) },
	  InitialDirection{ FVector(0.0f) },
	  bInitializerFloaterLocations{ false },
	  bShouldFloat{ false },
	  InitialForce{ FVector(2000000.0f, 0.0f, 0.0f) },
	  InitialTorque{ FVector(2000000.0f, 0.0f, 0.0f) },
	  RunningTime{ 0.0f },
	  Amplitude{ 0.0f },
	  Period{ 0.0f },
	  PhaseShift{ 0.0f },
	  VerticalShift{ 0.0f } {

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh")); 

}

// Called when the game starts or when spawned
void AFloater::BeginPlay() {
	Super::BeginPlay();

	//float InitialX = FMath::FRand();
	//float InitialY = FMath::FRand();
	//float InitialZ = FMath::FRand();
	float InitialX = FMath::FRandRange(-500.0f, 500.0f);
	float InitialY = FMath::FRandRange(-500.0f, 500.0f);
	float InitialZ = FMath::FRandRange(-500.0f, 500.0f);
	
	InitialLocation.X = InitialX;
	InitialLocation.Y = InitialY;
	InitialLocation.Z = InitialZ;

	//InitialLocation *= 1000.0f;

	PlacedLocation = GetActorLocation();

	if (bInitializerFloaterLocations) {
		SetActorLocation(InitialLocation);
	}

	BaseZLocation = PlacedLocation.Z;

	//FHitResult HitResult{};
	//FVector LocalOffset = FVector(200.0f, 0.0f, 0.0f);
	//AddActorLocalOffset(LocalOffset, true, &HitResult);
	//AddActorWorldOffset(LocalOffset, true, &HitResult);
	
	/*StaticMesh->AddForce(InitialForce);
	StaticMesh->AddTorque(InitialTorque);*/
}

// Called every frame
void AFloater::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bShouldFloat) {
		//FHitResult HitResult{};

		//// If bSweep is true, the collisions will work even if physics is not being simulated
		//AddActorLocalOffset(InitialDirection, true, &HitResult);

		//FVector HitLocation = HitResult.Location;

		FVector NewLocation = GetActorLocation();

		NewLocation.Z = BaseZLocation + Amplitude * FMath::Sin((1/Period) * RunningTime - PhaseShift) + VerticalShift;

		SetActorLocation(NewLocation);
		RunningTime += DeltaTime;

	}

	//FRotator Rotation = FRotator(0.0f, 1.0f, 0.0f);
	//AddActorLocalRotation(Rotation);
	////AddActorWorldRotation(Rotation);
}


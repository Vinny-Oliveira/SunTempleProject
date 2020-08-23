// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	//AddActorToArray(Actor_1);
	//AddActorToArray(Actor_2);
	//AddActorToArray(Actor_3);
	//AddActorToArray(Actor_4);
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ASpawnVolume::AddActorToArray(TSubclassOf<AActor>& MyActor) {
//	if (MyActor) {
//		SpawnArray.Add(MyActor);
//	}
//}

FVector ASpawnVolume::GetSpawnPoint() {
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return Point;
}

void ASpawnVolume::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location) {
	if (ToSpawn) {
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		// Spawn the Actor
		if (World) {
			AActor* SpawnedActor{ World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams) };

			// If the Actor is an Enemy, put a controller on it
			if (AEnemy* Enemy{ Cast<AEnemy>(SpawnedActor) }) {
				Enemy->SpawnDefaultController();
				Enemy->PlaceAIController();
			}
		}
	}
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor() {
	if (SpawnArray.Num() > 0) {
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);
		return SpawnArray[Selection];
	
	} else {
		return nullptr;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class PROJECT_01_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	class UBoxComponent* SpawningBox{ nullptr };

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	//TSubclassOf<class AActor> Actor_1{};
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	//TSubclassOf<class AActor> Actor_2{};
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	//TSubclassOf<class AActor> Actor_3{};
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	//TSubclassOf<class AActor> Actor_4{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<TSubclassOf<AActor>> SpawnArray{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void AddActorToArray(TSubclassOf<AActor>& MyActor);

	/// <summary>
	/// Return a random point in the spawn volume
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetSpawnPoint();
	
	/// <summary>
	/// Return a random Actor of the Actor Array
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "Spawning")
	TSubclassOf<AActor> GetSpawnActor();

	/// <summary>
	/// Event to be implemented in Blueprints
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
	void SpawnOurActor(UClass* ToSpawn, const FVector& Location);
};

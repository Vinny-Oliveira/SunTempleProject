// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStorage.generated.h"

UCLASS()
class PROJECT_01_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// ItemStorage does not need to Tick
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

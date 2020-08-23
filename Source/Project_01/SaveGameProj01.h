// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameProj01.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats {

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Health{};
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHealth{};
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Stamina{};
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina{};
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Coins{};
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location{};
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation{};

};

/**
 * 
 */
UCLASS()
class PROJECT_01_API USaveGameProj01 : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveGameProj01();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName{};

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex{};

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats{};

};

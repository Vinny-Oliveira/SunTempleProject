// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_01_API AExplosive : public AItem
{
	GENERATED_BODY()
	
public:

	AExplosive();

	/** Damage dealt by explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass{};

	/** Event to detect when the switch is stepped on */
	// UFUNCTION() - No need to use UFUNCTION macro, because the parent function is already a UFUNCTION
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/** Event to detect when the switch is no longer stepped on */
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


};

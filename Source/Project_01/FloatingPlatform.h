// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class PROJECT_01_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	/** Mesh for the platform */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	UStaticMeshComponent* Mesh{ nullptr };

	/** Where the platform starts its movement */
	UPROPERTY(EditAnywhere)
	FVector StartPoint;
	
	/** Where the platform ends its movement */
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector EndPoint;

	/** Interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpSpeed{};
	
	/** Time the platform will stay still before it starts moving again */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float WaitTime{};

	/** Timer handle for the interpolation */
	FTimerHandle InterpTimer;

	/** Whether the platform is done interping on not */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bIsMoving{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Toggle the value of bIsInterping
	/// </summary>
	void ToggleMovement();

	/// <summary>
	/// Swap 2 elements of the same type T
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="Element1"></param>
	/// <param name="Element2"></param>
	template <typename T>
	void SwapElements(T& Element1, T& Element2);
};

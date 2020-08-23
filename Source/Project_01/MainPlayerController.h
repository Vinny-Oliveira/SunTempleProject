// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_01_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the HEUDOverlay widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay{ nullptr };

	/** Reference to the enemy health bar UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WEnemyHealthBar;
	
	/** Variable to hold the enemy health bar widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar{ nullptr };

	/** Check if the enemy health bar is to be displayed or not */
	bool bEnemyHealthBarVisible{};

	FVector EnemyLocation{};

protected:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

public:

	void DisplayEnemyHealthBar();

	void HideEnemyHealthBar();
};

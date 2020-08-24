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

#pragma region PLAYER_HUD
	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the HEUDOverlay widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay{ nullptr };

#pragma endregion


#pragma region ENEMY_HEALTH_BAR
	/** Reference to the enemy health bar UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WEnemyHealthBar;
	
	/** Variable to hold the enemy health bar widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar{ nullptr };
	
	/** Check if the enemy health bar is to be displayed or not */
	bool bEnemyHealthBarVisible{};

	FVector EnemyLocation{};

#pragma endregion


#pragma region PAUSE_MENU
	/** Reference to the enemy health bar UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;
	
	/** Variable to hold the enemy health bar widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu{ nullptr };

	/** Check if the Pause Menu is to be displayed or not */
	bool bPauseMenuVisible{};

#pragma endregion


protected:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// Create the UserWidget, add it to viewport and hide it
	/// </summary>
	/// <param name="Widget"></param>
	/// <param name="UserWidget"></param>
	void CreateAndHideWidget(TSubclassOf<UUserWidget>& Widget, UUserWidget* UserWidget);

	void DisplayWidget(UUserWidget* Widget, bool& isVisible);

	void HideWidget(UUserWidget* Widget, bool& isVisible);
public:

	void DisplayEnemyHealthBar();

	void HideEnemyHealthBar();
	
	void DisplayPauseMenu();

	void HidePauseMenu();

};

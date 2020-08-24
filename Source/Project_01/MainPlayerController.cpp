// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h" 

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	// Main Character's HUD overlay
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	// Enemy health bar
	if (WEnemyHealthBar) {
		CreateAndHideWidget(WEnemyHealthBar, EnemyHealthBar);

		// Make the health bar flat on the screen
		FVector2D Allignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Allignment);
	}
	
	// Pause Menu
	if (WPauseMenu) {
		CreateAndHideWidget(WPauseMenu, PauseMenu);
	}

}

void AMainPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Place the enemy health bar on the proper place
	if (EnemyHealthBar) {
		FVector2D PositionInViewport{};
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 85.f;

		FVector2D SizeInViewport(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::CreateAndHideWidget(TSubclassOf<UUserWidget>& Widget, UUserWidget* UserWidget) {
	UserWidget = CreateWidget<UUserWidget>(this, Widget);
	if (UserWidget) {
		UserWidget->AddToViewport();
		UserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayWidget(UUserWidget* Widget, bool& isVisible) {
	if (Widget) {
		isVisible = true;
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HideWidget(UUserWidget* Widget, bool& isVisible) {
	if (Widget) {
		isVisible = false;
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayEnemyHealthBar() {
	DisplayWidget(EnemyHealthBar, bEnemyHealthBarVisible);
}

void AMainPlayerController::HideEnemyHealthBar() {
	HideWidget(PauseMenu, bEnemyHealthBarVisible);
}

void AMainPlayerController::DisplayPauseMenu() {
	DisplayWidget(PauseMenu, bPauseMenuVisible);
}

void AMainPlayerController::HidePauseMenu() {
	HideWidget(PauseMenu, bPauseMenuVisible);
}
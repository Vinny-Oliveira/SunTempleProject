// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Main.h"
#include "GameUtilities.h"

void UMainAnimInstance::CheckForPawn() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Main = Cast<AMain>(Pawn);
		}
	}
}

void UMainAnimInstance::NativeInitializeAnimation() {
	UMainAnimInstance::CheckForPawn();
}

void UMainAnimInstance::UpdateAnimationProperties() {
	UMainAnimInstance::CheckForPawn();

	if (Pawn) {
		// Get the magnitude of speed on the XY plane
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		// Check if the Pawn in in air
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}
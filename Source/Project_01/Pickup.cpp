// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Main.h"

APickup::APickup() {

}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AMain* Main{ GetValidCharacter(OtherActor) }) {
		OnPickupBP(Main);
		EmitParticles();
		PlaySound();
		Main->PickupLocations.Add(GetActorLocation());
		Destroy();
	}

}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

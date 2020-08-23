// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Main.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AExplosive::AExplosive()
	: Damage{ 15.f } {

}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AMain* Main{ GetValidCharacter(OtherActor) };
	AEnemy* Enemy{ GetValidEnemy(OtherActor) };

	if (Main || Enemy) {
		EmitParticles();
		PlaySound();
	//	Main->DecrementHealth(Damage);
	//	Destroy();
	//
	//} else if (AEnemy* Enemy{ GetValidEnemy(OtherActor) }) {
	//	EmitParticles();
	//	PlaySound();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
		//Enemy->DecrementHealth(Damage);
		Destroy();
	}
	
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

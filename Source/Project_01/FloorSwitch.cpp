// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
	: SwitchTime{ 2.f }, bCharacterOnSwitch{ false } {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// This ensures the trigger box is only to be stepped onto. There is no physics involved
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	// Ignore everything that steps on the trigger box, except the pawn
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind the trigger box to its functions
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));
	if (!bCharacterOnSwitch) {
		bCharacterOnSwitch = true;
	}
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP END"));
	if (bCharacterOnSwitch) {
		bCharacterOnSwitch = false;
	}
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}

void AFloorSwitch::UpdateComponentLocation(UStaticMeshComponent* Component, FVector InitialLocation, float Z) {
	FVector NewLocation = InitialLocation;
	NewLocation.Z += Z;
	Component->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateDoorLocation(float Z) {
	AFloorSwitch::UpdateComponentLocation(Door, InitialDoorLocation, Z);
}

void AFloorSwitch::UpdateFloorSwitchLocation(float Z) {
	AFloorSwitch::UpdateComponentLocation(FloorSwitch, InitialSwitchLocation, Z);
}

void AFloorSwitch::CloseDoor() {
	if (!bCharacterOnSwitch) {
		LowerDoor();
		RaiseFloorSwitch();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "SaveGameProj01.h"
#include "ItemStorage.h"

// Sets default values
AMain::AMain()
	: BaseTurnRate{ 65.f },
	BaseLookUpRate{ 65.f },
	MaxHealth{ 100.f },
	Health{ 65.f },
	MaxStamina{ 150.f },
	Stamina{ 120.f },
	Coins{ 0 },
	RunningSpeed{ 650.f },
	SprintingSpeed{ 950.f },
	bShiftKeyDown{ false },
	bLeftMouseBtnDown{ false },
	bEscDown{ false },
	bIsAttacking{ false },
	MovementStatus{ EMovementStatus::EMS_Normal },
	StaminaStatus{ EStaminaStatus::ESS_Normal },
	StaminaDrainRate{ 25.f },
	MinSprintStamina{ 50.f },
	InterpSpeed{ 15.f },
	bInterpingToEnemy{ false } {

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // Camera fllows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller

	// Set size for collision capsule
	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Do not rotate this camera

	// Do not rotate when the controller rotates. That should just affect the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // Rotation rate for the character movement
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
	MainPlayerController = Cast<AMainPlayerController>(GetController());

	LoadGame();
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleStaminaStatus(DeltaTime);

	InterpToEnemy(DeltaTime);

	HandleCombatTargetLocation();
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // This macro stops the function execution if the PlayerInputComponent is not valid

	// Bind movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	
	// Bind turns
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind turn rates
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

	// Bind jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);
	
	// Bind left mouse button
	PlayerInputComponent->BindAction("LeftMouseBtn", IE_Pressed, this, &AMain::LeftMouseBtnDown);
	PlayerInputComponent->BindAction("LeftMouseBtn", IE_Released, this, &AMain::LeftMouseBtnUp);

	// Bind Escape and make sure you can use this input when the game is paused
	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::EscDown).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::EscUp).bExecuteWhenPaused = true;

}

void AMain::MoveInDirection(float Value, EAxis::Type Axis) {
	if ((Controller != nullptr) && (Value != 0.f) && (!bIsAttacking) && (IsAlive())) {
		// Find out which way is forward (where the camera is pointing to)
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// Move in that direction
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(Axis);
		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveForward(float Value) {
	AMain::MoveInDirection(Value, EAxis::X);
}

void AMain::MoveRight(float Value) {
	AMain::MoveInDirection(Value, EAxis::Y);
}

void AMain::TurnAtRate(float Rate) {
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate) {
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::Die() {
	UAnimInstance* AnimInstance{ GetMesh()->GetAnimInstance() };
	// Play death animation
	if (AnimInstance && CombatMontage && IsAlive()) {
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}

	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::DeathEnd() {

	// Freeze position and animation
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMain::DecrementHealth(float Amount) {
	if (!IsAlive()) {
		return;
	}
	
	Health -= Amount;

	// Check if the character dies
	if (Health <= 0.f) {
		Health = 0.f;
		Die();
	}
}

void AMain::IncrementCoins(int32 Amount) {
	Coins += Amount;
}

void AMain::IncrementHealth(float Amount) {
	(Health + Amount > MaxHealth) ? (Health = MaxHealth) : (Health += Amount);
}

void AMain::SetMovementStatus(EMovementStatus Status) {
	MovementStatus = Status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	} else if (MovementStatus == EMovementStatus::EMS_Dead) {
		GetCharacterMovement()->MaxWalkSpeed = 0;
	} else {
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::ShiftKeyDown() {
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp() {
	bShiftKeyDown = false;
}

void AMain::HandleStaminaStatus(float DeltaTime) {
	if (!IsAlive()) {
		return;
	}

	float DeltaStamina{ StaminaDrainRate * DeltaTime };
	float CurrentSpeed{ GetVelocity().Size() };
	bool bIsInAir{ GetMovementComponent()->IsFalling() };

	switch (StaminaStatus) {
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown && (CurrentSpeed > 0.f) && !bIsInAir) { // Decrease stamina and sprint
			Stamina -= DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Sprinting);

			// Threshhold to change color
			if (Stamina < MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}

		} else { // Replenish stamina and do not sprint
			Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
			if (Stamina > MaxStamina) {
				Stamina = MaxStamina;
			}
		}
		break;

	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown && (CurrentSpeed > 0.f) && !bIsInAir) { // Decrease stamina and sprint
			Stamina -= DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Sprinting);

			// Character exhausted
			if (Stamina < 0.f) {
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				SetMovementStatus(EMovementStatus::EMS_Normal);
				Stamina = 0.f;
			}

		} else { // Replenish stamina and do not sprint
			Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
			if (Stamina > MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}
		}
		break;

	case EStaminaStatus::ESS_Exhausted:
		// Do not sprint, zero out the Stamina if shift is pressed, and go to recovery state if shift key is let go
		SetMovementStatus(EMovementStatus::EMS_Normal);
		if (bShiftKeyDown && (CurrentSpeed > 0.f) && !bIsInAir) {
			Stamina = 0.f;
		} else {
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
		}
		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:
		// Replenish stamina and get back to normal if past the threshold
		Stamina += DeltaStamina;
		SetMovementStatus(EMovementStatus::EMS_Normal);
		if (Stamina > MinSprintStamina) {
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		break;

	default:
		break;
	}
}

void AMain::ShowPickupLocations() {
	//for (int32 i{ 0 }; i < PickupLocations.Num(); i++) { // TArray uses Num() to collect the number of items
	for (auto Location : PickupLocations) {
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 12, FLinearColor::Red, 5.f, 2.f);
	}
	
}

void AMain::LeftMouseBtnDown() {
	bLeftMouseBtnDown = true;

	if (!IsAlive()) {
		return;
	}

	if (ActiveOverlappingItem) {
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon) {
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	} else if (EquippedWeapon) { 
		Attack();
	}
}

void AMain::LeftMouseBtnUp() {
	bLeftMouseBtnDown = false;
}

void AMain::EscDown() {
	bEscDown = true;

	if (MainPlayerController) {
		MainPlayerController->TogglePauseMenu();
	}
}

void AMain::EscUp() {
	bEscDown = false;
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet) { 
	if (EquippedWeapon) {
		EquippedWeapon->Destroy();
	}
	
	EquippedWeapon = WeaponToSet;
}

void AMain::Attack() {
	if (!IsAlive()) {
		return;
	}

	if (!bIsAttacking) { // Do not attack if an attack animation is playing
		bIsAttacking = true;
		SetInterpingToEnemy(true);

		// Play attack animation
		UAnimInstance* AnimInstance{ GetMesh()->GetAnimInstance() };
		if (AnimInstance && CombatMontage) {
			int32 Section{ FMath::RandRange(0, 1) };
			FName AnimName;

			// Choose an attack animation randomly
			switch (Section) {
			case 0:
				AnimName = FName("Attack_01");
				break;

			case 1:
				AnimName = FName("Attack_02");
				break;

			default:
				AnimName = FName("Attack_01");
				break;
			}

			AnimInstance->Montage_Play(CombatMontage, 2.f);
			AnimInstance->Montage_JumpToSection(AnimName, CombatMontage);
		}
	}
}

void AMain::AttackEnd() {
	bIsAttacking = false;
	SetInterpingToEnemy(false);
	if (bLeftMouseBtnDown) {
		Attack(); // Attack again if the button is held down
	}
}

void AMain::PlaySwingSound() {
	if (EquippedWeapon->SwingSound) {
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

FRotator AMain::GetLookAtRotationYaw(FVector TargetLocation) {
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

FRotator AMain::GetLookAtRotationYaw(AActor* Target) {
	return GetLookAtRotationYaw(Target->GetActorLocation());
}

void AMain::InterpToEnemy(float DeltaTime) {
	if (bInterpingToEnemy && CombatTarget) {
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget);
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}
}

float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	DecrementHealth(DamageAmount);

	return DamageAmount;
}

void AMain::HandleCombatTargetLocation() {
	if (MainPlayerController) {
		if (CombatTarget) {
			MainPlayerController->EnemyLocation = CombatTarget->GetActorLocation();
		}
	}
}

void AMain::Jump() {
	if (IsAlive()) {
		Super::Jump();
	}
}

void AMain::UpdateCombatTarget() {
	TArray<AActor*> OverlappingActors{};
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() > 0) {
		// Distance to the first index
		AEnemy* ClosestEnemy{ Cast<AEnemy>(OverlappingActors[0]) };
		FVector MainLocation{ GetActorLocation() };
		float MinDistance{ (ClosestEnemy->GetActorLocation() - MainLocation).Size() };

		// Check which enemy is the closest one
		for (auto Enemy : OverlappingActors) {
			float DistanceToActor{ (Enemy->GetActorLocation() - MainLocation).Size() };
			if (DistanceToActor < MinDistance) {
				MinDistance = DistanceToActor;
				ClosestEnemy = Cast<AEnemy>(Enemy);
			}
		}

		// Update Enemy
		SetCombatTarget(ClosestEnemy);
		if (MainPlayerController) {
			MainPlayerController->DisplayEnemyHealthBar();
		}

	} else {
		// Remove the enemy health bar
		CombatTarget = nullptr;
		if (MainPlayerController) {
			MainPlayerController->HideEnemyHealthBar();
		}
	}
}

void AMain::SwitchLevel(FName LevelName) {
	UWorld* World{ GetWorld() };

	// Only switch level if the transition is not to the current level
	if (World) {
		FString FS_CurrentLevel{ World->GetMapName() };
		FName FN_CurrentLevel(*FS_CurrentLevel); // Initialize the FName with a de-referenced FString, which is equivalent to a C string

		if (FN_CurrentLevel != LevelName) {
			UGameplayStatics::OpenLevel(World, LevelName);
		}

	}
}

void AMain::SaveGame() {
	// Create a pointer to a save game object
	USaveGame* SaveGame{ UGameplayStatics::CreateSaveGameObject(USaveGameProj01::StaticClass()) };
	USaveGameProj01* SaveGameInstance{ Cast<USaveGameProj01>(SaveGame) };

	// Store the player stats in the struct
	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Coins = Coins;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	// Save the equipped weapon
	if (EquippedWeapon) {
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	// Save the information to a save slot
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void AMain::LoadGame() {
	// Create a pointer to a save game object
	USaveGame* LoadGame{ UGameplayStatics::CreateSaveGameObject(USaveGameProj01::StaticClass()) };
	USaveGameProj01* LoadGameInstance{ Cast<USaveGameProj01>(LoadGame) };

	// The SaveGame object is loaded and overwrites the previous objects
	LoadGame = UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex);
	LoadGameInstance = Cast<USaveGameProj01>(LoadGame);

	// Load the stats into the player
	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Coins = LoadGameInstance->CharacterStats.Coins;
	
	// Load the character alive or dead depending on the state it was when saved
	SetMovementStatus(EMovementStatus::EMS_Normal);
	if (Health > 0) {
		GetMesh()->bPauseAnims = false;
		GetMesh()->bNoSkeletonUpdate = false;
	} else {
		Die();
	}

	// Spawn and equip the loaded weapon
	if (WeaponStorage) {
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons) {
			FString WeaponName{ LoadGameInstance->CharacterStats.WeaponName };

			// Only equip the weapon if it exists in the storage
			if (Weapons->WeaponMap.Contains(WeaponName)) {
				AWeapon* WeaponToEquip{ GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]) };
				if (WeaponToEquip) {
					WeaponToEquip->Equip(this);
				}
			}

		}
	}
}

void AMain::LoadGame(bool CanSetPosition) {
	//if (CanSetPosition) {
	//	SetActorLocation(LoadGameInstance->CharacterStats.Location);
	//	SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	//}
}
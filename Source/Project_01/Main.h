// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	EMS_Dead		UMETA(DisplayName = "Dead"),
	
	EMS_MAX			UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8 {
	ESS_Normal					UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum			UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted				UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering		UMETA(DisplayName = "ExhaustedRecovering"),
	
	ESS_MAX						UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class PROJECT_01_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

#pragma region CAMERA
	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom{ nullptr };

	/** Camera that follows the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera{ nullptr };

	/** Rate of turning speed (%) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate{};
	
	/** Rate of looking up/down speed (%) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate{};

#pragma endregion


#pragma region MOVEMENT
	/** Enum for the movement state of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus { EMovementStatus::EMS_Normal };
	
	/** Enum for the movement state of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus { EStaminaStatus::ESS_Normal };

	/** Speed for regular run */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed{};

	/** Speed for fast sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed{};

	/** Rate at which stamina is drained while the character sprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate{};
	
	/** Minimum stamina that allows sprinting after recovery */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina{};

#pragma endregion


#pragma region COMBAT

	/** Boolean to check if the character is attacking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bIsAttacking{};

	/** Reference to the montage with the combat animations */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage{ nullptr };

	/** Particles emitted by the character when it is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles{ nullptr };

	/** Sound played when the character is struck by an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* StruckSound{ nullptr };

	/** Enemy the character is fighting with */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTarget{ nullptr };

	/** Speed to interpolate the character's rotation towards the enemy */
	float InterpSpeed{};

	/** Check if the character is interpolationg towards the enemy */
	bool bInterpingToEnemy{};

	/** Player controller to see enemy health during combat */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController{ nullptr };

	/** Filter to return only Enemy classes and its derived classes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter{};

#pragma endregion


#pragma region CONTROLLER_EVENTS
	/** Boolean to check if the Shift key is being pressed or not */
	bool bShiftKeyDown{};

	/** Boolean to check if the left mouse button is being pressed */
	bool bLeftMouseBtnDown{};
	
	/** Boolean to check if the Escape key is being pressed */
	bool bEscDown{};

#pragma endregion


#pragma region ITEM_INTERACTION
	/** TArray that stores the locations of the items picked up */
	TArray<FVector> PickupLocations{};

	/** Weapon equiped to the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	class AWeapon* EquippedWeapon{ nullptr };

	/** Item the character is overlapping with */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	class AItem* ActiveOverlappingItem{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage{};

#pragma endregion


#pragma region PLAYER_STATS
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxStamina{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins{};

#pragma endregion



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Move is a specific direction based on the camera and the axis (forward/backward or left/right)
	/// </summary>
	/// <param name="Value"></param>
	/// <param name="Axis"></param>
	void MoveInDirection(float Value, EAxis::Type Axis);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region GETTERS_AND_SETTERS
	/// <summary>
	/// Set movement status and running speed
	/// </summary>
	/// <param name="Status"></param>
	void SetMovementStatus(EMovementStatus Status);

	/// <summary>
	/// Setter for the StaminaStatus
	/// </summary>
	/// <param name="Status"></param>
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	/// <summary>
	/// Getter for the EquippedWeapon
	/// </summary>
	/// <returns></returns>
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	/// <summary>
	/// Setter for the Equipped Weapon
	/// </summary>
	/// <param name="WeaponToSet"></param>
	void SetEquippedWeapon(AWeapon* WeaponToSet);

	/// <summary>
	/// Setter for the Active Overlapping Item
	/// </summary>
	/// <param name="Item"></param>
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	/// <summary>
	/// Getter for CameraBoom
	/// </summary>
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/// <summary>
	/// Getters for FollowCamera
	/// </summary>
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// Set whether or not the Character is interping to the enemy
	/// </summary>
	/// <param name="IsInterping"></param>
	FORCEINLINE void SetInterpingToEnemy(bool IsInterping) { bInterpingToEnemy = IsInterping; }

	/// <summary>
	/// Set the CombatTarget
	/// </summary>
	/// <param name="Target"></param>
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

#pragma endregion


#pragma region MOVEMENT
	/// <summary>
	/// Called for forward and backwards input
	/// </summary>
	/// <param name="value"></param>
	void MoveForward(float Value);

	/// <summary>
	/// Called for sideways input
	/// </summary>
	/// <param name="value"></param>
	void MoveRight(float Value);

	/// <summary>
	/// Called via input to turn at a given Rate
	/// </summary>
	/// <param name="Rate">This is a normalized turn rate</param>
	void TurnAtRate(float Rate);
	
	/// <summary>
	/// Called via input to look up/down at a given Rate
	/// </summary>
	/// <param name="Rate">This is a normalized look up/down rate</param>
	void LookUpAtRate(float Rate);

	/// <summary>
	/// Return the yaw rotation needed for the character to face the target
	/// </summary>
	/// <param name="Target"></param>
	/// <returns></returns>
	FRotator GetLookAtRotationYaw(FVector Target);
	FRotator GetLookAtRotationYaw(AActor* Target);

	/// <summary>
	/// Rotate the enemy towards the target using interpolation
	/// </summary>
	/// <param name="DeltaTime"></param>
	void InterpToEnemy(float DeltaTime);

	/// <summary>
	/// Add to the original jump functionality
	/// </summary>
	virtual void Jump() override;

#pragma endregion


#pragma region PLAYER_INPUT
	/// <summary>
	/// Press shift down to enable sprint
	/// </summary>
	void ShiftKeyDown();
	
	/// <summary>
	/// Release shift to stop sprint
	/// </summary>
	void ShiftKeyUp();

	/// <summary>
	/// Call actions for when the left mouse button is pressed
	/// </summary>
	void LeftMouseBtnDown();
	
	/// <summary>
	/// Call actions for when the left mouse button is released
	/// </summary>
	void LeftMouseBtnUp();
	
	/// <summary>
	/// Call actions for when the Escape key is pressed
	/// </summary>
	void EscDown();
	
	/// <summary>
	/// Call actions for when the Escape key is released
	/// </summary>
	void EscUp();

#pragma endregion


#pragma region COMBAT
	/// <summary>
	/// Decrease the health by an Amount
	/// </summary>
	/// <param name="Amount"></param>
	void DecrementHealth(float Amount);

	/// <summary>
	/// Take damage from enemies
	/// </summary>
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/// <summary>
	/// Play attack animations and deal damage
	/// </summary>
	void Attack();

	/// <summary>
	/// Trigger this function when the attack animation finishes
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/// <summary>
	/// Play the equipped weapon's swing sound
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	/// <summary>
	/// Check if the character has a combat target
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasCombatTarget() { return (CombatTarget != nullptr); }

	/// <summary>
	/// Update the Main Character's Combat Target after it kills an Enemy
	/// </summary>
	void UpdateCombatTarget();

#pragma endregion


#pragma region DEATH
	/// <summary>
	/// Trigger the death events of the mais character
	/// </summary>
	void Die();

	/// <summary>
	/// Trigger the death events after the death animation is played
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/// <summary>
	/// Returns whether the Enemy is alive or not
	/// </summary>
	/// <returns></returns>
	FORCEINLINE bool IsAlive() { return (MovementStatus != EMovementStatus::EMS_Dead); }

#pragma endregion


#pragma region BEHAVIOR_HANDLERS
	/// <summary>
	/// Handle how the status of the stamina bar
	/// </summary>
	/// <param name="DeltaTime"></param>
	void HandleStaminaStatus(float DeltaTime);

	/// <summary>
	/// Handle how the enemy health bar is displayed
	/// </summary>
	void HandleCombatTargetLocation();

#pragma endregion


#pragma region PICKUP_ITEMS
	/// <summary>
	/// Increase the coins by an Amount
	/// </summary>
	/// <param name="Amount"></param>
	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);
	
	/// <summary>
	/// Increase the player's health by an Amount
	/// </summary>
	/// <param name="Amount"></param>
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float Amount);

	/// <summary>
	/// Display a debug sphere on the location of an item that has been picked up
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

#pragma endregion


#pragma region LEVEL_TRANSITION

	void SwitchLevel(FName LevelName);

#pragma endregion


#pragma region SAVE_LOAD_GAME
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	
	class USaveGameProj01* LoadGame();

	// Load the game, reset the character's position, and reload the saved level
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool CanSetPosition);

#pragma endregion


};

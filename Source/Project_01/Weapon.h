// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	
	EWS_MAX			UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class PROJECT_01_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	FString Name{};

#pragma region PHYSICS_COMPONENTS
	/** Skeletal mesh of the weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh{ nullptr };

	/** Box collider used for combat */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class UBoxComponent* CombatCollision{ nullptr };

#pragma endregion


#pragma region SOUNDS
	/** Sound played when the weapon is equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound{ nullptr };

	/** Sound played when the weapon is swung */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound{ nullptr };

#pragma endregion


#pragma region DAMAGE
	/** Damage the weapon deals */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float Damage{};

	/** Damage type class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	TSubclassOf<UDamageType> DamageTypeClass{};

#pragma endregion


#pragma region WEAPON_WIELDER
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	AController* WeaponInstigator{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class AMain* Wielder{ nullptr };

#pragma endregion


	/** Enum for the state of the weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EWeaponState WeaponState{ EWeaponState::EWS_Pickup };

	/** Boolean to set if the weapon can emit particles or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bCanEmitParticles{};

protected:
	virtual void BeginPlay() override;

public:

	/// <summary>
	/// Getter for the WeaponState
	/// </summary>
	/// <returns></returns>
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	/// <summary>
	/// Setter of the WeaponState
	/// </summary>
	/// <param name="State"></param>
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }

	/// <summary>
	/// Event to detect when the outer sphere collider is overlapped with
	/// </summary>
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/// <summary>
	/// Event to detect when the outer sphere collider is no longer overlapped with
	/// </summary>
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	/// <summary>
	/// Detect when the combat box is overlapped
	/// </summary>
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// Detect when the combat box is no longer overlapped
	/// </summary>
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// Equip the weapon to a Character' socket
	/// </summary>
	/// <param name="Character"></param>
	void Equip(class AMain* Character);

	/// <summary>
	/// Allow the weapon to deal damage
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	
	/// <summary>
	/// Do not allow the weapon to deal damage anymore
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	/// <summary>
	/// Setter of the Weapon Instigator
	/// </summary>
	/// <param name="Instigator"></param>
	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()
public:
	// Amount of bullets in clip
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	int32 BulletsAmount;

	// Total amount of bullets
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	int32 BulletsTotal;
};

UCLASS()
class RIFT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

	virtual void StartFire();
	virtual void StopFire();

	// Assisting variable
	bool bCanFire;
	bool bReloading;

	// Function to start reloading
	void StartReload();
	bool CanReload() const;

	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	void OnEquip();

	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	// Timer handle for reloading
	FTimerHandle ReloadTimerHandle;

	// Functions for UI
	FAmmoData GetAmmoData() const { return CurrentAmmo; }
	bool IsReloading() const { return bReloading; }

	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void ChangeClip();

protected:
	virtual void BeginPlay() override;

	// Name for muzzle socket
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	FName MuzzleSocketName = "MuzzleFlash";

	// Mesh for our gun
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float ShootDistance = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float ReloadTime = 1.5f;
	
	// Information about ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	FAmmoData DefaultAmmo{30, 120};

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;

	// Helping functions for reload and shooting
	void DecreaseAmmo();

	// Debug function
	void LogAmmo();

private:
	// Stores current amount of ammo
	FAmmoData CurrentAmmo;
};

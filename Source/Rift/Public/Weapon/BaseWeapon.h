// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class AMainCharacter;
class UCurveFloat;

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

	void StartAiming();
	void StopAiming();

	virtual void Tick(float DeltaSeconds) override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Recoil)
	UCurveFloat* HorizontalCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Recoil)
	UCurveFloat* VerticalCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Recoil)
	float MinimumNonAimSpread = -0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Recoil)
	float MaximumNonAimSpread = 0.25f;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;

	// Helping functions for reload and shooting
	void DecreaseAmmo();

	// Debug function
	void LogAmmo();

	UPROPERTY()
	AMainCharacter* Player;

	FTimeline RecoilTimeline;

	UFUNCTION()
	void StartHorizontalRecoil(float Value);

	UFUNCTION()
	void StartVerticalRecoil(float Value);

	void StartRecoil();

	void StopRecoil();

	bool IsAiming = false;

private:
	// Stores current amount of ammo
	FAmmoData CurrentAmmo;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/MainCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create mesh for weapon
	// Choose mesh in blueprint
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.BulletsAmount > 0, TEXT("Bullets count couldn`t be less or equal zero"));
	checkf(DefaultAmmo.BulletsTotal > 0, TEXT("Total bullets count couldn`t be less or equal zero"));
	CurrentAmmo = DefaultAmmo;
}


void ABaseWeapon::OnEquip()
{
	WeaponMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ABaseWeapon::StartFire()
{
}

void ABaseWeapon::StopFire()
{
}

void ABaseWeapon::MakeShot()
{
}

bool ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Character = Cast<AMainCharacter>(GetOwner());
	if (!Character)return false;

	if (Character->IsPlayerControlled())
	{
		Character->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}

	return true;
}

FVector ABaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * ShootDistance;
	return true;
}

void ABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
}


void ABaseWeapon::DecreaseAmmo()
{
	CurrentAmmo.BulletsAmount--;
	// Debug log
	LogAmmo();

	// If no ammo in clip and have ammo in stash - start reload
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StartReload();
	}
}

// If we have no ammo at all return true
bool ABaseWeapon::IsAmmoEmpty() const
{
	return IsClipEmpty() && CurrentAmmo.BulletsTotal == 0;
}

// If no ammo in current clip return true
bool ABaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.BulletsAmount == 0;
}

void ABaseWeapon::StartReload()
{
	// If we have bullets in stash start reloading
	if (CurrentAmmo.BulletsTotal > 0)
	{
		// Can't fire during reloading
		bCanFire = false;
		bReloading = true;
		// Start reloading "animation" for 1.5s
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ABaseWeapon::ChangeClip, ReloadTime, false);
	}
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmo.BulletsAmount < DefaultAmmo.BulletsAmount && CurrentAmmo.BulletsTotal > 0;
}

void ABaseWeapon::ChangeClip()
{
	// Get amount of ammo we lack in clip
	int32 NeedAmmo = DefaultAmmo.BulletsAmount - CurrentAmmo.BulletsAmount;

	// if we have such amount of ammo in stash fill our clip
	if (NeedAmmo <= CurrentAmmo.BulletsTotal)
	{
		CurrentAmmo.BulletsAmount += NeedAmmo;
		CurrentAmmo.BulletsTotal -= NeedAmmo;
	}
	else
	{
		// if not just use as much as possible
		CurrentAmmo.BulletsAmount += CurrentAmmo.BulletsTotal;
		CurrentAmmo.BulletsTotal = 0;
	}

	// Let fire at the end
	bCanFire = true;
	bReloading = false;
}

// Debug function
void ABaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.BulletsAmount) + " / ";
	AmmoInfo += FString::FromInt(CurrentAmmo.BulletsTotal);
	UE_LOG(LogTemp, Display, TEXT("%s"), *AmmoInfo);
}

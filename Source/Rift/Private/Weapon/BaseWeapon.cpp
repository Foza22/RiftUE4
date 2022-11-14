// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/MainCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	FOnTimelineFloat XRecoilCurve;
	FOnTimelineFloat YRecoilCurve;

	XRecoilCurve.BindUFunction(this, FName("StartHorizontalRecoil"));
	YRecoilCurve.BindUFunction(this, FName("StartVerticalRecoil"));

	if(!HorizontalCurve || !VerticalCurve) return;
	
	RecoilTimeline.AddInterpFloat(HorizontalCurve, XRecoilCurve);
	RecoilTimeline.AddInterpFloat(VerticalCurve, YRecoilCurve);
}

void ABaseWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(RecoilTimeline.IsPlaying())
	{
		RecoilTimeline.TickTimeline(DeltaSeconds);
	}

	if(RecoilTimeline.IsReversing())
	{
		RecoilTimeline.TickTimeline(DeltaSeconds);
	}
}

void ABaseWeapon::OnEquip()
{
	WeaponMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Player = Cast<AMainCharacter>(GetOwner());
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
	if (!Character) return false;

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
	TraceEnd = TraceStart + ViewRotation.Vector() * ShootDistance;
	
	if(!IsAiming)
	{
		TraceEnd.Y += TraceEnd.Y * FMath::RandRange(MinimumNonAimSpread, MaximumNonAimSpread);
		TraceEnd.Z += TraceEnd.Z * FMath::RandRange(MinimumNonAimSpread, MaximumNonAimSpread);
	}
	
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
	//LogAmmo();

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
	StopFire();
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

void ABaseWeapon::StartHorizontalRecoil(float Value)
{
	Player->AddControllerYawInput(Value);
}

void ABaseWeapon::StartVerticalRecoil(float Value)
{
	Player->AddControllerPitchInput(Value);
}

void ABaseWeapon::StartRecoil()
{
	RecoilTimeline.PlayFromStart();
}

void ABaseWeapon::StopRecoil()
{
	RecoilTimeline.Stop();
}

void ABaseWeapon::StartAiming()
{
	// Temporary aim
	Player->GetCurrentCamera()->SetFieldOfView(50.f);
	IsAiming = true;
}

void ABaseWeapon::StopAiming()
{
	// Temporary aim
	Player->GetCurrentCamera()->SetFieldOfView(90.f);
	IsAiming = false;
}

// Debug function
void ABaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.BulletsAmount) + " / ";
	AmmoInfo += FString::FromInt(CurrentAmmo.BulletsTotal);
	UE_LOG(LogTemp, Display, TEXT("%s"), *AmmoInfo);
}


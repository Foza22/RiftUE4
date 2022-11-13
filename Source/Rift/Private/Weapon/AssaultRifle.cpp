// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AssaultRifle.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Player/MainCharacter.h"

AAssaultRifle::AAssaultRifle()
{
}

void AAssaultRifle::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / FireRate;
}

void AAssaultRifle::StartFire()
{
	if (IsClipEmpty()) return;

	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AAssaultRifle::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void AAssaultRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void AAssaultRifle::MakeShot()
{
	if(IsClipEmpty()) return;
	
	const auto Player = Cast<AMainCharacter>(GetOwner());
	if (!GetWorld() || Player->IsRunning())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("YOU HIT"));
		DrawDebugSphere(GetWorld(), HitResult.Location, 5.f, 10, FColor::Red, false, 3);
		MakeDamage(HitResult);
	}

	DecreaseAmmo();
}

bool AAssaultRifle::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * ShootDistance;
	return true;
}

void AAssaultRifle::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
}

AController* AAssaultRifle::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

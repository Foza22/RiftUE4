// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AssaultRifle.h"

#include "DrawDebugHelpers.h"
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
	StartRecoil();
}

void AAssaultRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	StopRecoil();
}

void AAssaultRifle::MakeShot()
{
	if(IsClipEmpty())
	{
		StopFire();
		return;
	}
	
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
		DrawDebugSphere(GetWorld(), HitResult.Location, 5.f, 10, FColor::Red, false, 3);
		MakeDamage(HitResult);
	}

	DecreaseAmmo();
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

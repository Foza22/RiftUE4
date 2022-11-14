// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "AssaultRifle.generated.h"


UCLASS()
class RIFT_API AAssaultRifle : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AAssaultRifle();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float FireRate = 560.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float DamageAmount = 10.0f;
	
	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	
private:
	FTimerHandle ShotTimerHandle;

	void MakeDamage(const FHitResult& HitResult);

	AController* GetController() const;

	float TimeBetweenShots;
};

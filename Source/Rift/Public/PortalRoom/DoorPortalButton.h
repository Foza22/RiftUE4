// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorPortalButton.generated.h"

class UBoxComponent;
class UPointLightComponent;
class UAudioComponent;
class APortalDoor;

UCLASS()
class RIFT_API ADoorPortalButton : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorPortalButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* ButtonBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* CylinderCollision;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	// UPointLightComponent* ButtonLightning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UAudioComponent* ButtonSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	APortalDoor* Door;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonPress)
	float ButtonMovementSpeed = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonPress)
	float ButtonPressedLocation = -15.f;

private:
	float ButtonInitialLocation;
	float ButtonTargetLocation;
	float ButtonCurrentLocation;

	float NewLocation;

	bool IsPressed;
	bool IsReleased;

	void PressButton(float DeltaTime);
	void ReleaseButton(float DeltaTime);
};

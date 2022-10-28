// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "BaseVehicle.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWheeledVehicleMovementComponent4W;
class UBoxComponent;
class AMainCharacter;

UCLASS()
class RIFT_API ABaseVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

protected:
	// Make camera offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	// Make camera for vehicle
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* InteractCollision;

	// Force to rotate when car is not ground
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Force = 20.f;

public:
	// Sets default values for this pawn's properties
	ABaseVehicle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Overlap functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Functions for movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Functions for camera rotation
	void LookUp(float Value);
	void Turn(float Value);

	// Functions for handbrake
	void OnHandbreakPressed();
	void OnHandbreakReleased();

	// Update air physics in air
	void UpdateInAirControl(float DeltaTime);

	// Function to get in/out to/from vehicle
	void LeaveVehicle();

	// Controller of vehicle
	UWheeledVehicleMovementComponent4W* Vehicle4W;

	// Save our character while controlling car
	AMainCharacter* CurrentCharacter;
};

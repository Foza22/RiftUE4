// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UBoxComponent;
class ADoor;

UCLASS()
class RIFT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Make camera rotation correct
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	// Camera for 3rd person view
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent3P;

	// Camera for 1st person view
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent1P;

	// Function for activating running animation
	UFUNCTION(BlueprintCallable, Category = Movement)
	bool IsRunning() const;

	// Health function for UI
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetHealth() const { return Health; }
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overlap functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Settings function to change hold/toggle crouch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
	bool CrouchHold;

	// Health by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	float DefaultHealth = 100.f;

private:
	// Hold current overlapped door
	UPROPERTY()
	ADoor* CurrentDoor;

	// Interact with door
	void Interact();

	// Functions for movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Functions for running
	void OnStartRunning();
	void OnStopRunning();

	// Functions for camera rotation
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Functions for hold crouch
	void StartCrouch();
	void StopCrouch();

	// Function for toggle crouch
	void ToggleCrouch();

	// Function for changing view
	void ToggleCameraView();

	// Take damage to actor
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;


	// Variables for activating running
	bool WantsToRun;
	bool IsMovingForward;

	// Speed settings
	UPROPERTY(EditDefaultsOnly, Category=Speed)
	float MaxSpeedWalk = 400.f;

	UPROPERTY(EditDefaultsOnly, Category=Speed)
	float MaxSpeedCrouch = 200.f;

	UPROPERTY(EditDefaultsOnly, Category=Speed)
	float MaxSpeedRun = 600.f;

	// Variable for toggle crouch mode
	bool IsCrouching;

	// Current health
	float Health = 0.f;

	// Flag for current view mode
	bool bIsThirdPerson;
};

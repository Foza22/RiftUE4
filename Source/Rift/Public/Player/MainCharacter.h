// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Settings function to change hold/toggle crouch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
		bool CrouchHold;

	// Functions for movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Functions for camera rotation
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Functions for hold crouch
	void StartCrouch();
	void StopCrouch();

	// Functions for toggle crouch
	void ToggleCrouch();
	bool IsCrouching;

	// Function for changing view
	void ToggleCameraView();

	// Flag for current view mode
	bool bIsThirdPerson;
};

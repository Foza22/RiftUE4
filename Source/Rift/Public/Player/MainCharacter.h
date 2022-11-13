// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UBoxComponent;
class ADoor;
class ABaseVehicle;
class UPhysicsHandleComponent;
class ABuildingVisual;

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

	// Make camera offset
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	// Camera for 3rd person view
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent3P;

	// Camera for 1st person view
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	TSubclassOf<ABuildingVisual> BuildingClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	ABuildingVisual* Builder;

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

	UFUNCTION(BlueprintCallable, Category = Camera)
	UCameraComponent* GetCurrentCamera() const { return CurrentCamera; }

	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCurrentCamera(UCameraComponent* Camera) { CurrentCamera = Camera; }

	// Function for activating running animation
	UFUNCTION(BlueprintCallable, Category = Movement)
	bool IsRunning() const;

	// Health function for UI
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetIsWearingWeapon() const { return IsWearingWeapon; }

	UFUNCTION(BlueprintCallable, Category = Building)
	void SetBuildMode(bool Enabled);

	UFUNCTION(BlueprintCallable, Category = Building)
	bool GetBuildMode() const { return bInBuildMode; }

	UFUNCTION(BlueprintCallable, Category = Building)
	void SpawnBuilding();

	UFUNCTION(BlueprintCallable, Category = Building)
	void CycleBuildingMesh();
	
	// Functions for Driving Animations
	UFUNCTION(BlueprintCallable, Category = Building)
	void SetIsDriving(bool Condition) { IsDriving = Condition; }

	UFUNCTION(BlueprintCallable, Category = Driving)
	bool GetIsDriving() const { return IsDriving; }

	void PerformLineTrace(FHitResult& HitResult, float Distance = 650.f, bool DrawDebug = false) const;

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	UCameraComponent* CurrentCamera = nullptr;

	// Settings function to change hold/toggle crouch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
	bool CrouchHold = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool IsWearingWeapon = false;

	UPROPERTY(BlueprintReadOnly, Category= Building)
	bool bInBuildMode;

	// Health by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	float DefaultHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Grab)
	float GrabDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Building)
	float BuildDestroyDistance = 650.f;

private:
	// Hold current overlapped door
	UPROPERTY()
	ADoor* CurrentDoor = nullptr;

	// Hold current overlapped car
	UPROPERTY()
	ABaseVehicle* CurrentVehicle = nullptr;

	// If weapon created we can shoot
	UPROPERTY()
	ABaseWeapon* CurrentWeapon = nullptr;

	// Speed settings
	UPROPERTY(EditDefaultsOnly, Category=Speed)
	float MaxSpeedWalk = 400.f;

	UPROPERTY(EditDefaultsOnly, Category=Speed)
	float MaxSpeedCrouch = 200.f;

	UPROPERTY(EditDefaultsOnly, Category=Speed)
	float MaxSpeedRun = 600.f;

	// Function responsible for deleting object from map and adding it to player
	void PickupWeapon(const FHitResult& Hit);

	// Function for shooting
	void StartFire();
	void StopFire();

	// Function for reloading weapon
	void ReloadWeapon();

	void ToggleBuildMode();

	void DestroyInstance();

	// Interact with door
	void Interact();

	// Function called on Interact with car
	void GetToVehicle();

	// Functions for movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	void ToggleGrab(const FHitResult& Hit);

	// Functions for running
	void OnStartRunning();
	void OnStopRunning();

	void Turn(float Value);
	void LookUp(float Value);

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
	bool WantsToRun = false;
	bool IsMovingForward = false;

	// Variable for driving animation
	bool IsDriving = false;

	bool IsGrabbing = false;

	// Variable for toggle crouch mode
	bool IsCrouching = false;

	// Current health
	float Health = 0.f;

	// Flag for current view mode
	bool bIsThirdPerson = false;
};

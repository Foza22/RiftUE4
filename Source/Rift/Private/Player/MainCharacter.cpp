// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create component for right rotation of camera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	SpringArmComponent->SetRelativeLocation(FVector(10.f, 0.f, 70.f));
	SpringArmComponent->TargetArmLength = 350;
	SpringArmComponent->SocketOffset.Z = 50.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	bIsThirdPerson = false;

	// Create camera for 3rd person view
	CameraComponent3P = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp3P"));
	CameraComponent3P->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	CameraComponent3P->SetupAttachment(SpringArmComponent);
	CameraComponent3P->SetAutoActivate(false);

	// Create camera for 1st person view
	CameraComponent1P = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp1P"));
	CameraComponent1P->SetupAttachment(GetMesh(), FName(TEXT("head")));
	CameraComponent1P->SetRelativeLocation(FVector(10.f, 20.f, 0.f));
	CameraComponent1P->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	CameraComponent1P->SetAutoActivate(true);
	CameraComponent1P->bUsePawnControlRotation = true;

	// Change default movement parameters
	GetCharacterMovement()->MaxWalkSpeed = 400;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200;
	CrouchHold = true;
	IsCrouching = false;
}


// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind move events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// Bind view rotation for mouse
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind view rotation for joystick
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);

	// Bind for camera switch
	PlayerInputComponent->BindAction("ToggleCameraView", IE_Pressed, this, &AMainCharacter::ToggleCameraView);

	// Bind jump event
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind crouch event depending on settings
	if (CrouchHold)
	{
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouch);
		PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouch);
	}
	else
	{
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::ToggleCrouch);
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if (Value == 0.f) return;
	// Add movement in that direction
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMainCharacter::MoveRight(float Value)
{
	if (Value == 0.f) return;
	// Add movement in that direction
	AddMovementInput(GetActorRightVector(), Value);
}

// Functions for hold crouch
void AMainCharacter::StartCrouch()
{
	ACharacter::Crouch();
}

void AMainCharacter::StopCrouch()
{
	ACharacter::UnCrouch();
}

// Functions for toggle crouch
void AMainCharacter::ToggleCrouch()
{
	if (IsCrouching)
	{
		ACharacter::UnCrouch();
	}
	else
	{
		ACharacter::Crouch();
	}
	IsCrouching = !IsCrouching;
}


void AMainCharacter::TurnAtRate(float Rate)
{
	if (Rate == 0.f) return;
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	if (Rate == 0.f) return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::ToggleCameraView()
{
	// If we currently in third view we swap to first view
	if (bIsThirdPerson)
	{
		// Change our settings for first view
		CameraComponent3P->SetActive(false);
		CameraComponent1P->SetActive(true);
		// Set our flag to first view after changes
		bIsThirdPerson = false;
	}
	else
	{
		// Change our settings to default
		CameraComponent1P->SetActive(false);
		CameraComponent3P->SetActive(true);
		// Set our flag to third view after changes
		bIsThirdPerson = true;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interactions/Door.h"

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
	CameraComponent1P->SetRelativeLocation(FVector(0.f, 10.f, 0.f));
	CameraComponent1P->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	CameraComponent1P->SetAutoActivate(true);
	CameraComponent1P->bUsePawnControlRotation = true;

	// Change default movement parameters
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedWalk;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxSpeedCrouch;
	CrouchHold = true;
	IsCrouching = false;

	// On the beginning our character has default amount of health
	Health = DefaultHealth;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnOverlapEnd);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind move events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// Bind running
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::OnStopRunning);

	// Bind view rotation for mouse
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind view rotation for joystick
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);

	// CURRENTLY DISABLED
	// Bind for camera switch
	PlayerInputComponent->BindAction("ToggleCameraView", IE_Pressed, this, &AMainCharacter::ToggleCameraView);

	// Bind for interaction with world
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::Interact);

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

void AMainCharacter::Interact()
{
	// If we can get current door - we interact with it
	if (CurrentDoor)
	{
		FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
		CurrentDoor->ToggleDoor(ForwardVector);
	}
}

void AMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	// If we overlap door access radius - save it like current door
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->GetClass()->IsChildOf(
		ADoor::StaticClass()))
	{
		// if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Came to door!"));

		CurrentDoor = Cast<ADoor>(OtherActor);
	}
}

void AMainCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// On quiting access radius - delete current door 
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Came from door!"));

		CurrentDoor = nullptr;
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if (Value == 0.f) return;
	// Add movement in that direction
	AddMovementInput(GetActorForwardVector(), Value);
	IsMovingForward = Value > 0.f;
}

void AMainCharacter::MoveRight(float Value)
{
	if (Value == 0.f) return;
	// Add movement in that direction
	AddMovementInput(GetActorRightVector(), Value);
}

void AMainCharacter::OnStartRunning()
{
	// Turn flag to true
	WantsToRun = true;
	// Change max speed
	if (IsMovingForward)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedRun;
	}
}

void AMainCharacter::OnStopRunning()
{
	// Turn flag to false
	WantsToRun = false;
	// Return default max speed
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedWalk;
}

bool AMainCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}


float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Display, TEXT("Health: %f"), Health);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("DEATH!"));
	}

	return DamageAmount;
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
	// If we currently crouch and press the key - stand up
	if (IsCrouching)
	{
		ACharacter::UnCrouch();
	}
	// Is we don't crouch and press key - we start crouching
	else
	{
		ACharacter::Crouch();
	}
	// Toggle crouching flag
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

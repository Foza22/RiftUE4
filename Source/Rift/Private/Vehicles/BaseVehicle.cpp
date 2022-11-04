// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/BaseVehicle.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "AnimNodes/AnimNode_RandomPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/MainCharacter.h"

ABaseVehicle::ABaseVehicle()
{
	Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	// Change mass
	Vehicle4W->Mass = 1500.f;

	// Engine power
	Vehicle4W->MaxEngineRPM = 5700.f;
	
	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 3.f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 3.0f;
	
	// Torque Setup
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f,400.f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.f,500.f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.f,400.f);
	
	// Adjust the steering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.f,1.f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.f,0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.f,0.6f);

	// Automatic gear box
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.5f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 2.f;

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.45f;

	Vehicle4W->ChassisWidth = 140.f;
	Vehicle4W->ChassisHeight = 23.f;
	
	GetMesh()->SetWorldScale3D(FVector(1.2f,1.2f,1.2f));	
	
	// Create spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0.f,0.f,0.f));
	SpringArm->TargetArmLength = 3000.f;
	SpringArm->SocketOffset.Z = 0.f;
	SpringArm->bUsePawnControlRotation = true;

	// Create camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.f;

	// Create collision for interacting with car
	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->InitBoxExtent(FVector(200.f,200.f,200.f));
	InteractCollision->SetRelativeLocation(FVector(0.f,0.f,70.f));

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseVehicle::OnOverlapBegin);
}

void ABaseVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Currently Disabled
	//UpdateInAirControl(DeltaTime);
}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseVehicle::MoveRight);

	// Bind camera rotation
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseVehicle::LookUp); 
	PlayerInputComponent->BindAxis("Turn", this, &ABaseVehicle::Turn);

	// Bind handbrake
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ABaseVehicle::OnHandbreakPressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ABaseVehicle::OnHandbreakReleased);

	// Bind interaction with car
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseVehicle::LeaveVehicle);
}

void ABaseVehicle::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->GetClass()->IsChildOf(
		AMainCharacter::StaticClass()))
	{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Someone went to car!"));

		CurrentCharacter = Cast<AMainCharacter>(OtherActor);
	}
}

void ABaseVehicle::MoveForward(float Value)
{
	GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void ABaseVehicle::MoveRight(float Value)
{
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void ABaseVehicle::LookUp(float Value)
{
	if(Value == 0) return;

	AddControllerPitchInput(Value);
}

void ABaseVehicle::Turn(float Value)
{
	if(Value == 0) return;

	AddControllerYawInput(Value);
}

void ABaseVehicle::OnHandbreakPressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ABaseVehicle::OnHandbreakReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ABaseVehicle::LeaveVehicle()
{
	GetVehicleMovementComponent()->SetThrottleInput(0);
	CurrentCharacter->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT("CharacterQuit"));
	CurrentCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentCharacter->SetIsDriving(false);
	CurrentCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	auto Rotation = CurrentCharacter->GetActorRotation();
	Rotation.Roll = 0.f;
	CurrentCharacter->SetActorRotation(Rotation);
	GetController()->Possess(CurrentCharacter);
}

// DISABLED. CAUSE A LOT OF BUGS AND CRUSHES
// void ABaseVehicle::UpdateInAirControl(float DeltaTime)
// {
// 	if(Vehicle4W)
// 	{
// 		FCollisionQueryParams QueryParams;
// 		QueryParams.AddIgnoredActor(this);
//
// 		const FVector TraceStart = GetActorLocation() + FVector(0.f,0.f,50.f);
// 		const FVector TraceEnd = GetActorLocation() - FVector(0.f,0.f,200.f);
//
// 		FHitResult Hit;
//
// 		// Check if car is in air and is it flipped on its side
// 		const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
// 		const bool bNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1f;
//
// 		// Is car is in air allow air movement
// 		if(bInAir || bNotGrounded)
// 		{
// 			const float ForwardInput = InputComponent->GetAxisValue("MoveForward");
// 			const float RightInput = InputComponent->GetAxisValue("MoveRight");
//
// 			// If car is grounded allow to roll car
// 			const float AirMovementForcePitch = 3.f;
// 			const float AirMovementForceRoll = !bInAir && bNotGrounded ? 20.f : 3.f;
//
// 			if(UPrimitiveComponent* VehicleMesh = Vehicle4W->UpdatedPrimitive)
// 			{
// 				const FVector MovementVector = FVector(RightInput * -AirMovementForceRoll, ForwardInput*AirMovementForcePitch, 0.f) * DeltaTime * Force;
// 				const FVector NewAngularMovement = GetActorRotation().RotateVector(MovementVector);
//
// 				VehicleMesh->SetPhysicsAngularVelocity(NewAngularMovement, true);
// 			}
// 		}
// 	}
// }

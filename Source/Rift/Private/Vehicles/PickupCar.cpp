// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/PickupCar.h"
#include "GameFramework/SpringArmComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

APickupCar::APickupCar()
{
	// Change mass
	Vehicle4W->Mass = 1500.f;
	Force = 30.f;

	// Engine power
	Vehicle4W->MaxEngineRPM = 4500.f;

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.45f;

	SpringArm->SetRelativeLocation(FVector(-100.f,0.f,105.f));
	SpringArm->TargetArmLength = 550.f;
	SpringArm->SocketOffset.Z = 50.f;
	SpringArm->bUsePawnControlRotation = true;

	Vehicle4W->ChassisWidth = 130.f;
	Vehicle4W->ChassisHeight = 30.f;
}


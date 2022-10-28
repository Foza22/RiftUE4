// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/SportCar.h"
#include "GameFramework/SpringArmComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Components/BoxComponent.h"

ASportCar::ASportCar()
{
	// Change mass
	Vehicle4W->Mass = 1500.f;
	Force = 30.f;

	// Engine power
	Vehicle4W->MaxEngineRPM = 15000.f;

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.45f;

	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.1f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.f;

	Vehicle4W->TransmissionSetup.ClutchStrength = 20.f;

	SpringArm->SetRelativeLocation(FVector(-100.f,0.f,105.f));
	SpringArm->TargetArmLength = 550.f;
	SpringArm->SocketOffset.Z = 50.f;
	SpringArm->bUsePawnControlRotation = true;

	Vehicle4W->ChassisWidth = 130.f;
	Vehicle4W->ChassisHeight = 30.f;

	InteractCollision->InitBoxExtent(FVector(60.f,150.f,50.f));
	InteractCollision->SetRelativeLocation(FVector(20.f,0.f,70.f));
}


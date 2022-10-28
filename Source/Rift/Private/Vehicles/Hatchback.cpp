// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/Hatchback.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

AHatchback::AHatchback()
{
	// Change mass
	Vehicle4W->Mass = 1000.f;
	Force = 20.f;

	// Engine power
	Vehicle4W->MaxEngineRPM = 5700.f;

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.45f;
	
	SpringArm->SetRelativeLocation(FVector(-100.f,0.f,105.f));
	SpringArm->TargetArmLength = 550.f;
	SpringArm->SocketOffset.Z = 50.f;
	SpringArm->bUsePawnControlRotation = true;

	Vehicle4W->ChassisWidth = 140.f;
	Vehicle4W->ChassisHeight = 23.f;

	InteractCollision->InitBoxExtent(FVector(65.f,120.f,50.f));
	InteractCollision->SetRelativeLocation(FVector(15.f,0.f,80.f));
}


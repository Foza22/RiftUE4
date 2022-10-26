// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/BoxTruck.h"
#include "GameFramework/SpringArmComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

ABoxTruck::ABoxTruck()
{
	// Change mass
	Vehicle4W->Mass = 6000.f;
	Force = 50.f;

	// Engine power
	Vehicle4W->MaxEngineRPM = 5700.f;

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::Open_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.45f;

	SpringArm->SetRelativeLocation(FVector(-100.f,0.f,105.f));
	SpringArm->TargetArmLength = 950.f;
	SpringArm->SocketOffset.Z = 50.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	Vehicle4W->ChassisWidth = 180.f;
	Vehicle4W->ChassisHeight = 40.f;
}



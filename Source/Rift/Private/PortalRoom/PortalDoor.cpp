// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalRoom/PortalDoor.h"

#include "Components/AudioComponent.h"

// Sets default values
APortalDoor::APortalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BottomSurface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomSurface"));
	BottomSurface->SetupAttachment(RootComponent);
	BottomSurface->SetRelativeScale3D(FVector(2.f,2.f,2.f));

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(RootComponent);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(RootComponent);

	TopSurface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopSurface"));
	TopSurface->SetupAttachment(RootComponent);

	OpenSound = CreateDefaultSubobject<UAudioComponent>(TEXT("OpenSound"));
	OpenSound->SetupAttachment(RootComponent);
}

void APortalDoor::OpenDoor(float NewLocation) const
{
	const float Location = NewLocation * DoorSpeed;
	
	LeftDoor->SetRelativeLocation(FVector(Location,0.f,0.f));
	RightDoor->SetRelativeLocation(FVector(-Location,0.f,0.f));
}

void APortalDoor::CloseDoor(float NewLocation) const
{
	const float Location = NewLocation * DoorSpeed;
	
	LeftDoor->SetRelativeLocation(FVector(Location,0.f,0.f));
	RightDoor->SetRelativeLocation(FVector(-Location,0.f,0.f));
}


// Called when the game starts or when spawned
void APortalDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


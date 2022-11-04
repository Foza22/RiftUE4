// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalRoom/PortalDoor.h"

// Sets default values
APortalDoor::APortalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BottomSurface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomSurface"));
	BottomSurface->SetupAttachment(RootComponent);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(RootComponent);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(RootComponent);

	TopSurface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopSurface"));
	TopSurface->SetupAttachment(RootComponent);
}

void APortalDoor::OpenDoor()
{
	UE_LOG(LogTemp, Display, TEXT("OPEN OPEN"));
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


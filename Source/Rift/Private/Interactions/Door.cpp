// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Door.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our Default Components
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorProxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorProximityVolume"));

	//Setup our Attachments
	DoorFrame->SetupAttachment(RootComponent);
	Door->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform);
	DoorProxVolume->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform);

	DoorProxVolume->InitBoxExtent(FVector(120.f, 100.f, 110.f));

	// Get door initial yaw 
	InitialYaw = GetActorRotation().Yaw;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// On every tick get state of flag
	// If state is opening - open door
	if (IsOpening)
	{
		OpenDoor(DeltaTime);
	}
	// If state is closing - close door
	if (IsClosing)
	{
		CloseDoor(DeltaTime);
	}
}

void ADoor::ToggleDoor(const FVector& ForwardVector)
{
	// Is door forward vector and character are in the same direction?
	DotProd = FVector::DotProduct(DoorProxVolume->GetForwardVector(), ForwardVector);

	// Set value -1 or 1 based on DotProd result
	Direction = FMath::Sign(DotProd);

	// Set value of opened door -90 or 90 direction
	MaxDegree = Direction * 90.f;

	// If door Is Closed on the interaction event - make it opening
	if (IsClosed)
	{
		IsClosed = false;
		IsClosing = false;
		IsOpening = true;
	}
	// If door is not closed on the interaction event - make it closing
	else
	{
		IsClosed = true;
		IsClosing = true;
		IsOpening = false;
	}
}

void ADoor::OpenDoor(float DeltaTime)
{
	// Get door current yaw 
	DoorCurrentRotation = Door->GetRelativeRotation().Yaw;

	// Find rotation to add
	AddRotation = Direction * DeltaTime * DoorOpeningSpeed;

	// If door yaw close to max degree - stop rotating
	if (FMath::IsNearlyEqual(DoorCurrentRotation, MaxDegree, 1.5f))
	{
		IsClosing = false;
		IsOpening = false;
	}
	// Add extra rotation to current rotation 
	else if (IsOpening)
	{
		Door->AddRelativeRotation(FQuat(FRotator(0.f, AddRotation, 0.f)), false, nullptr, ETeleportType::None);
	}
}

void ADoor::CloseDoor(float DeltaTime)
{
	// Get door current yaw 
	DoorCurrentRotation = Door->GetRelativeRotation().Yaw;

	// Depending on opening rotation get closing direction
	AddRotation = DoorCurrentRotation > InitialYaw ? -DeltaTime * DoorOpeningSpeed : DeltaTime * DoorOpeningSpeed;

	// If current rotation is close to Initial rotation - stop closing
	if (FMath::IsNearlyEqual(DoorCurrentRotation, InitialYaw, 1.5f))
	{
		IsClosing = false;
		IsOpening = false;
	}
	// Add extra rotation to current rotation 
	else if (IsClosing)
	{
		Door->AddRelativeRotation(FQuat(FRotator(0.f, AddRotation, 0.f)), false, nullptr, ETeleportType::None);
	}
}

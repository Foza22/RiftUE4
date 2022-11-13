// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalRoom/DoorPortalButton.h"
#include "Components/AudioComponent.h"
#include "PortalRoom/PortalDoor.h"

// Sets default values
ADoorPortalButton::ADoorPortalButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBaseMesh"));
	ButtonBase->SetupAttachment(RootComponent);

	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	Button->SetupAttachment(RootComponent);

	CylinderCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderCollision"));
	CylinderCollision->SetupAttachment(Button);
	CylinderCollision->SetRelativeLocation(FVector(0.f, 0.f, 45.f));
	CylinderCollision->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.03f));
	CylinderCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CylinderCollision->SetHiddenInGame(true);

	ButtonSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ButtonSound"));
	ButtonSound->SetAutoActivate(false);
	ButtonSound->bOverrideAttenuation = true;
	// Set inner and outer radius in blueprint

	Door = CreateDefaultSubobject<APortalDoor>(TEXT("PortalDoor"));

	CylinderCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoorPortalButton::OnOverlapBegin);
	CylinderCollision->OnComponentEndOverlap.AddDynamic(this, &ADoorPortalButton::OnOverlapEnd);

	ButtonInitialLocation = Button->GetRelativeLocation().Y;
	ButtonCurrentLocation = ButtonInitialLocation;
}

void ADoorPortalButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		IsPressed = true;
		IsReleased = false;
	}
}

void ADoorPortalButton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		IsReleased = true;
		IsPressed = false;
	}
}

// Called when the game starts or when spawned
void ADoorPortalButton::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorPortalButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPressed)
	{
		PressButton(DeltaTime);
	}

	if (IsReleased)
	{
		ReleaseButton(DeltaTime);
	}
}

void ADoorPortalButton::PressButton(float DeltaTime)
{
	ButtonCurrentLocation = Button->GetRelativeLocation().Z;
	ButtonTargetLocation = ButtonPressedLocation;

	NewLocation = FMath::FInterpConstantTo(ButtonCurrentLocation, ButtonTargetLocation, DeltaTime, ButtonMovementSpeed);

	if (FMath::IsNearlyEqual(ButtonCurrentLocation, ButtonTargetLocation))
	{
		IsPressed = false;
		IsReleased = false;
		ButtonSound->Play();
		Door->OpenSound->Play();
	}
	else if (IsPressed)
	{
		Button->SetRelativeLocation(FVector(0.f, 0.f, NewLocation));
		Door->OpenDoor(NewLocation);
	}
}

void ADoorPortalButton::ReleaseButton(float DeltaTime)
{
	ButtonCurrentLocation = Button->GetRelativeLocation().Z;
	ButtonTargetLocation = ButtonInitialLocation;

	NewLocation = FMath::FInterpConstantTo(ButtonCurrentLocation, ButtonTargetLocation, DeltaTime, ButtonMovementSpeed);

	if (FMath::IsNearlyEqual(ButtonCurrentLocation, ButtonTargetLocation))
	{
		IsPressed = false;
		IsReleased = false;
	}
	else if (IsReleased)
	{
		Button->SetRelativeLocation(FVector(0.f, 0.f, NewLocation));
		Door->CloseDoor(NewLocation);
	}
}

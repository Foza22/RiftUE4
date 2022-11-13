// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/BuildingVisual.h"

#include "BuildingSystem/BaseBuilding.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABuildingVisual::ABuildingVisual()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BuildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildMesh"));
	BuildMesh->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = BuildMesh;

	BuildingTypeIndex = 0;
	bMaterialIsTrue = false;
	bReturnedMesh = true;

	BuildMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuildingVisual::OnOverlapBegin);
	BuildMesh->OnComponentEndOverlap.AddDynamic(this, &ABuildingVisual::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABuildingVisual::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);

	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}

	if (MaterialTrue)
	{
		bMaterialIsTrue = true;
		BuildMesh->SetMaterial(0, MaterialTrue);
	}
}

void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult, const FRotator& Rotation)
{
	if (!HitResult.bBlockingHit)
	{
		InteractingBuilding = nullptr;
		SetActorHiddenInGame(true);
		return;
	}

	SetActorHiddenInGame(false);
	SetActorLocation(HitResult.Location);
	SetActorRotation(Rotation);
	InteractingBuilding = GetHitBuildingActor(HitResult);

	if (!InteractingBuilding)
	{
		if (bReturnedMesh)
		{
			SetMeshTo(EBuildType::Foundation);
		}
		InteractingBuilding = nullptr;
		return;
	}

	if (!bReturnedMesh)
	{
		ReturnMeshToSelected();
	}

	SocketData = InteractingBuilding->
		GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter);
	if (SocketData.SocketTransform.Equals(FTransform()))
	{
		if (MaterialFalse && bMaterialIsTrue)
		{
			bMaterialIsTrue = false;
			BuildMesh->SetMaterial(0, MaterialFalse);
		}
		return;
	}

	SetActorTransform(SocketData.SocketTransform);
	if (MaterialTrue && !bMaterialIsTrue)
	{
		bMaterialIsTrue = true;
		BuildMesh->SetMaterial(0, MaterialTrue);
	}
}

ABaseBuilding* ABuildingVisual::GetHitBuildingActor(const FHitResult& HitResult) const
{
	return Cast<ABaseBuilding>(HitResult.GetActor());
}

void ABuildingVisual::SetMeshTo(EBuildType BuildType)
{
	bReturnedMesh = false;

	for (const FBuildingVisualType& Building : BuildingTypes)
	{
		if (Building.BuildType == BuildType)
		{
			BuildMesh->SetStaticMesh(Building.BuildingMesh);
			return;
		}
	}
}

void ABuildingVisual::ReturnMeshToSelected()
{
	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		bReturnedMesh = true;
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}
}

void ABuildingVisual::SpawnBuilding()
{
	if (!BuildingClass || IsHidden()) return;

	if (!InteractingBuilding)
	{
		GetWorld()->SpawnActor<ABaseBuilding>(BuildingClass, GetActorTransform());
		return;
	}

	if (!bMaterialIsTrue) return;

	InteractingBuilding->AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
}

void ABuildingVisual::CycleMesh()
{
	if (!bReturnedMesh) return;

	if (++BuildingTypeIndex >= BuildingTypes.Num())
	{
		BuildingTypeIndex = 0;
	}

	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}
}

void ABuildingVisual::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->IsA(ABaseBuilding::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("YOU CAN NOT BUILD"));
	}
}

void ABuildingVisual::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// On quiting access radius - delete current door and vehicle 
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("YOU CAN BUILD"));
	}
}



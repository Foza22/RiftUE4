// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/BaseBuilding.h"
#include "DrawDebugHelpers.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ABaseBuilding::ABaseBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedMesh"));
	RootComponent = FoundationInstancedMesh;

	WallInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstancedMesh"));

	CeilingInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CeilingInstancedMesh"));
}

// Called when the game starts or when spawned
void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	
	FInstanceSocketCheck InstanceSocket;
	
	InstanceSocket.InstancedComponent = FoundationInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);
	
	InstanceSocket.InstancedComponent = WallInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = CeilingInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);
	
	FBuildingSocketData BuildingSocketData;
	BuildingSocketData.Index = 0;
	BuildingSocketData.InstancedComponent = FoundationInstancedMesh;
	BuildingSocketData.SocketName = NAME_None;
	BuildingSocketData.SocketTransform = GetActorTransform();
	AddInstance(BuildingSocketData, EBuildType::Foundation);
	
	MeshInstanceSockets = FoundationInstancedMesh->GetAllSocketNames();
	MeshInstanceSockets.Append(WallInstancedMesh->GetAllSocketNames());
	MeshInstanceSockets.Append(CeilingInstancedMesh->GetAllSocketNames());
}

void ABaseBuilding::DestroyInstance(FVector& HitPoint)
{
	const TArray<int32> HitIndexes = FoundationInstancedMesh->GetInstancesOverlappingSphere(HitPoint, 1.f);

	if (HitIndexes.Num() <= 0) return;

	FoundationInstancedMesh->RemoveInstance(HitIndexes[0]);
}

FTransform ABaseBuilding::GetSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex,
                                             const FName& SocketName)
{
	if (InstancedComponent)
	{
		FTransform InstanceTransform = FTransform();
		InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
		FTransform SocketTransform = InstancedComponent->GetSocketTransform(SocketName, RTS_Component);
		InstanceTransform = SocketTransform * InstanceTransform;

		// DEBUG
		DrawDebugString(GetWorld(), InstanceTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White,
		                5.f);
		DrawDebugSphere(GetWorld(), InstanceTransform.GetLocation(), 12.f, 10, FColor::Red);
		FTransform Temp;
		InstancedComponent->GetInstanceTransform(InstanceIndex, Temp, true);
		DrawDebugSphere(GetWorld(), Temp.GetLocation(), 5.f, 15, FColor::Blue);

		return InstanceTransform;
	}

	return FTransform();
}

int32 ABaseBuilding::GetHitIndex(const FHitResult& HitResult) const
{
	DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 10, FColor::Red);

	return HitResult.Item;
}

FBuildingSocketData ABaseBuilding::GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter,
                                                         float ValidHitDistance)
{
	FBuildingSocketData SocketData = FBuildingSocketData();

	UInstancedStaticMeshComponent* HitComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent());
	if (!HitComponent) return SocketData;

	int32 HitIndex = GetHitIndex(HitResult);
	if (HitIndex == -1) return SocketData;

	for (const FName& SocketName : MeshInstanceSockets)
	{
		if (!IsValidSocket(HitComponent, SocketName, Filter)) continue;

		FTransform SocketTransform = GetSocketTransform(HitComponent, HitIndex, SocketName);
		if (FVector::Distance(SocketTransform.GetLocation(), HitResult.Location) <= ValidHitDistance)
		{
			SocketData.InstancedComponent = HitComponent;
			SocketData.Index = HitIndex;
			SocketData.SocketName = SocketName;
			SocketData.SocketTransform = SocketTransform;

			return SocketData;
		}
	}

	return SocketData;
}

bool ABaseBuilding::IsValidSocket(UInstancedStaticMeshComponent* HitComponent, const FName& SocketName,
                                  const FName& Filter) const
{
	bool bSuccess = true;

	const FString FilterString = Filter.ToString();
	const FString SocketNameString = SocketName.ToString();

	if (!SocketNameString.Contains(FilterString))
	{
		bSuccess = false;
	}

	return bSuccess;
}

// Get all instances to one mesh
void ABaseBuilding::AddInstance(const FBuildingSocketData& SocketData, EBuildType BuildType)
{
	for(auto& InstanceSocket : InstanceSocketsCheck)
	{
		if(InstanceSocket.InstancedComponent == SocketData.InstancedComponent)
		{
			bool bFoundMatching = false;
			for(const auto& IndexSocket : InstanceSocket.InstanceSocketInformation)
			{
					if(IndexSocket.Index == SocketData.Index)
					{
						bFoundMatching = true;
						break;
					}
			}

			if(bFoundMatching)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found matching index"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not find matching index"));
				FBuildingIndexSockets BuildIndexSockets;
				BuildIndexSockets.Index = SocketData.Index;
			
				FSocketInformation SocketInformation;
				for(const FName& SocketName : InstanceSocket.InstancedComponent->GetAllSocketNames())
				{
					SocketInformation.SocketName = SocketName.ToString();
					if(SocketName.IsEqual(SocketData.SocketName))
					{
						SocketInformation.bSocketInUse = true;
					}
					BuildIndexSockets.SocketsInformation.Add(SocketInformation);
				}
			
				InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
			}
		}
	}
	
	switch (BuildType)
	{
	case EBuildType::Foundation:
		FoundationInstancedMesh->AddInstanceWorldSpace(SocketData.SocketTransform);
		break;

	case EBuildType::Wall:
		WallInstancedMesh->AddInstanceWorldSpace(SocketData.SocketTransform);
		break;

	case EBuildType::Ceiling:
		CeilingInstancedMesh->AddInstanceWorldSpace(SocketData.SocketTransform);
		break;
	}
}

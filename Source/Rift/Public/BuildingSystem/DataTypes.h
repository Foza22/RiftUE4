#pragma once

#include "DataTypes.generated.h"

class UInstancedStaticMeshComponent;

UENUM(BlueprintType)
enum class EBuildType : uint8
{
	Foundation	UMETA(DisplayName = "Foundation"),
	Wall		UMETA(DisplayName = "Wall"),
	Ceiling		UMETA(DisplayName = "Ceiling")
};


USTRUCT(BlueprintType)
struct FBuildingVisualType
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Building)
	UStaticMesh* BuildingMesh;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	EBuildType BuildType;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	FName FilterCharacter; 
};

USTRUCT(BlueprintType)
struct FBuildingSocketData
{
	GENERATED_BODY()

	UInstancedStaticMeshComponent* InstancedComponent;
	int32 Index;
	FName SocketName;
	FTransform SocketTransform;
};

USTRUCT(BlueprintType)
struct FSocketInformation
{
	GENERATED_BODY()

	FName SocketName;
	bool bSocketInUse = false;
};

USTRUCT(BlueprintType)
struct FBuildingIndexSockets
{
	GENERATED_BODY()

	int32 Index;
	TArray<FSocketInformation> SocketsInformation;
};

USTRUCT(BlueprintType)
struct FInstanceSocketCheck
{
	GENERATED_BODY()

	UInstancedStaticMeshComponent* InstancedComponent;
	TArray<FBuildingIndexSockets> InstanceSocketInformation;
};
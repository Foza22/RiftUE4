// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "BaseBuilding.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class RIFT_API ABaseBuilding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseBuilding();

	UFUNCTION(BlueprintCallable, Category = Building)
	void DestroyInstance(FVector& HitPoint);

	UFUNCTION(BlueprintCallable, Category = Building)
	FTransform GetSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex,
	                              const FName& SocketName);

	int32 GetHitIndex(const FHitResult& HitResult) const;

	FBuildingSocketData GetHitSocketTransform(const FHitResult& HitResult,const FName& Filter, float ValidHitDistance = 25.f);

	void AddInstance(const FBuildingSocketData& SocketData, EBuildType BuildType);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	UInstancedStaticMeshComponent* FoundationInstancedMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	UInstancedStaticMeshComponent* WallInstancedMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	UInstancedStaticMeshComponent* CeilingInstancedMesh;

	TArray<FName> MeshInstanceSockets;

TArray<FInstanceSocketCheck> InstanceSocketsCheck;
	
	bool IsValidSocket(UInstancedStaticMeshComponent* HitComponent, const FName& SocketName,const FName& Filter) const;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

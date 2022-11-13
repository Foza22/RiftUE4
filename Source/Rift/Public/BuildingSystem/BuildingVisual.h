// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "BuildingVisual.generated.h"

class ABaseBuilding;
class UMaterialInstance;

UCLASS()
class RIFT_API ABuildingVisual : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingVisual();

	void SetBuildPosition(const FHitResult& HitResult, const FRotator& Rotation);

	void SpawnBuilding();

	void CycleMesh();

	// Overlap functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	UStaticMeshComponent* BuildMesh;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	TSubclassOf<ABaseBuilding> BuildingClass;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	TArray<FBuildingVisualType> BuildingTypes;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	UMaterialInstance* MaterialFalse;

	UPROPERTY(EditDefaultsOnly, Category = Building)
	UMaterialInstance* MaterialTrue;

	bool bMaterialIsTrue;

	uint8 BuildingTypeIndex;

	ABaseBuilding* GetHitBuildingActor(const FHitResult& HitResult) const;

	UPROPERTY()
	ABaseBuilding* InteractingBuilding;

	FBuildingSocketData SocketData;
	
	void SetMeshTo(EBuildType BuildType);
	void ReturnMeshToSelected();
	bool bReturnedMesh;
};

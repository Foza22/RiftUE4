// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	UInstancedStaticMeshComponent* FoundationInstancedMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

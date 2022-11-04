// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalCube.generated.h"

UCLASS()
class RIFT_API APortalCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalCube();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* PortalCube;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

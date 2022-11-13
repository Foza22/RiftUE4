// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;

UCLASS()
class RIFT_API ADoor : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//MeshComponents to represent Door assets
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Door;

	//BoxComponent which will be used as our Proximity volume.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* DoorProxVolume;

	// Door action depending on toggle door
	UFUNCTION()
	void OpenDoor(float DeltaTime);

	UFUNCTION()
	void CloseDoor(float DeltaTime);

	// Speed of door opening
	UPROPERTY(EditAnywhere, Category = Opening)
	float DoorOpeningSpeed = 150.0f; 

public:
	// Sets default values for this actor's properties
	ADoor();

	// Switch action with door
	void ToggleDoor(const FVector& ForwardVector);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// States of door
	bool IsClosed = true;
	bool IsOpening = false;
	bool IsClosing = false;

	// Parameters for door opening
	float InitialYaw = 0.f; // Base rotation of door
	float DotProd = 0.f; // Stores our location to door
	float MaxDegree = 0.f; // Based on DotProd stores angle of opened door (-90;90)
	float Direction = 0.f; // Based on DotProd stores direction of opening door
	float DoorCurrentRotation = 0.f; // Stores Yaw of door at moment
	float AddRotation = 0.f; // Rotation to add to door
};

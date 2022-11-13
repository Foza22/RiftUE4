// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalDoor.generated.h"

UCLASS()
class RIFT_API APortalDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	UStaticMeshComponent* BottomSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	UStaticMeshComponent* TopSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	UAudioComponent* OpenSound;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OpenDoor(float NewLocation) const;

	UFUNCTION()
	void CloseDoor(float NewLocation) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Opening)
	float DoorSpeed = 6.f;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* BottomSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
	UStaticMeshComponent* TopSurface;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OpenDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

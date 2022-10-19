// Fill out your copyright notice in the Description page of Project Settings.


#include "RiftGameModeBase.h"
#include "Player/MainCharacter.h"

ARiftGameModeBase::ARiftGameModeBase()
{
	// Use our main character by default
	DefaultPawnClass = AMainCharacter::StaticClass();
}

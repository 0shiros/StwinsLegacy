// Fill out your copyright notice in the Description page of Project Settings.


#include "VenoirArmor.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UVenoirArmor::UVenoirArmor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UVenoirArmor::ApplyEnhancement()
{
	PlayerCharacter->PlayerStats.MaxHealth *= 2.f; // PlayerCharacter->PlayerStats.MaxHealth = 200.f;
	PlayerCharacter->CurrentHealth = PlayerCharacter->PlayerStats.MaxHealth; // PlayerCharacter->CurrentHealth = 200.f;
}

void UVenoirArmor::RemoveEnhancement()
{
	PlayerCharacter->PlayerStats.MaxHealth /= 2.f; // PlayerCharacter->PlayerStats.MaxHealth = 100.f;
	PlayerCharacter->CurrentHealth = FMath::Min(PlayerCharacter->CurrentHealth, PlayerCharacter->PlayerStats.MaxHealth); // PlayerCharacter->CurrentHealth = FMath::Min(200.f,
}


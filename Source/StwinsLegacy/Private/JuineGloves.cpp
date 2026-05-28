// Fill out your copyright notice in the Description page of Project Settings.


#include "JuineGloves.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UJuineGloves::UJuineGloves()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UJuineGloves::ApplyEnhancement()
{
	PlayerCharacter->PlayerStats.AttackSpeedMultipliers[EAttackType::Basic] *= 1.5f; // Example: Increase basic attack speed by 50%
}

void UJuineGloves::RemoveEnhancement()
{
	PlayerCharacter->PlayerStats.AttackSpeedMultipliers[EAttackType::Basic] /= 1.5f; // Revert the attack speed increase
}


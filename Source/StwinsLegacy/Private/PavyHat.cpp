// Fill out your copyright notice in the Description page of Project Settings.


#include "PavyHat.h"
#include "PlayerCharacter.h"


// Sets default values for this component's properties
UPavyHat::UPavyHat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPavyHat::ApplyEnhancement()
{
	PlayerCharacter->PlayerStats.AttackCooldowns[EAttackType::Special] *= 0.5f; // Reduce special attack cooldown by 50%
}

void UPavyHat::RemoveEnhancement()
{
	PlayerCharacter->PlayerStats.AttackCooldowns[EAttackType::Special] /= 0.5f; // Revert special attack cooldown reduction
}



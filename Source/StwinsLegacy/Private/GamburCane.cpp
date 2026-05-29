// Fill out your copyright notice in the Description page of Project Settings.


#include "GamburCane.h"
#include "PlayerCharacter.h"


// Sets default values for this component's properties
UGamburCane::UGamburCane()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGamburCane::ApplyEnhancement()
{
	PlayerCharacter->PlayerStats.BaseAttack *= 2.f; // Double the base attack of the player character
	for (TPair<EAttackType, float>& AttackRangeMultiplier : PlayerCharacter->PlayerStats.AttackRangeMultipliers)
	{
		AttackRangeMultiplier.Value *= 2.f; // Double the attack range multiplier for all attack types
	}
}

void UGamburCane::RemoveEnhancement()
{
	PlayerCharacter->PlayerStats.BaseAttack /= 2.f; // Revert the base attack to its original value
	for (TPair<EAttackType, float>& AttackRangeMultiplier : PlayerCharacter->PlayerStats.AttackRangeMultipliers)
	{		
		AttackRangeMultiplier.Value /= 2.f; // Revert the attack range multiplier to its original value for all attack types
	}
}


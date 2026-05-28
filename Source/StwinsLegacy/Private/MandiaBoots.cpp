// Fill out your copyright notice in the Description page of Project Settings.


#include "MandiaBoots.h"

#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UMandiaBoots::UMandiaBoots()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UMandiaBoots::ApplyEnhancement()
{
	PlayerCharacter->PlayerStats.BaseSpeed *= 1.5f; // Example: Increase player speed by 50%
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= PlayerCharacter->PlayerStats.BaseSpeed; // Update the character's movement speed based on the new BaseSpeed
	PlayerCharacter->PlayerStats.BaseDashDistance *= 1.5f; // Increase dash distance by 50%
}

void UMandiaBoots::RemoveEnhancement()
{
	PlayerCharacter->PlayerStats.BaseSpeed /= 1.5f; // Revert player speed to original value
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed /= PlayerCharacter->PlayerStats.BaseSpeed; // Revert dash distance to original value
	PlayerCharacter->PlayerStats.BaseDashDistance /= 1.5f; // Revert dash distance to normal
}


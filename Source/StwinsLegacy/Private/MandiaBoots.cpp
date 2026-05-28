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
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 1.5f; // Increase movement speed by 50%
	PlayerCharacter->PlayerStats.BaseDashDistance *= 1.5f; // Increase dash distance by 50%
}

void UMandiaBoots::RemoveEnhancement()
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed /= 1.5f; // Revert movement speed to normal
	PlayerCharacter->PlayerStats.BaseDashDistance /= 1.5f; // Revert dash distance to normal
}


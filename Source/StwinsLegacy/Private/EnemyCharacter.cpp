// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyData.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyData)
	{
		InitialiseCharacterStats();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Data Not Set"));
	}
}

void AEnemyCharacter::InitialiseCharacterStats()
{
	EnemyStats = EnemyData->EnemyStats;
	GetCharacterMovement()->MaxWalkSpeed = EnemyStats.BaseSpeed;	
}

void AEnemyCharacter::TakeDamage(float DamageAmount, float KnockbackForce)
{	
	CurrentHealth -= DamageAmount;
	LaunchCharacter(-GetActorForwardVector() * KnockbackForce, true, true);
	
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy %s Defeated"), *GetName()));
		//Destroy();
	}
}


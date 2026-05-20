// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AIControllerEnemy.h"
#include "EnemyData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Set up AI Controller
	AIControllerClass = AAIControllerEnemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;
	GetCharacterMovement()->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = true;
	GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance = 120.f;
	
	// Create and configure the spawn particle effect component
	SpawnParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpawnParticleEffect"));
	SpawnParticleEffect->SetupAttachment(GetMesh());
	SpawnParticleEffect->bAutoActivate = false;
	SpawnParticleEffect->bAutoDestroy = false;
}

void AEnemyCharacter::InitialiseCharacterStats(UEnemyData* EnemyData)
{
	GetMesh()->SetVisibility(false);
	EnemyStats = EnemyData->EnemyStats;
	GetCharacterMovement()->MaxWalkSpeed = EnemyStats.BaseSpeed;	
	CurrentHealth = EnemyStats.MaxHealth;
	
	SpawnParticleEffect->SetTemplate(EnemyData->SpawnParticleEffect);	
	SpawnParticleEffect->Activate(true);
	
	FTimerHandle UnusedHandle;
	
	GetWorldTimerManager().SetTimer(
			UnusedHandle,
			this,
			&AEnemyCharacter::OnSpawnFinished,
			EnemyData->SpawnParticleEffectDuration,
			false
		);		
}

void AEnemyCharacter::TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection)
{	
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Enemy %s Already Defeated"), *GetName()));
		return;
	}
	
    CurrentHealth = FMath::Max(0.f, CurrentHealth - DamageAmount);
	LaunchCharacter(KnockbackDirection * KnockbackForce, true, true);
	
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy %s Defeated"), *GetName()));
		//Destroy();
	}
}

void AEnemyCharacter::OnSpawnFinished()
{
	GetMesh()->SetVisibility(true);
	SpawnParticleEffect->Deactivate();
}


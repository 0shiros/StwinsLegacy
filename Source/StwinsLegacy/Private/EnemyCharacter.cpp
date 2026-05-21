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
	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpawnParticleEffect"));
	ParticleEffect->SetupAttachment(GetMesh());
	ParticleEffect->bAutoActivate = false;
	ParticleEffect->bAutoDestroy = false;
}

void AEnemyCharacter::InitialiseCharacterStats(UEnemyData* EnemyData)
{
	EnemyStats = EnemyData->EnemyStats;
	GetCharacterMovement()->MaxWalkSpeed = EnemyStats.BaseSpeed;	
	CurrentHealth = EnemyStats.MaxHealth;
	
	SpawnAnimation(EnemyData);
}

void AEnemyCharacter::SpawnAnimation(UEnemyData* EnemyData)
{	
	GetMesh()->SetVisibility(false);
	ParticleEffect->SetTemplate(EnemyData->SpawnParticleEffect);	
	ParticleEffect->Activate(true);

	FTimerHandle UnusedHandle;

	GetWorldTimerManager().SetTimer(
	UnusedHandle,
	this,
	&AEnemyCharacter::OnSpawnFinished,
	EnemyData->SpawnParticleEffectDuration,
	false
	);	
}

void AEnemyCharacter::TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection, float StunDuration)
{	
	Super::TakeDamage(DamageAmount, KnockbackForce, KnockbackDirection, StunDuration);
}

void AEnemyCharacter::OnSpawnFinished()
{
	GetMesh()->SetVisibility(true);
	ParticleEffect->Deactivate();
}

void AEnemyCharacter::EnableActions()
{
	Super::EnableActions();
}

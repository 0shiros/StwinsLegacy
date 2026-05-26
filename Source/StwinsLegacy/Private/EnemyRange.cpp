// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRange.h"

#include "EnemyData.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AEnemyRange::AEnemyRange()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyRange::SpawnAnimation()
{
	Super::SpawnAnimation();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Ranged Enemy Spawn Animation"));
}

void AEnemyRange::AttackAnimationNotify()
{
	if (!IsValid(RangeAttackMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Range Attack Montage Not Valid"));
		return;
	}
	
	PlayAnimMontage(RangeAttackMontage, EnemyData->EnemyStats.AttackSpeed);	
}

void AEnemyRange::Attack()
{	
	Super::Attack();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ranged Enemy Attacking"));
	
	UWorld* World = GetWorld();
	if (!World || !EnemyData || !EnemyData->EnemyStats.ProjectileClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("World, EnemyData, or ProjectileClass is null in Ranged Enemy Attack"));
		return;
	}
	
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * EnemyData->EnemyStats.ProjectileSpawnOffSet;
	
	FRotator SpawnRotation = (PlayerReference->GetActorLocation() - GetActorLocation()).Rotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(EnemyData->EnemyStats.ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams))
	{
		SpawnedProjectile->EnemyData = EnemyData;
		SpawnedProjectile->ProjectileMovement->InitialSpeed = EnemyData->EnemyStats.ProjectileSpeed;
		SpawnedProjectile->ProjectileMovement->MaxSpeed = EnemyData->EnemyStats.ProjectileSpeed;
		SpawnedProjectile->ProjectileMovement->Velocity = SpawnRotation.Vector() * EnemyData->EnemyStats.ProjectileSpeed;
	}	
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "CleoirBody.h"
#include "EnemyData.h"
#include "MyGameInstance.h"
#include "NavigationSystem.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Set up AI Controller
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

void AEnemyCharacter::InitialiseCharacterStats()
{
	DisableActions();
	GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	EnemyStats = EnemyData->EnemyStats;
	GetCharacterMovement()->MaxWalkSpeed = EnemyStats.BaseSpeed;	
	CurrentHealth = EnemyStats.MaxHealth;
	
	SpawnAnimation();
}

void AEnemyCharacter::SpawnAnimation()
{	
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

void AEnemyCharacter::Attack()
{
	OrientEnemyToTarget();

	if (UCleoirBody* CleoirBody = Cast<UCleoirBody>(PlayerReference->GetComponentByClass(UCleoirBody::StaticClass())))
	{
		CleoirBody->bHasPlayerBeenTouched = true;
	}
}

void AEnemyCharacter::OrientEnemyToTarget()
{
	if (!PlayerReference) return;

	FVector DirectionToTarget = PlayerReference->GetActorLocation() - GetActorLocation();
	DirectionToTarget.Z = 0.f;

	if (!DirectionToTarget.IsNearlyZero())
	{
		FRotator TargetRotation = DirectionToTarget.Rotation();
		SetActorRotation(TargetRotation);
	}
}

FVector AEnemyCharacter::Fleeing(APlayerCharacter* PlayerCharacter)
{
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const FVector EnemyLocation = GetActorLocation();

	FVector FleeDirection = (EnemyLocation - PlayerLocation).GetSafeNormal();
	FleeDirection.Z = EnemyLocation.Z; 

	const float FleeDistance = EnemyStats.AttackRanges[EnemyStats.EnemyType] * EnemyStats.MultiplierAttackRangeToFlee;

	const FVector FleeOrigin = EnemyLocation + (FleeDirection * FleeDistance);

	if (const TObjectPtr<UNavigationSystemV1> NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation RandomFleePoint;

		if (NavSys->GetRandomPointInNavigableRadius(FleeOrigin, 200.f, RandomFleePoint))
		{
			return RandomFleePoint.Location;
		}
	}
	
	// If no valid point is found, return a point directly opposite the player at the desired flee distance
	FVector FleePoint = EnemyLocation + (FleeDirection * FleeDistance);
	FleePoint.Z = EnemyLocation.Z;	
	
	return FleePoint;
}

void AEnemyCharacter::OnSpawnFinished()
{
	ParticleEffect->Deactivate();
	EnableActions();
}

void AEnemyCharacter::Death()
{	
	GameInstance->AddSouls(EnemyData->SoulsDropped);
	GameInstance->AddScore(EnemyData->Score);
	Destroy();
}

void AEnemyCharacter::DisableActions()
{
	Super::DisableActions();
	
	OnCanMove.ExecuteIfBound(false);
	OnCanAttack.ExecuteIfBound(false);
}

void AEnemyCharacter::EnableActions()
{
	Super::EnableActions();
	
	OnCanMove.ExecuteIfBound(true);
	OnCanAttack.ExecuteIfBound(true);
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnDestroyed.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

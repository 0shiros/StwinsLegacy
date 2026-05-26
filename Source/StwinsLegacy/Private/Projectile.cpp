// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "EnemyData.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollider"));
	RootComponent = ProjectileCollider;
	ProjectileCollider->SetCollisionProfileName(TEXT("EnemyProjectile"));
	ProjectileCollider->SetEnableGravity(false);
		
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	ProjectileCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileOverlap);
}

void AProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (!EnemyData)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EnemyData is null in Projectile OnProjectileOverlap"));
			return;
		}
		
		Player->TakeDamage(EnemyData->EnemyStats.AttackMultiplier * EnemyData->EnemyStats.BaseAttack, EnemyData->EnemyStats.KnockbackForce, GetActorForwardVector(), EnemyData->EnemyStats.StunDuration);
		Destroy();
	}
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ProjectileCollider->OnComponentBeginOverlap.RemoveDynamic(this, &AProjectile::OnProjectileOverlap);
	
	Super::EndPlay(EndPlayReason);
}


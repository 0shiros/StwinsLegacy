// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"

#include "EnemyData.h"


// Sets default values
AEnemyBoss::AEnemyBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyBoss::SpawnAnimation()
{
	Super::SpawnAnimation();
}

void AEnemyBoss::AttackAnimationNotify()
{
	if (!IsValid(BossAttackMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Boss Attack Montage Not Valid"));
		return;
	}
	
	PlayAnimMontage(BossAttackMontage, EnemyData->EnemyStats.AttackSpeed);	
}

void AEnemyBoss::Attack()
{
	Super::Attack();
}


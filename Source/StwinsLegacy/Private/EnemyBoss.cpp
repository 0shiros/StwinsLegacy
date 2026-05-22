// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"


// Sets default values
AEnemyBoss::AEnemyBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyBoss::SpawnAnimation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Boss Spawn Animation"));
}

void AEnemyBoss::Attack(APlayerCharacter* Target)
{
	Super::Attack(Target);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Boss Attacking"));
}


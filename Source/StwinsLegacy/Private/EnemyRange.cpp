// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRange.h"


// Sets default values
AEnemyRange::AEnemyRange()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyRange::SpawnAnimation(UEnemyData* EnemyData)
{
	Super::SpawnAnimation(EnemyData);
}

void AEnemyRange::Attack()
{
	
}


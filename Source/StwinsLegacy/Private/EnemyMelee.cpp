// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMelee.h"


// Sets default values
AEnemyMelee::AEnemyMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyMelee::SpawnAnimation(UEnemyData* EnemyData)
{
	Super::SpawnAnimation(EnemyData);
}

void AEnemyMelee::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Method of child class %s called"), *GetName()));
}



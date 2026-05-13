// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.h"
#include "EnemyCharacterStats.generated.h"

USTRUCT(BlueprintType)
struct STWINSLEGACY_API FEnemyCharacterStats : public FBaseCharacterStats
{
	GENERATED_BODY()
	
	//Melee
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")	
	float MeleeAttackMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackCooldownMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackRange = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackAngle = 60.f;
	
	//Ranged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<class AProjectile> ProjectileClass;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjectileSpeed = 1000.f;	
};
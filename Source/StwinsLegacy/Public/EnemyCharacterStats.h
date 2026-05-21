// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.h"
#include "EnemyCharacterStats.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
	Boss UMETA(DisplayName = "Boss")	
};

USTRUCT(BlueprintType)
struct STWINSLEGACY_API FEnemyCharacterStats : public FBaseCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EEnemyType EnemyType = EEnemyType::Melee;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StunDuration = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EEnemyType, float> DistanceToAttack = {
		{EEnemyType::Melee, 125.f},
		{EEnemyType::Ranged, 575.f},
		{EEnemyType::Boss, 175.f}
	};	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EEnemyType, float> AttackRanges = {
		{EEnemyType::Melee, 150.f},
		{EEnemyType::Ranged, 600.f},
		{EEnemyType::Boss, 200.f}
	};	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")	
	float AttackMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackCooldownMultiplier = 1.f;
		
	//Melee
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackRadius = 60.f;
	
	//Ranged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<class AProjectile> ProjectileClass;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjectileSpeed = 1000.f;		
};
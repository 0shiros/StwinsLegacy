// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackType.h"
#include "BaseCharacterStats.h"
#include "PlayerCharacterStats.generated.h"

USTRUCT(BlueprintType)
struct STWINSLEGACY_API FPlayerCharacterStats : public FBaseCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BaseDashDistance = 2000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCooldown = 1.0f;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCooldownMultiplier = 1.0f;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackMultipliers = {
		{EAttackType::Basic, 1.f},
		{EAttackType::Heavy, 2.f},
		{EAttackType::Special, 3.f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackCooldowns = {
		{EAttackType::Basic, 1.0f},
		{EAttackType::Heavy, 2.0f},
		{EAttackType::Special, 5.0f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackCooldownMultipliers = {
		{EAttackType::Basic, 1.0f},
		{EAttackType::Heavy, 1.0f},
		{EAttackType::Special, 1.0f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackRadius = {
		{EAttackType::Basic, 40.f},
		{EAttackType::Heavy, 90.f},
		{EAttackType::Special, 120.f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackRanges = {
		{EAttackType::Basic, 200.f},
		{EAttackType::Heavy, 300.f},
		{EAttackType::Special, 500.f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackRangeMultipliers = {
		{EAttackType::Basic, 1.f},
		{EAttackType::Heavy, 1.f},
		{EAttackType::Special, 1.f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> KnockbackForces = {
		{EAttackType::Basic, 100.f},
		{EAttackType::Heavy, 200.f},
		{EAttackType::Special, 500.f}
	};
};

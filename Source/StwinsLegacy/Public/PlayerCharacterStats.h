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
	int BaseDashDistance = 4000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCooldown = 0.5f;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCooldownMultiplier = 1.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackMultipliers = {
		{EAttackType::Basic, 1.f},
		{EAttackType::Heavy, 2.f},
		{EAttackType::Special, 3.f}
	};
	
	//Animation speed multipliers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackCooldowns = {
		{EAttackType::Basic, 0.f}, 
		{EAttackType::Heavy, 1.0f}, 
		{EAttackType::Special, 5.0f}
	};
	
	//Will disapear with animation montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackSpeedMultipliers = {
		{EAttackType::Basic, 1.0f}, 
		{EAttackType::Heavy, 1.0f},
		{EAttackType::Special, 1.0f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackRadius = {
		{EAttackType::Basic, 60.f},
		{EAttackType::Heavy, 90.f},
		{EAttackType::Special, 180.f}
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EAttackType, float> AttackRanges = {
		{EAttackType::Basic, 300.f},
		{EAttackType::Heavy, 450.f},
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
		{EAttackType::Basic, 1000.f},
		{EAttackType::Heavy, 2000.f},
		{EAttackType::Special, 4000.f}
	};
};

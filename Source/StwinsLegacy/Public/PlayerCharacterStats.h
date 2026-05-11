// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.h"
#include "PlayerCharacterStats.generated.h"

USTRUCT(BlueprintType)
struct STWINSLEGACY_API FPlayerCharacterStats : public FBaseCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BaseDashDistance = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCooldown = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HeavyAttackDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HeavyAttackRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int HeavyAttackRange = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpecialAttackDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpecialAttackRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int SpecialAttackRange = 100;
};

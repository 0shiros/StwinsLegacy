// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.generated.h"

USTRUCT(BlueprintType)
struct STWINSLEGACY_API FBaseCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BaseHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseSpeed = 800.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BaseAttack = 10;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BasicAttackDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BasicAttackRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BasicAttackRange = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BasicAttackMultiplier = 1;
};

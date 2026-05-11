// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.h"
#include "EnemyCharacterStats.generated.h"

USTRUCT(BlueprintType)
struct STWINSLEGACY_API FEnemyCharacterStats : public FBaseCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<class AProjectile> ProjectileClass;	
};
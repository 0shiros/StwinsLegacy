// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.h"
#include "EnemyCharacterStats.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

UCLASS(BlueprintType)
class STWINSLEGACY_API UEnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName EnemyName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleAnimation")
	TObjectPtr<UParticleSystem> SpawnParticleEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleAnimation")
	float SpawnParticleEffectDuration = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Score = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int SoulsDropped = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyCharacterStats EnemyStats;
};

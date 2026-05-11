// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterStats.h"
#include "EnemyCharacterStats.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

UCLASS()
class STWINSLEGACY_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName EnemyName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Score = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int SoulsDropped = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyCharacterStats CharacterStats;
};

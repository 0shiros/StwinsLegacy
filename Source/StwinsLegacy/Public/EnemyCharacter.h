// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacterStats.h"
#include "EnemyCharacter.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<class UEnemyData> EnemyData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyCharacterStats EnemyStats;
	
protected:
	virtual void BeginPlay() override;
	
	void InitialiseCharacterStats();

public:
	virtual void TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection) override;
};

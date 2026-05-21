// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyMelee.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API AEnemyMelee : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyMelee();
	
protected:
	
	virtual void SpawnAnimation(UEnemyData* EnemyData) override;
	
public:
	
	virtual void Attack() override;
};

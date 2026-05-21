// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyBoss.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API AEnemyBoss : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBoss();
	
protected:
	
	virtual void SpawnAnimation(UEnemyData* EnemyData) override;
	
public :
	
	virtual void Attack() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyRange.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API AEnemyRange : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyRange();

protected:
	
	virtual void SpawnAnimation() override;

public :
	
	virtual void Attack() override;
};

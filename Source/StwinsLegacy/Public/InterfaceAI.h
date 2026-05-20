// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacterStats.h"
#include "UObject/Interface.h"
#include "InterfaceAI.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterfaceAI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STWINSLEGACY_API IInterfaceAI
{
	GENERATED_BODY()

public:
	
	virtual EEnemyType GetEnemyType() const = 0;
};

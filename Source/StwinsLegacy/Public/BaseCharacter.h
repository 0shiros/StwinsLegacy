// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API ABaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	ABaseCharacter();		
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentHealth = 100;
	
	virtual void TakeDamage(float DamageAmount) override PURE_VIRTUAL(ABaseCharacter::TakeDamage, );
};

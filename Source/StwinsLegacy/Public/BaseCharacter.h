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
	
	// States
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bCanMove = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bCanAttack = false;
	
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentHealth = 100;
		
	// Combat
	virtual void TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection, float StunDuration) override;
	
	virtual void EnableActions();
};

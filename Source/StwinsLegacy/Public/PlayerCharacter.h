// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacterStats.h"
#include "PlayerCharacter.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> PlayerCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")	
	TObjectPtr<class USpringArmComponent> SpringArm;
			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerCharacterStats PlayerStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	TObjectPtr<class UMyGameInstance> GameInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerController")
	TObjectPtr<class APCGame> PlayerController;		
	
private :
	
	TMap<EAttackType, float> LastAttackTimes;
	
	float LastDashTime = -100.f;
	
protected:
	
	virtual void BeginPlay() override;
	
	void InitialiseCharacterStats();
	
public:	
	
	bool CanDash();
	
	bool CanAttack(EAttackType AttackType);	
	
	void Attack(EAttackType AttackType);
	
	void virtual TakeDamage(float DamageAmount, float KnockbackForce) override;
};

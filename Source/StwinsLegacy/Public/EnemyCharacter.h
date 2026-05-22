// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacterStats.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;

DECLARE_DELEGATE_OneParam(FOnCanMove, const bool);
DECLARE_DELEGATE_OneParam(FOnCanAttack, const bool);

UCLASS(Abstract)
class STWINSLEGACY_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();		
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	APlayerCharacter* PlayerReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ParticleAnimation")
	TObjectPtr<UParticleSystemComponent> ParticleEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	TObjectPtr<class UEnemyData> EnemyData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyCharacterStats EnemyStats;
	
	// Delegate 
	FOnCanMove OnCanMove;
	FOnCanAttack OnCanAttack;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitialiseCharacterStats();
	
	virtual void SpawnAnimation();	
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void AttackAnimationNotify() PURE_VIRTUAL(AEnemyCharacter::AttackAnimationNotify, );
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack();
	
	void OrientEnemyToTarget();
	
	virtual FVector Fleeing(APlayerCharacter* PlayerCharacter);
		
	EEnemyType GetEnemyType() const {return EnemyStats.EnemyType;}
	
	UFUNCTION()
	void OnSpawnFinished();	
	
	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void Death() override;
	
	virtual void DisableActions() override;
	
	virtual void EnableActions() override;
};

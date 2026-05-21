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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ParticleAnimation")
	TObjectPtr<UParticleSystemComponent> ParticleEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyCharacterStats EnemyStats;	
	
public:
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitialiseCharacterStats(class UEnemyData* EnemyData);
	
	virtual void SpawnAnimation(UEnemyData* EnemyData);	
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack() PURE_VIRTUAL(AEnemyCharacter::Attack, );
	
	virtual void TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection, float StunDuration) override;
	
	EEnemyType GetEnemyType() const {return EnemyStats.EnemyType;}
	
	UFUNCTION()
	void OnSpawnFinished();	
	
	virtual void EnableActions() override;
};

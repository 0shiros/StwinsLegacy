// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacterStats.h"
#include "InterfaceAI.h"
#include "EnemyCharacter.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API AEnemyCharacter : public ABaseCharacter, public IInterfaceAI
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ParticleAnimation")
	TObjectPtr<UParticleSystemComponent> SpawnParticleEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyCharacterStats EnemyStats;
	
protected:	
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitialiseCharacterStats(class UEnemyData* EnemyData);

public:
	virtual void TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection) override;
	
	virtual EEnemyType GetEnemyType() const override {return EnemyStats.EnemyType;}
	
	UFUNCTION()
	void OnSpawnFinished();
	
};

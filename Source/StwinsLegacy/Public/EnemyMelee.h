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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bSaveAttack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bIsAttacking;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	uint8 AttackComboCount;
	
	//Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationAttack")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	
protected:
	
	virtual void SpawnAnimation() override;
	
public:
	
	virtual void AttackAnimationNotify() override;
	
	virtual void Attack() override;
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ComboBasicAttackSave();
	
	void SwitchAnimMontage();
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ResetBasicAttackCombo();	
};

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
	
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> PlayerCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")	
	TObjectPtr<class USpringArmComponent> SpringArm;
			
	//Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerCharacterStats PlayerStats;
	
	//References	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	TObjectPtr<class UMyGameInstance> GameInstance;
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsAttacking = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bSaveAttack = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsHoldingAttack = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	uint8 AttackComboCount = 0;
	
	//Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationBasicAttack")
	TArray<TObjectPtr<UAnimMontage>> BasicAttackMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationHeavyAttack")
	TObjectPtr<UAnimMontage> HeavyAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSpecialAttack")
	TObjectPtr<UAnimMontage> SpecialAttackMontage;
		
private :
	
	TMap<EAttackType, float> LastAttackTimes;
	
	float LastDashTime = -100.f; //To allow instant dash at start
	
protected:
	
	virtual void BeginPlay() override;
	
	void InitialiseCharacterStats();
	
public:	
	
	bool CanDash();
	
	bool CanAttack(EAttackType AttackType);	
	
	//Basic Attack
	UFUNCTION(BlueprintCallable, Category = "BasicAttack")
	void BasicAttack(EAttackType AttackType);
	
	void BasicAttackAnimationNotify();
	
	UFUNCTION(BlueprintCallable, Category = "Basic Attack")
	void ComboBasicAttackSave();
	
	void SwitchAnimMontage();
	
	UFUNCTION(BlueprintCallable, Category = "Basic Attack")
	void ResetBasicAttackCombo();
	
	//Heavy Attack
	void HeavyAttackAnimationNotify();
	
	UFUNCTION(BlueprintCallable, Category = "HeavyAttack")
	void HeavyAttack(EAttackType AttackType);
	
	//Special Attack
	void SpecialAttackAnimationNotify();
	
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack")
	void SpecialAttack(EAttackType AttackType);
	
	void virtual TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ItemEnhanceData.h"
#include "PlayerCharacterStats.h"
#include "PlayerCharacter.generated.h"

class AStore;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStoreEntered, AStore*, Store, UItemEnhanceData*, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoreExited);

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TMap<EEnhancementItemType, TObjectPtr<UEnhancementItem>> ActiveEnhancements;
				
	//Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerCharacterStats PlayerStats;	
	
	float HealthTimer = 3.f;
	
	float LastTimeHeal = 0.f;
	
	//References	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	TObjectPtr<class UMyGameInstance> GameInstance;
			
	//States
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
	
	// Delegates
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintReadOnly, Category = "Delegates")
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintReadOnly, Category = "Delegates")
	FOnStoreEntered OnStoreEntered;
	
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintReadOnly, Category = "Delegates")
	FOnStoreExited OnStoreExited;
			
private :
	
	TMap<EAttackType, float> LastAttackTimes;
	
	float LastDashTime = -100.f; //To allow instant dash at start
	
protected:
	
	virtual void BeginPlay() override;
	
	void InitialiseCharacterStats();
	
public:	
	
	bool CanDash();
	
	bool CanAttack(EAttackType AttackType);	
	
	bool CanHeal();
	
	void Heal();
	
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
	
	virtual void TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection, float StunDuration) override;
	
	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void Death() override;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetCanMove(bool bNewCanMove) { bCanMove = bNewCanMove; }
		
	//HUD
	UFUNCTION(BlueprintCallable, Category = "UI")
	float UpdateHealthBar();
	
};

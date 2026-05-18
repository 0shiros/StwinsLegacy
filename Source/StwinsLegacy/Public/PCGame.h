// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCGame.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS(Abstract)
class STWINSLEGACY_API APCGame : public APlayerController
{
	GENERATED_BODY()
	
public :
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<class APlayerCharacter> PlayerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC")
	TObjectPtr<class UInputMappingContext> CharacterIMC;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> DashAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> BasicAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> HeavyAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SpecialAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> PauseMenuAction;
	
	FVector MoveInput;
	
	FVector AimDirection;
	
private:
	
	virtual void BeginPlay() override;	
	
	virtual void SetupInputComponent() override;	
	
public :	
	
	void Move(const FInputActionValue& Value);	
	
	void Dash(const FInputActionValue& Value);
	
	void OrientCharacterToAttack();
	
	void BasicAttack(const FInputActionValue& Value);
	
	void BasicAttackRelease(const FInputActionValue& Value);
	
	void HeavyAttack(const FInputActionValue& Value);
	
	void SpecialAttack(const FInputActionValue& Value);
	
	void Interact(const FInputActionValue& Value);
	
	void PauseMenu(const FInputActionValue& Value);	
};

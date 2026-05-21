// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerEnemy.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API AAIControllerEnemy : public AAIController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackboard")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<class AEnemyCharacter> ControlledEnemy;		
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
	TObjectPtr<class APlayerCharacter> PlayerCharacter;
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;

};

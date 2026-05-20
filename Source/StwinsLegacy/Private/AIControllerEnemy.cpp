// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerEnemy.h"

#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAIControllerEnemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledEnemy = Cast<AEnemyCharacter>(InPawn);
	
	RunBehaviorTree(BehaviorTree);
	
	EEnemyType EnemyType = ControlledEnemy->GetEnemyType();
	
	GetBlackboardComponent()->SetValueAsEnum(TypeEnemyKeyName, static_cast<uint8>(EnemyType));	
}

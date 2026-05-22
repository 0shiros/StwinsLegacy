// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerEnemy.h"

#include "EnemyCharacter.h"
#include "EnemyData.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


void AAIControllerEnemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledEnemy = Cast<AEnemyCharacter>(InPawn);	
	ControlledEnemy->OnCanMove.BindUObject(this, &AAIControllerEnemy::OnEnemyCanMove);
	ControlledEnemy->OnCanAttack.BindUObject(this, &AAIControllerEnemy::OnEnemyCanAttack);
	ControlledEnemy->InitialiseCharacterStats();
	
	EEnemyType EnemyType = ControlledEnemy->GetEnemyType();
	
	RunBehaviorTree(BehaviorTree);
	BlackboardComponent = GetBlackboardComponent();
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());		
	
	BlackboardComponent->SetValueAsObject(TEXT("AttackTarget"), PlayerCharacter);
	BlackboardComponent->SetValueAsFloat(TEXT("RangeToAttack"), ControlledEnemy->EnemyData->EnemyStats.DistanceToAttack[EnemyType]);
	BlackboardComponent->SetValueAsEnum(TEXT("EnemyType"), static_cast<uint8>(EnemyType));
	BlackboardComponent->SetValueAsFloat(TEXT("AttackCooldown"), ControlledEnemy->EnemyData->EnemyStats.AttackCooldown);
	
	FTimerHandle UnusedHandle;
	
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		this,
		&AAIControllerEnemy::UpdateDistanceToTarget,
		0.1f,
		true
	);
}

void AAIControllerEnemy::UpdateDistanceToTarget()
{
	if (!PlayerCharacter || !ControlledEnemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerCharacter or ControlledEnemy is null in UpdateDistanceToPlayer"));
		return;
	}
	
	float DistanceToPlayer = ControlledEnemy->GetDistanceTo(PlayerCharacter);
	BlackboardComponent->SetValueAsFloat(TEXT("DistanceToTarget"), DistanceToPlayer);
}

void AAIControllerEnemy::OnEnemyCanMove(const bool bCanMove)
{
	BlackboardComponent->SetValueAsBool(TEXT("CanMove"), bCanMove);
}

void AAIControllerEnemy::OnEnemyCanAttack(const bool bCanAttack)
{
	BlackboardComponent->SetValueAsBool(TEXT("CanAttack"), bCanAttack);
}

void AAIControllerEnemy::EnemyAttack()
{
	if (!PlayerCharacter || !ControlledEnemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerCharacter or ControlledEnemy is null in EnemyAttack"));
		return;
	}
	
	ControlledEnemy->AttackAnimationNotify();
}

void AAIControllerEnemy::EnemyFleePointChanged()
{
	if (!PlayerCharacter || !ControlledEnemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerCharacter or ControlledEnemy is null in EnemyFleePointChanged"));
		return;
	}
	
	FVector FleePoint = ControlledEnemy->Fleeing(PlayerCharacter);
	BlackboardComponent->SetValueAsVector(TEXT("FleePoint"), FleePoint);
}

void AAIControllerEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ControlledEnemy->OnCanMove.Unbind();
	ControlledEnemy->OnCanAttack.Unbind();
	
	Super::EndPlay(EndPlayReason);
}

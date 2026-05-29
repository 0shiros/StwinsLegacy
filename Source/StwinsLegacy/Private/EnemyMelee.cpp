// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMelee.h"

#include "Engine/OverlapResult.h"


// Sets default values
AEnemyMelee::AEnemyMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyMelee::SpawnAnimation()
{
	Super::SpawnAnimation();
}

void AEnemyMelee::AttackAnimationNotify()
{
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		SwitchAnimMontage();
	}
}

void AEnemyMelee::Attack()
{
	Super::Attack();
	
	float AttackRange = EnemyStats.AttackRanges[EnemyStats.EnemyType];
	float AttackRadius = EnemyStats.MeleeAttackRadius;
	float AttackMultiplier = EnemyStats.AttackMultiplier;
	
	TArray<FOverlapResult> OverlapResults;
    
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRange)
	);

	if (OverlapResults.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No Enemies Hit"));
		return;
	}
	
	TArray<TObjectPtr<AActor>> HitActors;
	
	FVector Forward = GetActorForwardVector();

	for (FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();

		if (!Actor || Actor == this) continue;

		FVector ToActor = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToActor)));

		if (AngleDeg <= AttackRadius)
		{
			HitActors.AddUnique(Actor);
		}
	}
	
	for (TObjectPtr HitActor : HitActors)
	{
		if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
		{
			FVector KnockbackDir = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Damageable->TakeDamage(EnemyStats.BaseAttack * AttackMultiplier,EnemyStats.KnockbackForce, KnockbackDir, EnemyStats.StunDuration);
		}
	}
	
	FVector RightEdge = GetActorLocation() + Forward.RotateAngleAxis( AttackRadius, FVector::UpVector) * AttackRange;
	FVector LeftEdge  = GetActorLocation()  + Forward.RotateAngleAxis(-AttackRadius, FVector::UpVector) * AttackRange;
	
	DrawDebugLine(GetWorld(), GetActorLocation() , RightEdge, FColor::Red, false, 0.f, 0, 2.f);
	DrawDebugLine(GetWorld(), GetActorLocation() , LeftEdge,  FColor::Red, false, 0.f, 0, 2.f);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Green, false, 0.5f, 0, 2.f);
}

void AEnemyMelee::ComboBasicAttackSave()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		SwitchAnimMontage();
	}
}

void AEnemyMelee::SwitchAnimMontage()
{
	if (AttackMontages.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AttackMontages array is empty in SwitchAnimMontage"));
		return;
	}
	
	UAnimMontage* Montage = AttackMontages[AttackComboCount];
	AttackComboCount = (AttackComboCount + 1) % AttackMontages.Num();
	PlayAnimMontage(Montage, EnemyStats.AttackSpeed);
}

void AEnemyMelee::ResetBasicAttackCombo()
{
	AttackComboCount = 0;
	bSaveAttack = false;
	bIsAttacking = false;
}





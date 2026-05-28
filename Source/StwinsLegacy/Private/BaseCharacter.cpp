// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseCharacter::TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection,
                                float StunDuration)
{
	if (Tags.Contains("Status.Invincible"))
	{
		Tags.Remove("Status.Invincible");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Character %s is invincible and ignored the damage."), *GetName()));
		return;	
	}
	
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Character %s is already dead."), *GetName()));
		Death();
		return;
	}
	
	LaunchCharacter(KnockbackDirection * KnockbackForce, true, true);
	CurrentHealth = FMath::Max(0.f, CurrentHealth - DamageAmount);
	
	if (CurrentHealth <= 0)
	{
		DeathAnimationNotify();
	}
		
	if (StunDuration > 0.f)
	{
		DisableActions();
		
		FTimerHandle UnusedHandle;
		
		GetWorldTimerManager().SetTimer(
			UnusedHandle,
			this,
			&ABaseCharacter::EnableActions,
			StunDuration,
			false
		);
	}
}

void ABaseCharacter::DeathAnimationNotify()
{
	if (!IsValid(DeathMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Death Montage Not Valid"));
		Death();
		return;
	}	
	
	PlayAnimMontage(DeathMontage);
	DisableActions();
}

void ABaseCharacter::DisableActions()
{	
	bCanMove = false;
	bCanAttack = false;
}

void ABaseCharacter::EnableActions()
{
	bCanMove = true;
	bCanAttack = true;
}


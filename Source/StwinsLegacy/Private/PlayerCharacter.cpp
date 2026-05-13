

#include "PlayerCharacter.h"

#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "PCGame.h"
#include "Camera/CameraComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);		
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PlayerController = Cast<APCGame>(GetController());
	
	if (GameInstance)
	{
		InitialiseCharacterStats();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game Instance Not Valid"));
	}
}

void APlayerCharacter::InitialiseCharacterStats()
{
	PlayerStats = GameInstance->GetCurrentSaveGame()->PlayerStats;
	CurrentHealth = PlayerStats.MaxHealth;
	GetCharacterMovement()->MaxWalkSpeed = PlayerStats.BaseSpeed;
	
	for (EAttackType AttackType : TEnumRange<EAttackType>())
	{
		LastAttackTimes.Add(AttackType, -100.f);
	}
	
	LastDashTime = -100.f;
}

bool APlayerCharacter::CanDash()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastDashTime >= PlayerStats.DashCooldown * PlayerStats.DashCooldownMultiplier)
	{
		LastDashTime = CurrentTime;
		return true;
	}
	
	return false;
}

bool APlayerCharacter::CanAttack(EAttackType AttackType)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastAttackTimes[AttackType] >= PlayerStats.AttackCooldowns[AttackType] * PlayerStats.AttackCooldownMultipliers[AttackType])
	{
		LastAttackTimes[AttackType] = CurrentTime;
		return true;
	}
	
	return false;
}

void APlayerCharacter::Attack(EAttackType AttackType)
{
	if (!CanAttack(AttackType))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Attack On Cooldown"));
		return;
	}
	
	float AttackRange = PlayerStats.AttackRanges[AttackType] * PlayerStats.AttackRangeMultipliers[AttackType];
	float AttackRadius = PlayerStats.AttackRadius[AttackType];
	float AttackMultiplier = PlayerStats.AttackMultipliers[AttackType];
	
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
	
	FVector RightBoundary = GetActorForwardVector()
	.RotateAngleAxis(AttackRadius, FVector::UpVector);
	FVector LeftBoundary = GetActorForwardVector()
		.RotateAngleAxis(-AttackRadius, FVector::UpVector);

	
	TArray<TObjectPtr<AActor>> HitActors;
	
    for (FOverlapResult& Result : OverlapResults)
    {
        AActor* Actor = Result.GetActor();
    	
        if (!Actor || Actor == this) continue;
    	
        FVector ToActor = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

        bool bRightOfLeft  = FVector::CrossProduct(LeftBoundary,  ToActor).Z >= 0;
        bool bLeftOfRight  = FVector::CrossProduct(RightBoundary, ToActor).Z <= 0;

        if (bRightOfLeft && bLeftOfRight)
        {
            HitActors.AddUnique(Actor);
        }
    }
	
	for (TObjectPtr HitActor : HitActors)
	{
		if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
		{
			Damageable->TakeDamage(PlayerStats.BaseAttack * AttackMultiplier);
		}
	}
	
    DrawDebugLine(GetWorld(), GetActorLocation(),
        GetActorLocation() + RightBoundary * AttackRange,
        FColor::Green, false, 1.f);
	
    DrawDebugLine(GetWorld(), GetActorLocation(),
        GetActorLocation() + LeftBoundary * AttackRange,
        FColor::Green, false, 1.f);
}

void APlayerCharacter::TakeDamage(float DamageAmount)
{	
	CurrentHealth -= DamageAmount;
	
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Died"));
	}
}




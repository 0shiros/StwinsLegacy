

#include "PlayerCharacter.h"

#include "MyGameInstance.h"
#include "MySaveGame.h"
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Attack %d Executed"), (int)AttackType));
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
			Damageable->TakeDamage(PlayerStats.BaseAttack * AttackMultiplier, PlayerStats.KnockbackForces[AttackType], KnockbackDir);
		}
	}
	
	FVector RightEdge = GetActorLocation() + Forward.RotateAngleAxis( AttackRadius, FVector::UpVector) * AttackRange;
	FVector LeftEdge  = GetActorLocation()  + Forward.RotateAngleAxis(-AttackRadius, FVector::UpVector) * AttackRange;

	DrawDebugLine(GetWorld(), GetActorLocation() , RightEdge, FColor::Red, false, 0.f, 0, 2.f);
	DrawDebugLine(GetWorld(), GetActorLocation() , LeftEdge,  FColor::Red, false, 0.f, 0, 2.f);
}

void APlayerCharacter::BasicAttackAnimationNotify()
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

void APlayerCharacter::ComboBasicAttackSave()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		SwitchAnimMontage();
	}
}

void APlayerCharacter::SwitchAnimMontage()
{
	switch (AttackComboCount)
	{
	case 0:
		AttackComboCount = 1;
		PlayAnimMontage(BasicAttackMontages[0]);
		break;
	case 1:
		AttackComboCount = 2;
		PlayAnimMontage(BasicAttackMontages[1]);
		break;
	case 2:
		AttackComboCount = 3;
		PlayAnimMontage(BasicAttackMontages[2]);
		break;
	case 3: 
		AttackComboCount = 0;
		PlayAnimMontage(BasicAttackMontages[3]);
		break;
	}
}

void APlayerCharacter::ResetBasicAttackCombo()
{
	bIsAttacking = false;
	bSaveAttack = false;
	AttackComboCount = 0;
}

void APlayerCharacter::TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection)
{	
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Player Character Already Dead"));
		return;
	}
	
	CurrentHealth = FMath::Max(0.f, CurrentHealth - DamageAmount);
	
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Died"));
	}
}




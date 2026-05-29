

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
	OnHealthChanged.Broadcast(UpdateHealthBar());
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
	if (!bCanAttack)
	{
		return false;
	}
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastAttackTimes[AttackType] >= PlayerStats.AttackCooldowns[AttackType])
	{
		LastAttackTimes[AttackType] = CurrentTime;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Can Attack: %s"), *UEnum::GetValueAsString(AttackType)));
		return true;
	}
	
	return false;
}

bool APlayerCharacter::CanHeal()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastTimeHeal >= HealthTimer && CurrentHealth < PlayerStats.MaxHealth)
	{
		LastTimeHeal = CurrentTime;
		return true;
	}
	
	return false;
}

void APlayerCharacter::Heal()
{
	if (!CanHeal())
	{
		return;
	}
	
	CurrentHealth = PlayerStats.MaxHealth;
	OnHealthChanged.Broadcast(UpdateHealthBar());
}

void APlayerCharacter::BasicAttack(EAttackType AttackType)
{			
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
			Damageable->TakeDamage(PlayerStats.BaseAttack * AttackMultiplier, PlayerStats.KnockbackForces[AttackType], KnockbackDir, PlayerStats.StunDurations[AttackType]);
		}
	}
	
	//FVector RightEdge = GetActorLocation() + Forward.RotateAngleAxis( AttackRadius, FVector::UpVector) * AttackRange;
	//FVector LeftEdge  = GetActorLocation()  + Forward.RotateAngleAxis(-AttackRadius, FVector::UpVector) * AttackRange;

	//DrawDebugLine(GetWorld(), GetActorLocation() , RightEdge, FColor::Red, false, 0.f, 0, 2.f);
	//DrawDebugLine(GetWorld(), GetActorLocation() , LeftEdge,  FColor::Red, false, 0.f, 0, 2.f);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Green, false, 0.5f, 0, 2.f);
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
		return;
	}
	
	if (bIsHoldingAttack)
	{
		SwitchAnimMontage();
	}
}

void APlayerCharacter::SwitchAnimMontage()
{	
	if (BasicAttackMontages.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Basic Attack Montages Not Valid"));
		return;
	}
	UAnimMontage* Montage = BasicAttackMontages[AttackComboCount];
	AttackComboCount = (AttackComboCount + 1) % BasicAttackMontages.Num();
	PlayAnimMontage(Montage, PlayerStats.AttackSpeedMultipliers[EAttackType::Basic]);	
}

void APlayerCharacter::ResetBasicAttackCombo()
{
	bIsAttacking = false;
	bSaveAttack = false;
	AttackComboCount = 0;
}

void APlayerCharacter::HeavyAttackAnimationNotify()
{	
	if (!IsValid(HeavyAttackMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Heavy Attack Montage Not Valid"));
		return;
	}
	
	PlayAnimMontage(HeavyAttackMontage, PlayerStats.AttackSpeedMultipliers[EAttackType::Heavy]);		
}

void APlayerCharacter::HeavyAttack(EAttackType AttackType)
{
	FVector Origin;
	FVector BoxExtent;
	GetMesh()->Bounds.GetBox().GetCenterAndExtents(Origin, BoxExtent);
	float Height = BoxExtent.Z * 2.0f;
	float AttackRange = PlayerStats.AttackRanges[AttackType] * PlayerStats.AttackRangeMultipliers[AttackType];
	float AttackRadius = PlayerStats.AttackRadius[AttackType];
	
	float AttackMultiplier = PlayerStats.AttackMultipliers[AttackType];
	
	FQuat Rotation = GetActorQuat();
	
	TArray<FOverlapResult> OverlapResults;
	
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		Rotation,
		ECC_Pawn,
		FCollisionShape::MakeBox(FVector(AttackRange, AttackRadius, Height))
	);
	
	if (OverlapResults.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No Enemies Hit"));
		return;
	}
	
	TArray<TObjectPtr<AActor>> HitActors;
		
	for (FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();

		if (!Actor || Actor == this) continue;

		HitActors.AddUnique(Actor);
	}
	
	for (TObjectPtr HitActor : HitActors)
	{
		if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
		{
			FVector KnockbackDir = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Damageable->TakeDamage(PlayerStats.BaseAttack * AttackMultiplier, PlayerStats.KnockbackForces[AttackType], KnockbackDir, PlayerStats.StunDurations[AttackType]);
		}
	}
	
	// DrawDebugBox(GetWorld(),
	// 	GetActorLocation() + GetActorForwardVector() * AttackRange,
	// 	FVector(AttackRange, AttackRadius, Height), 
	// 	 Rotation,
	// 	FColor::Blue, 
	// 	false, 
	// 	0.5f,
	// 	0, 
	// 	2.f);
}

void APlayerCharacter::SpecialAttackAnimationNotify()
{
	if (!IsValid(SpecialAttackMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Special Attack Montage Not Valid"));
		return;
	}
	
	PlayAnimMontage(SpecialAttackMontage, PlayerStats.AttackSpeedMultipliers[EAttackType::Special]);
}

void APlayerCharacter::SpecialAttack(EAttackType AttackType)
{
	float AttackRange = PlayerStats.AttackRanges[AttackType] * PlayerStats.AttackRangeMultipliers[AttackType];
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
	
	for (FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();

		if (!Actor || Actor == this) continue;

		HitActors.AddUnique(Actor);
	}
	
	for (TObjectPtr HitActor : HitActors)
	{
		if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
		{
			FVector KnockbackDir = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Damageable->TakeDamage(PlayerStats.BaseAttack * AttackMultiplier, PlayerStats.KnockbackForces[AttackType], KnockbackDir, PlayerStats.StunDurations[AttackType]);
		}
	}	
	
	//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Green, false, 0.5f, 0, 2.f);
}

void APlayerCharacter::TakeDamage(float DamageAmount, float KnockbackForce, FVector KnockbackDirection,
                                  float StunDuration)
{
	Super::TakeDamage(DamageAmount, KnockbackForce, KnockbackDirection, StunDuration);
	
	OnHealthChanged.Broadcast(UpdateHealthBar());
}

void APlayerCharacter::Death()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Died, UI Game Over Screen Should Appear"));
}

float APlayerCharacter::UpdateHealthBar()
{
	return static_cast<float>(CurrentHealth) / PlayerStats.MaxHealth;
}





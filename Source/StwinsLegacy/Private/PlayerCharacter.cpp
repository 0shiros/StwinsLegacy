

#include "PlayerCharacter.h"

#include "CollisionDebugDrawingPublic.h"
#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "PCGame.h"
#include "Camera/CameraComponent.h"
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
	GetCharacterMovement()->MaxWalkSpeed = PlayerStats.BaseSpeed;
}

void APlayerCharacter::BasicAttack()
{
	
}

void APlayerCharacter::HeavyAttack()
{
	
}

void APlayerCharacter::SpecialAttack()
{
	
}

void APlayerCharacter::TakeDamage(float DamageAmount)
{	
	PlayerStats.BaseHealth -= DamageAmount;
	
	if (PlayerStats.BaseHealth <= 0)
	{
		PlayerStats.BaseHealth = 0;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Died"));
	}
}




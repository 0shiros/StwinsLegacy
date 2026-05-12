

#include "PlayerCharacter.h"

#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "PCGame.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	LookAtMouse();
	UpdateRotation(DeltaTime);
}

void APlayerCharacter::InitialiseCharacterStats()
{
	CharacterStats = GameInstance->GetCurrentSaveGame()->PlayerStats;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStats.BaseSpeed;
}

void APlayerCharacter::Movement(FVector2D Value)
{	
	if (Value.IsNearlyZero()) return;

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	MoveInput = (Forward * Value.Y + Right * Value.X).GetSafeNormal();

	AddMovementInput(MoveInput);
}

void APlayerCharacter::LookAtMouse()
{
	if (!PlayerController) return;

	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);

	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

	FVector PlaneOrigin = GetActorLocation();
	FVector PlaneNormal = FVector::UpVector;

	FVector AimPoint = FMath::LinePlaneIntersection(
		WorldLocation,
		WorldLocation + WorldDirection * 10000.f,
		PlaneOrigin,
		PlaneNormal
	);

	AimDirection = (AimPoint - GetActorLocation()).GetSafeNormal();
}

void APlayerCharacter::UpdateRotation(float DeltaTime)
{
	if (AimDirection.IsNearlyZero()) return;

	FRotator TargetRot = AimDirection.Rotation();

	FRotator CurrentRot = GetActorRotation();

	FRotator NewRot = FMath::RInterpTo(
		CurrentRot,
		TargetRot,
		DeltaTime,
		15.f // vitesse de rotation (feel Hades)
	);

	SetActorRotation(NewRot);
}


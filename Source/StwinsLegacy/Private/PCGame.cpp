// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGame.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"

void APCGame::BeginPlay()
{
	Super::BeginPlay();
	
	if (!CharacterIMC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character Input Mapping Context Not Set"));
		return;
	}
	
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	if (TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
	{		
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(CharacterIMC, 0);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Local Player Not Valid"));
	}
}

void APCGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	LookAtMouse();
	UpdateRotation(DeltaTime);
}

void APCGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APCGame::Move);
		}
		
		if (DashAction)
		{
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &APCGame::Dash);
		}
		
		if (BasicAttackAction)
		{
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &APCGame::BasicAttack);
		}
		
		if (HeavyAttackAction)
		{
			EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &APCGame::HeavyAttack);
		}
		
		if (SpecialAttackAction)
		{
			EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Triggered, this, &APCGame::SpecialAttack);
		}
		
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APCGame::Interact);
		}
		
		if (PauseMenuAction)
		{
			EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Triggered, this, &APCGame::PauseMenu);
		}	
	}
}

void APCGame::Move(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}	
	
	FVector2D Input = Value.Get<FVector2D>();
	
	FVector Forward = FVector::ForwardVector;
	FVector Right = FVector::RightVector;

	MoveInput = (Forward * Input.Y + Right * Input.X).GetSafeNormal();

	PlayerCharacter->AddMovementInput(MoveInput);
}

void APCGame::LookAtMouse()
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	float MouseX, MouseY;
	GetMousePosition(MouseX, MouseY);

	FVector WorldLocation, WorldDirection;
	DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

	FVector PlaneOrigin = PlayerCharacter->GetActorLocation();
	FVector PlaneNormal = FVector::UpVector;

	FVector AimPoint = FMath::LinePlaneIntersection(
		WorldLocation,
		WorldLocation + WorldDirection * 10000.f,
		PlaneOrigin,
		PlaneNormal
	);
	
	AimDirection = (AimPoint - PlayerCharacter->GetActorLocation()).GetSafeNormal();
}

void APCGame::UpdateRotation(float DeltaTime)
{
	if (!PlayerCharacter || AimDirection.IsNearlyZero()) return;

	FRotator TargetRot = AimDirection.Rotation();

	FRotator CurrentRot = PlayerCharacter->GetActorRotation();

	FRotator NewRot = FMath::RInterpTo(
		CurrentRot,
		TargetRot,
		DeltaTime,
		25.f
	);

	PlayerCharacter->SetActorRotation(NewRot);
}

void APCGame::Dash(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	if (!PlayerCharacter->CanDash())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Dash On Cooldown"));
		return;
	}
	
	PlayerCharacter->LaunchCharacter(MoveInput * PlayerCharacter->PlayerStats.BaseDashDistance, true, true);
}

void APCGame::BasicAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	PlayerCharacter->Attack(EAttackType::Basic);
}

void APCGame::HeavyAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	PlayerCharacter->Attack(EAttackType::Heavy);
}

void APCGame::SpecialAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	PlayerCharacter->Attack(EAttackType::Special);
}

void APCGame::Interact(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Interact Action Triggered"));
}

void APCGame::PauseMenu(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Pause Menu Action Triggered"));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGame.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"

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
	InputMode.SetConsumeCaptureMouseDown(false);
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
	
	SetShowMouseCursor(true);
	
	MoveInput = PlayerCharacter->GetActorForwardVector();
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
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &APCGame::Dash);
		}
		
		if (BasicAttackAction)
		{
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &APCGame::BasicAttack);
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Completed, this, &APCGame::BasicAttackRelease);
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &APCGame::OrientCharacterToAttack);
		}
		
		if (HeavyAttackAction)
		{
			EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &APCGame::HeavyAttack);
		}
		
		if (SpecialAttackAction)
		{
			EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Started, this, &APCGame::SpecialAttack);
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
	
	if (!PlayerCharacter->bCanMove)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Cannot Move"));
		return;
	}
	
	FVector2D Input = Value.Get<FVector2D>();
	if (Input.IsNearlyZero()) return;

	FRotator CameraYaw(0.f, PlayerCharacter->PlayerCamera->GetComponentRotation().Yaw, 0.f);
	FMatrix CameraMatrix = FRotationMatrix(CameraYaw);

	FVector Forward = CameraMatrix.GetUnitAxis(EAxis::X);
	FVector Right   = CameraMatrix.GetUnitAxis(EAxis::Y);
	
	MoveInput = Forward * Input.Y + Right * Input.X;
	MoveInput.Normalize();
	
	PlayerCharacter->AddMovementInput(MoveInput);
}

void APCGame::Dash(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	if (!PlayerCharacter->CanDash() || !PlayerCharacter->bCanMove)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Dash On Cooldown"));
		return;
	}
	
	PlayerCharacter->LaunchCharacter( MoveInput * PlayerCharacter->PlayerStats.BaseDashDistance, true, true);
}

void APCGame::OrientCharacterToAttack()
{	
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
	
	PlayerCharacter->SetActorRotation(FRotator(0.f, AimDirection.Rotation().Yaw, 0.f));
}

void APCGame::BasicAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}	
	
	if (!PlayerCharacter->CanAttack(EAttackType::Basic))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Basic Attack On Cooldown"));
		return;
	}
	
	PlayerCharacter->bIsHoldingAttack = true;
	PlayerCharacter->BasicAttackAnimationNotify();
}

void APCGame::BasicAttackRelease(const FInputActionValue& Value)
{	
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	PlayerCharacter->bIsHoldingAttack = false;
}

void APCGame::HeavyAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	if (!PlayerCharacter->CanAttack(EAttackType::Heavy))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Heavy Attack On Cooldown"));
		return;
	}
	
	OrientCharacterToAttack();
	PlayerCharacter->HeavyAttackAnimationNotify();
}

void APCGame::SpecialAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	if (!PlayerCharacter->CanAttack(EAttackType::Special))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Special Attack On Cooldown"));
		return;
	}
	
	OrientCharacterToAttack();
	PlayerCharacter->SpecialAttackAnimationNotify();
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

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
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Move Action Triggered"));
}

void APCGame::Dash(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Dash Action Triggered"));
}

void APCGame::BasicAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Basic Attack Action Triggered"));
}

void APCGame::HeavyAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Heavy Attack Action Triggered"));
}

void APCGame::SpecialAttack(const FInputActionValue& Value)
{
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Character Not Valid"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Special Attack Action Triggered"));
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

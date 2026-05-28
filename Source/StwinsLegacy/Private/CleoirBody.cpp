// Fill out your copyright notice in the Description page of Project Settings.


#include "CleoirBody.h"

#include "PlayerCharacter.h"


// Sets default values for this component's properties
UCleoirBody::UCleoirBody()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCleoirBody::ApplyEnhancement()
{	
	GetWorld()->GetTimerManager().SetTimer(
		EnhancementTimerHandle,
		this,
		&UCleoirBody::PlayerProtection,
		reloadEnhancementDuration,
		true
	);
}

void UCleoirBody::PlayerProtection()
{	
	if (!bHasPlayerBeenTouched)
	{
		PlayerCharacter->Tags.Add("Status.Invincible");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Player is now invincible!"));
	}
	
	bHasPlayerBeenTouched = false;
}

void UCleoirBody::RemoveEnhancement()
{	
	EnhancementTimerHandle.Invalidate();
}


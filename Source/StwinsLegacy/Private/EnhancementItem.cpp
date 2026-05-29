// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancementItem.h"

#include "PlayerCharacter.h"


// Sets default values for this component's properties
UEnhancementItem::UEnhancementItem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnhancementItem::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EnhancementItem: Owner is not a PlayerCharacter!"));
		return;
	}
	
	ApplyEnhancement();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enhancement applied!"));
}

void UEnhancementItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveEnhancement();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Enhancement removed!"));
	
	Super::EndPlay(EndPlayReason);
}



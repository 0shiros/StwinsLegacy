// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPoint.h"

#include "GMGame.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATeleportPoint::ATeleportPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeleportPoint::OnPlayerOverlap);
}

void ATeleportPoint::SetVisible(bool bIsVisible)
{
	SetActorHiddenInGame(!bIsVisible);
	CollisionComponent->SetCollisionProfileName(bIsVisible ? TEXT("Portal") : TEXT("NoCollision"));
}

void ATeleportPoint::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGMGame* GameMode = Cast<AGMGame>(GetWorld()->GetAuthGameMode()))
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
		{
			GameMode->HandlePlayerTeleportation(this);
			SetVisible(false);
		}
	}
}

void ATeleportPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ATeleportPoint::OnPlayerOverlap);
	
	Super::EndPlay(EndPlayReason);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"

#include "EnemyCharacter.h"
#include "TeleportPoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ARoomManager::ARoomManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RoomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomMesh"));
	RootComponent = RoomMesh;
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(RootComponent);
	
	DeathArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathArea"));
	DeathArea->SetupAttachment(RootComponent);
	DeathArea->OnComponentBeginOverlap.AddDynamic(this, &ARoomManager::OnDeathAreaOverlap);
}

void ARoomManager::SetTeleportPointsVisibility(TMap<EPosition, bool> VisibilityMap)
{
	for (const TPair<EPosition, bool>& Pair : VisibilityMap)
	{
		if (TeleportPoints.Contains(Pair.Key) && TeleportPoints[Pair.Key])
		{
			TeleportPoints[Pair.Key]->SetVisible(Pair.Value);
		}
	}
}
void ARoomManager::InitializeRoom_Implementation(UMyGameInstance* GameInstance)
{
	// Default implementation does nothing, can be overridden in Blueprint for specific room types
}

void ARoomManager::LaunchSpawnWaves(int32 CurrentCycle)
{		
	SpawnedEnemies.Empty();
	SpawnCycleEnemyIndex = 0;
	CurrentWaveCycle = CurrentCycle;
	
	CheckToSpawnWave();
}

void ARoomManager::CheckToSpawnWave()
{
	if (SpawnedEnemies.Num() > 0)
	{
		return;
	}

	if (SpawnCycleEnemyIndex >= MaxSpawnCycles)
	{
		switch (RoomType)
		{	
			case ERoomManagerType::Combat:
				SetTeleportPointsVisibility(
					{{EPosition::North, true}, 
						{EPosition::East, true}, 
						{EPosition::South, true}, 
						{EPosition::West, true}});
				break;
			case ERoomManagerType::Transition:
				SetTeleportPointsVisibility({
					{EPosition::North, true}, 
					{EPosition::East, false}, 
					{EPosition::South, false}, 
					{EPosition::West, false}});
				break;
			case ERoomManagerType::Store:
				SetTeleportPointsVisibility({
					{EPosition::North, false}, 
					{EPosition::East, false}, 
					{EPosition::South, false}, 
					{EPosition::West, false}});
				break;
			default:
				break;
		}
		return;
	}

	SpawnEnemiesForCycle();
}

void ARoomManager::SpawnEnemiesForCycle()
{	
	for (TPair<TSubclassOf<AEnemyCharacter>, int>& EnemyPair : EnemyTypes)
	{
		for (int32 i = 0; i < (EnemyPair.Value + CurrentWaveCycle); i++)
		{
			if (EnemyPair.Key)
			{
				FTransform SpawnTransform = GetRandomSpawnPoint();
				if (AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(EnemyPair.Key, SpawnTransform))
				{
					SpawnedEnemy->FinishSpawning(SpawnTransform);
					SpawnedEnemy->OnDestroyed.AddDynamic(this, &ARoomManager::OnSpawnedDestroyed);
					SpawnedEnemies.Add(SpawnedEnemy);
				}
			}
		}
	}

	SpawnCycleEnemyIndex++;
}

void ARoomManager::OnSpawnedDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor) return;

	if (AEnemyCharacter* DeadEnemy = Cast<AEnemyCharacter>(DestroyedActor))
	{
		SpawnedEnemies.Remove(DeadEnemy);
		
		if (SpawnedEnemies.Num() == 0)
		{
			CheckToSpawnWave();
		}
	}
}

FTransform ARoomManager::GetRandomSpawnPoint() const
{
	if (!SpawnArea)
	{
		return FTransform();
	}

	FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	FVector Origin = SpawnArea->GetComponentLocation();

	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent * 0.9f); // Slightly reduce the spawn area to avoid spawning too close to edges
	return FTransform(RandomPoint);
}

void ARoomManager::OnDeathAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		Character->Destroy();
	}
}

void ARoomManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeathArea->OnComponentBeginOverlap.RemoveDynamic(this, &ARoomManager::OnDeathAreaOverlap);
	
	Super::EndPlay(EndPlayReason);	
}


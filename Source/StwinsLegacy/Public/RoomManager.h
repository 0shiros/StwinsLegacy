// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

class UBoxComponent;
class AEnemyCharacter;
enum class EEnemyType : uint8;
enum class EPosition : uint8;

UENUM()
enum class ERoomManagerType : uint8
{
	None UMETA(DisplayName = "None"),
	Tutorial UMETA(DisplayName = "Tutorial"),
	Combat UMETA(DisplayName = "Combat"),
	Transition UMETA(DisplayName = "Transition"),
	Store UMETA(DisplayName = "Store")
};

UCLASS()
class STWINSLEGACY_API ARoomManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoomManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RoomMesh;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnArea")
	UBoxComponent* SpawnArea;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeathArea")
	UBoxComponent* DeathArea;
	
	//Teleport points for each direction	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portals")
	TMap<EPosition, class ATeleportPoint*> TeleportPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portals")
	ERoomManagerType RoomType = ERoomManagerType::None;
			
	void SetTeleportPointsVisibility(TMap<EPosition, bool> VisibilityMap);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Room")
	void InitializeRoom(class UMyGameInstance* GameInstance);

	//Spawn Enemies	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	TMap<TSubclassOf<AEnemyCharacter>, int> EnemyTypes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemies")
	TArray<TObjectPtr<AEnemyCharacter>> SpawnedEnemies;
	
	UPROPERTY()
	int SpawnCycleEnemyIndex = 0;
	
	UPROPERTY()
	int MaxSpawnCycles = 3;
	
	UPROPERTY()
	int CurrentWaveCycle = 0;	
	
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void LaunchSpawnWaves(int32 CurrentCycle);
	
	void CheckToSpawnWave();
	
	void SpawnEnemiesForCycle();
	
	UFUNCTION()
	void OnSpawnedDestroyed(AActor* DestroyedActor);
	
	FTransform GetRandomSpawnPoint() const;
	
	UFUNCTION()
	void OnDeathAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "TeleportPoint.h"
#include "GameFramework/GameModeBase.h"
#include "GMGame.generated.h"

class ARoomManager;
class UMyGameInstance;

UCLASS(Abstract)
class STWINSLEGACY_API AGMGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstanceReference")
	TObjectPtr<UMyGameInstance> MyGameInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TObjectPtr<APlayerCharacter> PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomRandoms")
	TArray<TSoftObjectPtr<UWorld>> FightRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	TSoftObjectPtr<UWorld> TransitionRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	TSoftObjectPtr<UWorld> StoreRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	TSoftObjectPtr<UWorld> TutorialRoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RoomInformation")
	TSoftObjectPtr<UWorld> CurrentRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RoomInformation")
	TObjectPtr<ARoomManager> RoomManager = nullptr;

	
	UPROPERTY()
	int32 CurrentRoomIndex = 0;

	UPROPERTY()
	int32 NumberOfCycles = 10;

	UPROPERTY()
	int32 CurrentCycle = 0;

	UPROPERTY()
	int32 NumberOfFightRoomsPerCycle = 3;
	
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitializeRoomSequence();

	virtual void RestartStoreRun();
	virtual void RestartPlayer(AController* NewPlayer) override;
	void HandlePlayerTeleportation(const ATeleportPoint* TeleportPoint);

private:
	UPROPERTY()
	bool bIsLoadingRoom = false;

	UPROPERTY()
	TSoftObjectPtr<UWorld> PendingRoom;

	UPROPERTY()
	TSoftObjectPtr<UWorld> RoomToUnload;

	UPROPERTY()
	EPosition PendingTeleportPosition = EPosition::North;

	void LoadTutorialRoom();
	void LoadFightRoom(const EPosition* OppositeTeleportPoint);
	void LoadTransitionRoom(const EPosition* OppositeTeleportPoint);
	void LoadStoreRoom(const EPosition* OppositeTeleportPoint);

	void RequestRoomLoad(const TSoftObjectPtr<UWorld>& RoomToLoad, EPosition OppositeTeleportPoint);
	void ApplyRoomLoaded();
	FLatentActionInfo MakeLatentInfo(const FName& CallbackName, int32 UuidOffset = 0) const;

	ARoomManager* FindRoomManagerInStreamingLevel(const TSoftObjectPtr<UWorld>& RoomSoftPtr) const;
	void SetRoomData(ARoomManager* TargetRoomManager) const;
	bool TeleportPlayerToEntry(ARoomManager* TargetRoomManager, EPosition EntryPosition) const;
	void AdvanceCycleCountersAfterLoad(const TSoftObjectPtr<UWorld>& LoadedRoom);

	UFUNCTION()
	void OnTutorialRoomLoaded();

	UFUNCTION()
	void OnRoomLoaded();
};
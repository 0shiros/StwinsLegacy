#include "GMGame.h"

#include "ItemEnhanceData.h"
#include "MyGameInstance.h"
#include "RoomManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

void AGMGame::InitializeRoomSequence()
{
	if (!MyGameInstance)
	{
		MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	}
	if (!MyGameInstance)
	{
		return;
	}

	CurrentCycle = 0;
	CurrentRoomIndex = 0;
	bIsLoadingRoom = false;
	PendingRoom.Reset();
	RoomToUnload.Reset();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	RoomManager = nullptr;
	CurrentRoom.Reset();

	RestartStoreRun();
	LoadTutorialRoom();
}

void AGMGame::RestartStoreRun()
{
	for (TPair<EEnhancementItemType, UItemEnhanceData*>& ItemDataPair : MyGameInstance->EnhancementItemDataMap)
	{
		if (ItemDataPair.Value)
		{
			ItemDataPair.Value->bIsPurchased = false;
		}
	}
}

void AGMGame::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer && !NewPlayer->GetPawn())
	{
		Super::RestartPlayer(NewPlayer);
	}
}

void AGMGame::HandlePlayerTeleportation(const ATeleportPoint* TeleportPoint)
{
	if (!TeleportPoint || bIsLoadingRoom)
	{
		return;
	}

	const EPosition OppositeTeleportPoint = GetOppositeDirection(TeleportPoint->TeleportPosition);

	if (CurrentRoomIndex < NumberOfFightRoomsPerCycle)
	{
		LoadFightRoom(&OppositeTeleportPoint);
	}
	else if (CurrentRoomIndex == NumberOfFightRoomsPerCycle)
	{
		LoadTransitionRoom(&OppositeTeleportPoint);
	}
	else if (CurrentRoomIndex == NumberOfFightRoomsPerCycle + 1)
	{
		LoadStoreRoom(&OppositeTeleportPoint);
	}
}

void AGMGame::LoadTutorialRoom()
{
	if (TutorialRoom.IsNull() || bIsLoadingRoom)
	{
		return;
	}

	bIsLoadingRoom = true;
	PendingRoom = TutorialRoom;
	RoomToUnload = CurrentRoom;
	PendingTeleportPosition = EPosition::South; 

	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(
		this,
		PendingRoom,
		true,
		false, 
		MakeLatentInfo(TEXT("OnTutorialRoomLoaded"), 1001)
	);
}

void AGMGame::OnTutorialRoomLoaded()
{
	ApplyRoomLoaded();
}

void AGMGame::LoadFightRoom(const EPosition* OppositeTeleportPoint)
{
	if (!OppositeTeleportPoint || FightRooms.Num() == 0)
	{
		return;
	}

	const int32 NextRandomRoomIndex = FMath::RandRange(0, FightRooms.Num() - 1);
	RequestRoomLoad(FightRooms[NextRandomRoomIndex], *OppositeTeleportPoint);
}

void AGMGame::LoadTransitionRoom(const EPosition* OppositeTeleportPoint)
{
	if (!OppositeTeleportPoint)
	{
		return;
	}
	RequestRoomLoad(TransitionRoom, *OppositeTeleportPoint);
}

void AGMGame::LoadStoreRoom(const EPosition* OppositeTeleportPoint)
{
	if (!OppositeTeleportPoint)
	{
		return;
	}
	RequestRoomLoad(StoreRoom, *OppositeTeleportPoint);
}

void AGMGame::RequestRoomLoad(const TSoftObjectPtr<UWorld>& RoomToLoad, EPosition OppositeTeleportPoint)
{
	if (bIsLoadingRoom || RoomToLoad.IsNull())
	{
		return;
	}

	bIsLoadingRoom = true;
	PendingRoom = RoomToLoad;
	RoomToUnload = CurrentRoom;
	PendingTeleportPosition = OppositeTeleportPoint;

	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(
		this,
		PendingRoom,
		true,
		false, 
		MakeLatentInfo(TEXT("OnRoomLoaded"), 2001)
	);
}

void AGMGame::OnRoomLoaded()
{
	ApplyRoomLoaded();
}

void AGMGame::ApplyRoomLoaded()
{
	ARoomManager* NewRoomManager = FindRoomManagerInStreamingLevel(PendingRoom);
	if (!NewRoomManager)
	{
		bIsLoadingRoom = false;
		return;
	}

	SetRoomData(NewRoomManager);

	if (PlayerCharacter)
	{
		TeleportPlayerToEntry(NewRoomManager, PendingTeleportPosition);
	}

	if (RoomToUnload.IsValid() && RoomToUnload != PendingRoom)
	{
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(
			this,
			RoomToUnload,
			FLatentActionInfo(),
			false
		);
	}

	CurrentRoom = PendingRoom;
	RoomManager = NewRoomManager;

	AdvanceCycleCountersAfterLoad(CurrentRoom);

	bIsLoadingRoom = false;
}

FLatentActionInfo AGMGame::MakeLatentInfo(const FName& CallbackName, int32 UuidOffset) const
{
	FLatentActionInfo Info;
	Info.CallbackTarget = const_cast<AGMGame*>(this);
	Info.ExecutionFunction = CallbackName;
	Info.Linkage = 0;
	Info.UUID = GetUniqueID() + UuidOffset;
	return Info;
}

ARoomManager* AGMGame::FindRoomManagerInStreamingLevel(const TSoftObjectPtr<UWorld>& RoomSoftPtr) const
{
	if (RoomSoftPtr.IsNull())
	{
		return nullptr;
	}

	const FName PackageName(*FPackageName::ObjectPathToPackageName(RoomSoftPtr.ToSoftObjectPath().ToString()));
	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), PackageName);
	if (!StreamingLevel)
	{
		return nullptr;
	}

	ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();
	if (!LoadedLevel)
	{
		return nullptr;
	}

	for (AActor* Actor : LoadedLevel->Actors)
	{
		if (ARoomManager* Found = Cast<ARoomManager>(Actor))
		{
			return Found;
		}
	}

	return nullptr;
}

void AGMGame::SetRoomData(ARoomManager* TargetRoomManager) const
{
	if (!TargetRoomManager)
	{
		return;
	}
	
	switch (TargetRoomManager->RoomType)
	{
		case ERoomManagerType::Tutorial:
			TargetRoomManager->SetTeleportPointsVisibility({
				{EPosition::North, true},
				{EPosition::South, false},
				{EPosition::East, false},
				{EPosition::West, false}
			});
			break;
		case ERoomManagerType::Combat:
			TargetRoomManager->SetTeleportPointsVisibility({
				{EPosition::North, false},
				{EPosition::South, false},
				{EPosition::East, false},	
				{EPosition::West, false}
			});
			TargetRoomManager->LaunchSpawnWaves(CurrentCycle);
			break;
		case ERoomManagerType::Transition:
			TargetRoomManager->SetTeleportPointsVisibility({
				{EPosition::North, false},
				{EPosition::South, false},
				{EPosition::East, false},
				{EPosition::West, false}
			});
			TargetRoomManager->LaunchSpawnWaves(CurrentCycle);
			break;
		case ERoomManagerType::Store:
			TargetRoomManager->SetTeleportPointsVisibility({
				{EPosition::North, true},
				{EPosition::South, false},
				{EPosition::East, false},
				{EPosition::West, false}
			});
			TargetRoomManager->InitializeRoom(MyGameInstance);
			break;
		default:
			break;
	}
}

bool AGMGame::TeleportPlayerToEntry(ARoomManager* TargetRoomManager, EPosition EntryPosition) const
{
	if (!TargetRoomManager || !PlayerCharacter)
	{
		return false;
	}

	if (ATeleportPoint** FoundPoint = TargetRoomManager->TeleportPoints.Find(EntryPosition))
	{
		if (*FoundPoint)
		{
			PlayerCharacter->SetActorLocationAndRotation(
				(*FoundPoint)->GetActorLocation(),
				(*FoundPoint)->GetActorRotation()
			);
			return true;
		}
	}

	return false;
}

void AGMGame::AdvanceCycleCountersAfterLoad(const TSoftObjectPtr<UWorld>& LoadedRoom)
{
	CurrentRoomIndex++;

	if (LoadedRoom == StoreRoom)
	{
		CurrentRoomIndex = 0;
		CurrentCycle++;
	}
}
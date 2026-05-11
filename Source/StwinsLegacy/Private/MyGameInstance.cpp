// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();
	
	CurrentSaveGame = LoadOrCreateSaveGame();
}

UMySaveGame* UMyGameInstance::LoadOrCreateSaveGame()
{
	UMySaveGame* LoadedSaveGame = nullptr;
	
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		LoadedSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		LoadedSaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}
	
	return LoadedSaveGame;
}

void UMyGameInstance::SaveGame()
{
	if (!CurrentSaveGame)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No valid save game to save!"));
		return;
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GMGame.h"

#include "ItemEnhanceData.h"
#include "MyGameInstance.h"

void AGMGame::BeginPlay()
{
	Super::BeginPlay();
	
	if (MyGameInstance == nullptr)
	{
		MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	}
	
	if (!MyGameInstance)
	{
		return;
	}
	
	RestartStoreRun();
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

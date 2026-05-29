// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"

#include "TeleportPoint.h"


// Sets default values
ARoomManager::ARoomManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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


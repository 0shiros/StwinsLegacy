// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMGame.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class STWINSLEGACY_API AGMGame : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstanceReference")
	TObjectPtr<class UMyGameInstance> MyGameInstance;
	
	virtual void BeginPlay() override;
	
	virtual void RestartStoreRun();
	
	virtual void RestartPlayer(AController* NewPlayer) override;
};

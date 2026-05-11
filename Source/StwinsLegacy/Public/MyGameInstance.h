// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STWINSLEGACY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	FString SaveSlotName = TEXT("PlayerSaveSlot");	
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TObjectPtr<class UMySaveGame> CurrentSaveGame;
	
protected:
	virtual void Init() override;
	
	UMySaveGame* LoadOrCreateSaveGame();
	
public:
	
	void SaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	UMySaveGame* GetCurrentSaveGame() const { return CurrentSaveGame; }
};

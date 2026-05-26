// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, int, NewScore);

UCLASS()
class STWINSLEGACY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	FString SaveSlotName = TEXT("PlayerSaveSlot");	
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TObjectPtr<class UMySaveGame> CurrentSaveGame;
	
	// Score
	int CurrentScore = 0;
	
	// Delegates
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, Category = "Delegates")
	FOnScoreUpdated OnScoreUpdated;
	
protected:
	virtual void Init() override;
	
	UMySaveGame* LoadOrCreateSaveGame();
	
public:
	
	void SaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	UMySaveGame* GetCurrentSaveGame() const { return CurrentSaveGame; }

	void AddScore(int Score);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portals")
	TMap<EPosition, class ATeleportPoint*> TeleportPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portals")
	ERoomManagerType RoomType = ERoomManagerType::None;
			
	void SetTeleportPointsVisibility(TMap<EPosition, bool> VisibilityMap);
};

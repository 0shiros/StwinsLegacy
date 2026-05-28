// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemEnhanceData.generated.h"


class UEnhancementItem;

UENUM(Blueprintable)
enum class EEnhancementItemType : uint8
{
	MandiaBoots UMETA(DisplayName = "Mandia Boots"),
	PavyHat UMETA(DisplayName = "Pavy Hat"),
	JuineGloves UMETA(DisplayName = "Juine Gloves"),
	VenoirArmor UMETA(DisplayName = "Venoir Armor"),
	CleoirBody UMETA(DisplayName = "Cleoir Body"),
	GamburCane UMETA(DisplayName = "Gambur Cane")
};

UCLASS(Blueprintable)
class STWINSLEGACY_API UItemEnhanceData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public :	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhancement Item Data")
	EEnhancementItemType ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhancement Item Data")
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhancement Item Data")
	int Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhancement Item Data")
	TSubclassOf<UEnhancementItem> EnhancementClass;
};

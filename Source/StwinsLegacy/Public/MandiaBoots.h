// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancementItem.h"
#include "MandiaBoots.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STWINSLEGACY_API UMandiaBoots : public UEnhancementItem
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMandiaBoots();
	
	virtual void ApplyEnhancement() override;
	
	virtual void RemoveEnhancement() override;
};

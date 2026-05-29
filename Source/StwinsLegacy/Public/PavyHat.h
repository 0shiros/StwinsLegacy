// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancementItem.h"
#include "PavyHat.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STWINSLEGACY_API UPavyHat : public UEnhancementItem
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPavyHat();
	
	virtual void ApplyEnhancement() override;
	
	virtual void RemoveEnhancement() override;

};

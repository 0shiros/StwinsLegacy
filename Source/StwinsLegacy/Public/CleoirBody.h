// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancementItem.h"
#include "CleoirBody.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STWINSLEGACY_API UCleoirBody : public UEnhancementItem
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCleoirBody();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhancement")
	float reloadEnhancementDuration = 3.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enhancement")
	bool bHasPlayerBeenTouched = false;
	
	FTimerHandle EnhancementTimerHandle;
	
	

protected:
	
	virtual void ApplyEnhancement() override;
	
	void PlayerProtection();
	
	virtual void RemoveEnhancement() override;	
};

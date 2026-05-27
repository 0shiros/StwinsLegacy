// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancementItem.generated.h"

UCLASS(Abstract)
class STWINSLEGACY_API UEnhancementItem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	explicit UEnhancementItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerReference")
	TObjectPtr<class APlayerCharacter> PlayerCharacter;
	
	virtual void BeginPlay() override;
	
	virtual void ApplyEnhancement() PURE_VIRTUAL(UEnhancementItem::ApplyEnhancement, );
	
	virtual void RemoveEnhancement() PURE_VIRTUAL(UEnhancementItem::RemoveEnhancement, );
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportPoint.generated.h"

UENUM(BlueprintType)
enum class EPosition : uint8
{
	North UMETA(DisplayName = "North"),
	East UMETA(DisplayName = "East"),
	South UMETA(DisplayName = "South"),
	West UMETA(DisplayName = "West")
};

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Combat UMETA(DisplayName = "Combat"),
	Transition UMETA(DisplayName = "Transition"),
	Store UMETA(DisplayName = "Store")
};

static EPosition GetOppositeDirection(EPosition Dir)
{
	return static_cast<EPosition>((static_cast<int32>(Dir) + 2) % 4);
}

UCLASS()
class STWINSLEGACY_API ATeleportPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportPoint();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	EPosition TeleportPosition;	
	
	void SetVisible(bool bIsVisible);
	
	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

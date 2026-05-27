// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"

class UItemEnhanceData;
class APlayerCharacter;

UCLASS()
class STWINSLEGACY_API AStore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStore();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USphereComponent> InteractionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StoreMesh;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	TObjectPtr<UItemEnhanceData> ItemSellable;
	
protected:
	
	void SetItemPurchased(APlayerCharacter* Player);
	
	void ItemSell();
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

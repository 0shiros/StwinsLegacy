// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"

#include "EnhancementItem.h"
#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStore::AStore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	RootComponent = InteractionSphere;
	
	StoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoreMesh"));
	StoreMesh->SetupAttachment(RootComponent);
	
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AStore::OnComponentBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AStore::OnComponentEndOverlap);

}

void AStore::InitializeStore(UItemEnhanceData* ItemData)
{
	PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GameInstanceReference = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance	(GetWorld()));
	ItemSellable = ItemData;
}

void AStore::SetItemPurchased(APlayerCharacter* Player)
{
	UEnhancementItem* EnhancementItem = NewObject<UEnhancementItem>(Player, ItemSellable->EnhancementClass);
	EnhancementItem->RegisterComponent();
	Player->ActiveEnhancements.Add(ItemSellable->ItemName, EnhancementItem);
	Destroy();
}

void AStore::ItemSell()
{		
	if (PlayerReference && GameInstanceReference)
	{
		if (GameInstanceReference->SoulsQuantity >= ItemSellable->Cost)
		{
			GameInstanceReference->AddSouls(-ItemSellable->Cost);
			GameInstanceReference->EnhancementItemDataMap[ItemSellable->ItemName]->bIsPurchased = true;
			SetItemPurchased(PlayerReference);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not enough souls to purchase this item!"));
		}
	}
}

void AStore::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerReference && OtherActor == PlayerReference)
	{
		PlayerReference->OnStoreEntered.Broadcast(this, ItemSellable);
	}
}

void AStore::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerReference && OtherActor == PlayerReference)
	{
		PlayerReference->OnStoreExited.Broadcast();
	}
}

void AStore::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InteractionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AStore::OnComponentBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.RemoveDynamic(this, &AStore::OnComponentEndOverlap);
	
	Super::EndPlay(EndPlayReason);
}

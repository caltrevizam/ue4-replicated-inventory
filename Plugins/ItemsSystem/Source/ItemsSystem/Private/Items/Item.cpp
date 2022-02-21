#include "Items/Item.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Components/InventoryComponent.h"

UItem::UItem()
{
	DisplayName = FText::FromString(TEXT("Item"));
	MaxStackSize = 1;
	Quantity = 1;
	RepKey = 0;
}

void UItem::OnRep_Quantity()
{
	Modified.Broadcast();
}

#if WITH_EDITOR
void UItem::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UItem, Quantity) 
	|| ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UItem, MaxStackSize))
		Quantity = FMath::Clamp(Quantity, 1, MaxStackSize);
}
#endif

void UItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItem, Quantity);
}

bool UItem::IsSupportedForNetworking() const
{
	return true;
}

void UItem::MarkDirtyForReplication()
{
	RepKey++;
}

bool UItem::ShouldReplicate(UActorChannel* Channel) const
{
	return Channel->KeyNeedsToReplicate(GetUniqueID(), RepKey);
}

void UItem::SetOwningInventory(UInventoryComponent* Inventory)
{
	OwningInventory = Inventory;
}

void UItem::SetQuantity(const int32 NewQuantity)
{
	uint32 Clamped = FMath::Clamp(NewQuantity, 0, MaxStackSize);
	if (Clamped != Quantity)
	{
		Quantity = Clamped;
		MarkDirtyForReplication();
	}
}

void UItem::Use()
{
	if (OwningInventory && OwningInventory->GetOwner()->HasAuthority())
		Used.Broadcast();
}

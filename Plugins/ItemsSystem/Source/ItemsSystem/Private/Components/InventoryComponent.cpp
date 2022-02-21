#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Items/Item.h"

UInventoryComponent::UInventoryComponent()
{
	SetNetAddressable();
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Items);
}

bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (Channel->KeyNeedsToReplicate(505, RepKey)) //505 has no importance
	{
		for (auto& Item : Items)
		{
			if (Item->ShouldReplicate(Channel))
				bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UInventoryComponent::MarkDirtyForReplication()
{
	RepKey++;
}

void UInventoryComponent::OnRep_Items()
{
	ItemsChanged.Broadcast();
}

UItem* UInventoryComponent::FindItemByClass(TSubclassOf<UItem> ItemClass) const
{
	for (auto& Item : Items)
	{
		if (Item->GetClass()->IsChildOf(ItemClass))
			return Item;
	}
	return nullptr;
}

TArray<UItem*> UInventoryComponent::FindAllItemsByClass(TSubclassOf<UItem> ItemClass) const
{
	TArray<UItem*> Result;

	for (auto& Item : Items)
	{
		if (Item->GetClass()->IsChildOf(ItemClass))
			Result.Add(Item);
	}

	return Result;
}

bool UInventoryComponent::HasItem(UItem* Item, const int32 Quantity /*= 1*/) const
{
	check(Quantity > 0);
	int32 AvailableQuantity = 0;

	for (auto& Existing : FindAllItemsByClass(Item->GetClass()))
	{
		AvailableQuantity += Existing->GetQuantity();
		if (AvailableQuantity >= Quantity)
			return true;
	}

	return false;
}

FItemAddResult UInventoryComponent::TryAddItem(UItem* Item)
{
	return TryAddItem_Internal(Item->GetClass(), Item->GetQuantity());
}

FItemAddResult UInventoryComponent::TryAddItemFromClass(TSubclassOf<UItem> ItemClass, const int32 Quantity)
{
	return TryAddItem_Internal(ItemClass, Quantity);
}

void UInventoryComponent::Consume(UItem* Item, const int32 Quantity /*= 1*/)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item && Quantity > 0)
	{
		ensure(Item->GetQuantity() >= Quantity);
		const int32 Final = Item->GetQuantity() - Quantity;

		if (Final > 0)
		{
			Item->SetQuantity(Final);
			MarkDirtyForReplication();
		}
		else
		{
			Remove(Item);
			Item->BeginDestroy();
		}
	}
}

bool UInventoryComponent::Remove(UItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item)
	{
		Items.RemoveSingle(Item);
		MarkDirtyForReplication();
		return true;
	}
	return false;
}

void UInventoryComponent::Use(UItem* Item)
{
	if (!Item)
		return;

	if (!GetOwner()->HasAuthority())
	{
		ServerUse(Item);
		return;
	}

	if (Item->GetOwningInventory() == this)
		Item->Use();
}

FItemAddResult UInventoryComponent::TryAddItem_Internal(TSubclassOf<UItem> ItemClass, const int32 Quantity)
{
	if (GetOwner() && GetOwner()->HasAuthority() && ItemClass && Quantity > 0)
	{
		int32 Remaining = Quantity;

		// fills the existing items stack
		for (auto& ExistingItem : FindAllItemsByClass(ItemClass))
		{
			Remaining = AddQuantityToItem(ExistingItem, Remaining);
			if (Remaining == 0)
				break;
		}

		// create new stacks while necessary
		while (Remaining > 0)
		{
			UItem* NewItem = NewObject<UItem>(GetOwner(), ItemClass);
			NewItem->SetOwningInventory(this);
			Remaining -= NewItem->GetQuantity();
			Remaining = AddQuantityToItem(NewItem, Remaining);
			Items.Add(NewItem);
		}

		MarkDirtyForReplication();
		return FItemAddResult::AddedAll(Quantity);
	}
	check(false);
	return FItemAddResult();
}

int32 UInventoryComponent::AddQuantityToItem(UItem* Item, int32 Quantity)
{
	int32 QuantityToFillTheStack = Item->GetMaxStackSize() - Item->GetQuantity();
	int32 Adding = FMath::Min(Quantity, QuantityToFillTheStack);
	if (Adding > 0)
	{
		int32 Final = Item->GetQuantity() + Adding;
		Item->SetQuantity(Final);
		return Quantity - Adding;
	}
	return Quantity;
}

bool UInventoryComponent::ServerUse_Validate(UItem* Item)
{
	return Item && FindItemByClass(Item->GetClass());
}

void UInventoryComponent::ServerUse_Implementation(UItem* Item)
{
	Use(Item);
}

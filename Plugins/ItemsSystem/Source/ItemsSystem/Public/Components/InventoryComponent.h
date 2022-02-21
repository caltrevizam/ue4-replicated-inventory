#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItem;

USTRUCT(BlueprintType)
struct FItemAddResult 
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 GivenAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 AddedAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ErrorText;

	// Factory
	static FItemAddResult AddedAll(const int32 GivenAmount) 
	{
		FItemAddResult Result;
		Result.GivenAmount = GivenAmount;
		Result.AddedAmount = GivenAmount;
		return Result;
	}

	static FItemAddResult AddedSome(const int32 GivenAmount, const int32 AddedAmount)
	{
		FItemAddResult Result;
		Result.GivenAmount = GivenAmount;
		Result.AddedAmount = AddedAmount;
		return Result;
	}

	static FItemAddResult AddedNone(const int32 GivenAmount, const FText& ErrorText)
	{
		FItemAddResult Result;
		Result.GivenAmount = GivenAmount;
		Result.ErrorText = ErrorText;
		return Result;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemsChanged);

UCLASS(ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class ITEMSSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_Items, VisibleAnywhere, Category = "Inventory")
	TArray<UItem*> Items;

	UPROPERTY()
	int32 RepKey;

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemsChanged ItemsChanged;

	UInventoryComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void MarkDirtyForReplication();

private:
	UFUNCTION()
	void OnRep_Items();

public:
	UFUNCTION(BlueprintPure, Category = "Inventory")
	FORCEINLINE TArray<UItem*> GetItems() const { return TArray<UItem*>(Items); }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UItem* FindItemByClass(TSubclassOf<UItem> ItemClass) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<UItem*> FindAllItemsByClass(TSubclassOf<UItem> ItemClass) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasItem(UItem* Item, const int32 Quantity = 1) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemAddResult TryAddItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemAddResult TryAddItemFromClass(TSubclassOf<UItem> ItemClass, const int32 Quantity);

private:
	FItemAddResult TryAddItem_Internal(TSubclassOf<UItem> ItemClass, const int32 Quantity);

	int32 AddQuantityToItem(UItem* Item, int32 Quantity);

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Consume(UItem* Item, const int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool Remove(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Use(UItem* Item);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse(UItem* Item);
};

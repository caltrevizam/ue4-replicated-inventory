#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class UTexture2D;
class UInventoryComponent;
class UActorChannel;
class AActor;

UENUM(BlueprintType)
enum class EItemRarity : uint8 {
	IR_Common UMETA(DisplayName = "Common"),
	IR_Uncommon UMETA(DisplayName = "Uncommon"),
	IR_Rare UMETA(DisplayName = "Rare"),
	IR_VeryRare UMETA(DisplayName = "VeryRare"),
	IR_Legendary UMETA(DisplayName = "Legendary"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemStatusChanged);

UCLASS(Abstract, Blueprintable)
class ITEMSSYSTEM_API UItem : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UInventoryComponent* OwningInventory;

	UPROPERTY()
	int32 RepKey;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	FString Id;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true), Category = "Items")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	EItemRarity Rarity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items", meta = (ClampMin = 1))
	int32 MaxStackSize;

	UPROPERTY(ReplicatedUsing = OnRep_Quantity, EditAnywhere, Category = "Items", meta = (ClampMin = 1))
	int32 Quantity;

public:
	UPROPERTY(BlueprintAssignable, Category = "Items")
	FItemStatusChanged Modified;

	UPROPERTY(BlueprintAssignable, Category = "Items")
	FItemStatusChanged Used;

	UItem();

private:
	UFUNCTION()
	void OnRep_Quantity();

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override;

	void MarkDirtyForReplication();

public:
	bool ShouldReplicate(UActorChannel* Channel) const;

	UFUNCTION(BlueprintPure, Category = "Items")
	FORCEINLINE UInventoryComponent* GetOwningInventory() const { return OwningInventory; };

	virtual void SetOwningInventory(UInventoryComponent* Inventory);

	UFUNCTION(BlueprintPure, Category = "Items")
	FORCEINLINE int32 GetMaxStackSize() const { return MaxStackSize; };

	UFUNCTION(BlueprintPure, Category = "Items")
	FORCEINLINE int32 GetQuantity() const { return Quantity; };

	void SetQuantity(const int32 NewQuantity);

	void Use();
};

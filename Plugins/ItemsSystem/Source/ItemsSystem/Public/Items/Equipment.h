// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Equipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentStatusChanged);

UCLASS(Abstract)
class ITEMSSYSTEM_API UEquipment : public UItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FString SocketName;
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FEquipmentStatusChanged Equipped;

	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FEquipmentStatusChanged Unequipped;

	UEquipment();

private:
	UFUNCTION()
	void OnUsed();
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Equipment")
	bool CanEquip() const;

	void Equip();

	void Unequip();
};

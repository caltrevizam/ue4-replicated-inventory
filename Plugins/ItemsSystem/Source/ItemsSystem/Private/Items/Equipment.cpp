#include "Items/Equipment.h"

UEquipment::UEquipment()
{
	DisplayName = FText::FromString(TEXT("Equipment"));
	Used.AddDynamic(this, &UEquipment::OnUsed);
}

void UEquipment::OnUsed()
{

}

bool UEquipment::CanEquip_Implementation() const
{
	return true;
}

void UEquipment::Equip()
{

}

void UEquipment::Unequip()
{

}
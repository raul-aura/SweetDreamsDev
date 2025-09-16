#include "Inventory/InventoryItem.h"
#include "Inventory/SweetDreamsItem.h"
#include "Inventory/ItemParameters.h"

UItemParameters* UInventoryItem::GetExtraParameters(int32 Index) const
{
	if (ItemData->ExtraParameters.IsValidIndex(Index))
	{
		return ItemData->ExtraParameters[Index];
	}
	return nullptr;
}

TArray<UItemParameters*> UInventoryItem::GetAllExtraParameters() const
{
	return ItemData->ExtraParameters;
}

UItemParameters* UInventoryItem::GetExtraParametersOfClass(TSubclassOf<UItemParameters> Class) const
{
    TArray<UItemParameters*> ExtraParameters = GetAllExtraParameters();
    for (UItemParameters* Param : ExtraParameters)
    {
        if (Param && Param->IsA(Class))
        {
            return Param;
        }
    }
    return nullptr;
}

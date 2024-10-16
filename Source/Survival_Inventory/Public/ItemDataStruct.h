#pragma once

#include "CoreMinimal.h"
#include "ItemDataStruct.generated.h"

UENUM()
enum class FItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Consumable UMETA(DisplayName = "Consumable"),
	Item UMETA(DisplayName = "Item")
	
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};

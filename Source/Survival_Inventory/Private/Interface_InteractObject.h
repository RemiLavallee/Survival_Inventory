#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_InteractObject.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_InteractObject : public UInterface
{
	GENERATED_BODY()
};

class IInterface_InteractObject
{
	GENERATED_BODY()

public:
	virtual void Interact();
};

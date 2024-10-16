#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface_InteractObject.h"
#include "Cup.generated.h"

UCLASS()
class ACup : public AActor, public IInterface_InteractObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CupMesh;
};

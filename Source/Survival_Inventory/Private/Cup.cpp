// Fill out your copyright notice in the Description page of Project Settings.


#include "Cup.h"

// Sets default values
ACup::ACup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	CupMesh = CreateDefaultSubobject<UStaticMeshComponent>("CupMesh");
	CupMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ACup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACup::Interact()
{
	
}


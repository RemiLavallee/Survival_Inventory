// Copyright Epic Games, Inc. All Rights Reserved.

#include "Survival_InventoryCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "PlayerW.h"
#include "Interface_InteractObject.h"
#include "Chaos/ChaosPerfTest.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASurvival_InventoryCharacter

ASurvival_InventoryCharacter::ASurvival_InventoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Create sphere collision
	SphereDetection = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDetection"));
	SphereDetection->InitSphereRadius(50.f);
	SphereDetection->SetupAttachment(RootComponent);
	SphereDetection->ShapeColor.Red;

	SphereDetection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereDetection->SetCollisionObjectType(ECC_WorldDynamic);
	SphereDetection->SetCollisionResponseToAllChannels(ECR_Overlap);

	SphereDetection->OnComponentBeginOverlap.AddDynamic(this, &ASurvival_InventoryCharacter::OnSphereOverlapBegin);
	SphereDetection->OnComponentEndOverlap.AddDynamic(this, &ASurvival_InventoryCharacter::OnSphereOverlapEnd);
}

void ASurvival_InventoryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	if(PlayerWidgetClass)
	{
		PlayerWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass);
		if(PlayerWidgetInstance)
		{
			PlayerWidgetInstance->AddToViewport();

			PlayerWidget = Cast<UPlayerW>(PlayerWidgetInstance);
				
			if(PlayerWidget)
			{
				PlayerWidget->SetPrompt_F(false);
			}
		}
	}
}

void ASurvival_InventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsInspecting)
	{
		//PerformLineTrace();
	}

	// DrawDebug to see the sphereCollision

	/*
	FVector SphereLocation = SphereDetection->GetComponentLocation();
	float SphereRadius = SphereDetection->GetScaledSphereRadius();
	FColor SphereColor = FColor::Red;

	DrawDebugSphere(GetWorld(), SphereLocation, SphereRadius, 20, SphereColor, false, 0.1f);
	*/
}

void ASurvival_InventoryCharacter::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this && OtherComp)
	{
		if(OtherActor->GetClass()->ImplementsInterface(UInterface_InteractObject::StaticClass()))
		{
			CurrentItem = OtherActor;
			PlayerWidget->SetPrompt_F(true);
		}
	}
}

void ASurvival_InventoryCharacter::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor != this)
	{
		CurrentItem = nullptr;
		PlayerWidget->SetPrompt_F(false);
	}
}

// Linetrace switch by a sphere
/*
void ASurvival_InventoryCharacter::PerformLineTrace()
{
	FVector Start = GetActorLocation();
	//FVector ForwardVector = GetActorForwardVector();
	//FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	FHitResult HitResult;
//	FCollisionObjectQueryParams ObjectQueryParams(ObjectTypes);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	float SphereRadius = 160.f;
	

	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, Start, FQuat:: Identity,
		FCollisionObjectQueryParams(ObjectTypes), FCollisionShape:: MakeSphere(SphereRadius), CollisionParams);

	if(bHit && HitResult.GetActor() != nullptr)
	{
		AActor* HitActor = HitResult.GetActor();

		if(HitActor->GetClass()->ImplementsInterface(UInterface_InteractObject::StaticClass()))
		{
			CurrentItem = HitActor;
			PlayerWidget->SetPrompt_F(true);
		}
		else
		{
			CurrentItem = nullptr;
			PlayerWidget->SetPrompt_F(false);
		}
	}
	else
	{
		CurrentItem = nullptr;
		PlayerWidget->SetPrompt_F(false);
	}

	DrawDebugSphere(GetWorld(), Start, SphereRadius, 12, FColor::Red, false, 2.0f);
}
*/

//////////////////////////////////////////////////////////////////////////
// Input

void ASurvival_InventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvival_InventoryCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvival_InventoryCharacter::Look);

		//EnhancedInputComponent->BindAction(LineTraceAction, ETriggerEvent::Triggered, this, &ASurvival_InventoryCharacter::PerformLineTrace);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASurvival_InventoryCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASurvival_InventoryCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
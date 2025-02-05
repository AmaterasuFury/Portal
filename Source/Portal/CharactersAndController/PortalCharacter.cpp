// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Portal/amsuInteractable.h"
#include "Portal/Components/amsuIntreactionDetectComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);
DEFINE_LOG_CATEGORY(LogHealthCharacter);

//////////////////////////////////////////////////////////////////////////
// APortalCharacter

APortalCharacter::APortalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	InteractDetectComponent = CreateDefaultSubobject<UamsuInteractionDetectComponent>(TEXT("Interact Detection Component"));
}

void APortalCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////// Input

void APortalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APortalCharacter::OnCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APortalCharacter::OnUnCrouch);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APortalCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APortalCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APortalCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APortalCharacter::BroadcastOnGunPickUp()
{
	OnGunPickedUp.Broadcast();
}

void APortalCharacter::DamageCharacter(float DamageGiven, float InDeltaTime)
{
	Health -= DamageGiven * InDeltaTime;

	if (Health <= 0.f)
	{
		Die();
	}
}

void APortalCharacter::HealthRegenerate(float HealPerSecond, float InDeltaTime)
{
	if (MaxHealth > Health)
	{
		Health = FMath::Clamp(Health + (HealPerSecond * InDeltaTime), 0.f, MaxHealth);
	}
}

void APortalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	HealthRegenerate(HealthRegeneratePerSecond, DeltaSeconds);
	UE_LOG(LogHealthCharacter, Verbose, TEXT("The Character health is: %f"), Health);
}

void APortalCharacter::Die()
{
	//TODO Implement Death
	UE_LOG(LogHealthCharacter, Verbose, TEXT("DEAD!!!!"));
}


void APortalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APortalCharacter::Look(const FInputActionValue& Value)
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

void APortalCharacter::OnCrouch()
{
	Crouch();
}

void APortalCharacter::OnUnCrouch()
{
	UnCrouch();
}

void APortalCharacter::Interact()
{
	check(IsValid(InteractDetectComponent));
	
	if (const TScriptInterface<IamsuInteractable> InteractedActor = InteractDetectComponent->GetAimedInteractable())
	{
		IamsuInteractable::Execute_Interact(InteractedActor.GetObject(), this);
	}
}



// Copyright Epic Games, Inc. All Rights Reserved.


#include "UamsuPortalGun.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "PortalProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Portal/Portals/amsuPortal.h"

DEFINE_LOG_CATEGORY_STATIC(LogPortalGun, Log, All);

// Sets default values for this component's properties
UamsuPortalGun::UamsuPortalGun()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

// TODO spawn portals in the BeginPlay and hide them
PRAGMA_DISABLE_OPTIMIZATION
FHitResult UamsuPortalGun::GetAimedHitResult(float InCheckDistance, ECollisionChannel InCollisionChannel) const
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	// TODO It does not get a playerControler, find the way u get the player constoler
	APawn* PawnOwner = GetOwner<APawn>();
	if (!IsValid(PawnOwner))
	{
		FHitResult EmptyHitResult {};
		return EmptyHitResult;
	}
	
	const APlayerController* PlayerController = Cast<APlayerController>(PawnOwner->GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	const FVector TraceDestination = ViewLocation + ViewRotation.Vector() * InCheckDistance;
 
#if ENABLE_DRAW_DEBUG && 1
	DrawDebugLine(GetWorld(), ViewLocation, TraceDestination, FColor::Green, false, 2.1f, 0, 4.f);
#endif
	
	FHitResult HitResult;
	
	FCollisionQueryParams FCollisionQueryParams;
	FCollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceDestination, InCollisionChannel, FCollisionQueryParams );
	
	return HitResult;
}
PRAGMA_ENABLE_OPTIMIZATION
void UamsuPortalGun::BeginPlay()
{
	Super::BeginPlay();

	/** Spawn the portals on the 0.0.0 positions on the beginning of the game */
	FTransform const SpawnTransform(SpawnLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	PortalOne = GetWorld()->SpawnActor<AamsuPortal>(PortalOneClass, SpawnTransform, SpawnParameters);
	PortalTwo = GetWorld()->SpawnActor<AamsuPortal>(PortalTwoClass, SpawnTransform, SpawnParameters);

	if (IsValid(PortalOne) && IsValid(PortalTwo))
	{
		PortalOne->AnotherPortal = PortalTwo;
		PortalTwo->AnotherPortal = PortalOne;
	}
}

bool UamsuPortalGun::AttachWeapon(APortalCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UamsuPortalGun>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	AActor* OldOwner = GetOwner();
	
	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	Rename(nullptr, Character);
	
	if (IsValid(OldOwner))
	{
		OldOwner->Destroy();
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireLeftAction, ETriggerEvent::Triggered, this, &ThisClass::FireLeft);
			EnhancedInputComponent->BindAction(FireRightAction, ETriggerEvent::Triggered, this, &ThisClass::FireRight);
		}
	}

	return true;
}

void UamsuPortalGun::FireLeft()
{ //TODO when u finish this function u can get rid of the FireRight(), cuz the would do the same, just another pointer to a portal
	UE_LOG(LogPortalGun, Log, TEXT("Fire Left"));
	
	if (!IsValid(Character) || !IsValid(Character->GetController()))
	{
		return;
	}
	
		FHitResult AimedHit = GetAimedHitResult();

		// Todo Upgrade the spawn location
		const FVector PortalSpawnLocation = AimedHit.ImpactNormal;

		FTransform SpawnTransform(PortalSpawnLocation);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// TODO just change the location of the portal that was created on the begin play 
	PortalOne->SetActorRelativeLocation(PortalSpawnLocation);
	
	
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UamsuPortalGun::FireRight()
{
	if (!IsValid(Character) || !IsValid(Character->GetController()))
	{
		return;
	}
	UE_LOG(LogPortalGun, Log, TEXT("Fire Right"));
}

void UamsuPortalGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

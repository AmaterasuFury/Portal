// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "PortalCharacter.h"
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

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (!IsValid(Character)  || !IsValid(Character->GetController()))
	{
		return;
	}

	// Try and fire a projectile
//	if (ProjectileClass != nullptr)
//	{
//		UWorld* const World = GetWorld();
//		if (World != nullptr)
//		{
//			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
//			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
//			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
//			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
//	
//			//Set Spawn Collision Handling Override
//			FActorSpawnParameters ActorSpawnParams;
//			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
//	
//			// Spawn the projectile at the muzzle
//			World->SpawnActor<APortalProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
//		}
//	
//	}

	if (IsValid(PortalOne))
	{
		FHitResult AimedHit = GetAimedHitResult();

		// Todo Upgrade the spawn location
		const FVector SpawnLocation = AimedHit.ImpactPoint;

		FTransform SpawnTransform(SpawnLocation);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//GetWorld()->SpawnActor<AamsuPortal>(PortalOne, SpawnTransform, SpawnParameters);
	}
	
	
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

FHitResult UTP_WeaponComponent::GetAimedHitResult(float InCheckDistance, ECollisionChannel InCollisionChannel) const
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (IsValid(PlayerController))
	{
		GetOwner<APlayerController>()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	const FVector TraceDestination = ViewLocation + ViewRotation.Vector() * InCheckDistance;

#if ENABLE_DRAW_DEBUG && 1
	DrawDebugLine(GetWorld(), ViewLocation, TraceDestination, FColor::Green, false, 0.1f, 0, 4.f);
#endif
	
	FHitResult HitResult;
	
	FCollisionQueryParams FCollisionQueryParams;
	FCollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceDestination, InCollisionChannel, FCollisionQueryParams );
	
	return HitResult;
}

bool UTP_WeaponComponent::AttachWeapon(APortalCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

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
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
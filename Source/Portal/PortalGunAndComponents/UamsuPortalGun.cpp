// Copyright Epic Games, Inc. All Rights Reserved.


#include "UamsuPortalGun.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Portal/Portals/amsuPortal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/OverlapResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogPortalGun, Log, All);

// Sets default values for this component's properties
UamsuPortalGun::UamsuPortalGun()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

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

	// Make sure the PortalSurfaceMaterial is set at the PortalGun instance
	check(IsValid(PortalSurfaceMaterial));

	FVector Origin = PortalOne->GetActorLocation();
	FVector BoxExtent = FVector::ZeroVector;
	PortalOne->MeshComponentPortal->GetLocalBounds(Origin, BoxExtent);
	
	PortalsHalfWidth = BoxExtent.X;
	PortalsHalfHeight = BoxExtent.Z;
}

FHitResult UamsuPortalGun::GetAimedHitResult(float InCheckDistance, ECollisionChannel InCollisionChannel) const
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	APawn* PawnOwner = GetOwner<APawn>();
	if (!IsValid(PawnOwner))
	{
		FHitResult EmptyHitResult {};
		return EmptyHitResult;
	}
	
	const APlayerController* PlayerController = Cast<APlayerController>(PawnOwner->GetController());
	if (!IsValid(PlayerController))
	{
		FHitResult EmptyHitResult {};
		return EmptyHitResult;
	}

	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector TraceDestination = ViewLocation + ViewRotation.Vector() * InCheckDistance;
 
#if ENABLE_DRAW_DEBUG && 0
	DrawDebugLine(GetWorld(), ViewLocation, TraceDestination, FColor::Green, false, 0.1f, 0, 4.f);
#endif
	
	FHitResult HitResult;
	
	FCollisionQueryParams FCollisionQueryParams;
	FCollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceDestination, InCollisionChannel, FCollisionQueryParams);
	
	return HitResult;
}


// todo u can rename it to "CanSpawnPortalHereOrAdjust" or a better name

bool UamsuPortalGun::CanAdjustAndSpawnPortalHere(FHitResult & HitResult, AamsuPortal* Portal) const
{
	
	if (!IsValid(PortalSurfaceMaterial) || !HitResult.bBlockingHit)
	{
		return false;
	}

	const UPrimitiveComponent* HitComponent = HitResult.GetComponent();

	if (!IsValid(HitComponent))
	{
		return false;
	}

	const UMaterialInterface* const HitMaterial = HitComponent->GetMaterial(HitResult.ElementIndex);

	if(HitMaterial != PortalSurfaceMaterial)
	{
		return false;
	}
	
	const FVector HitPoint = HitResult.ImpactPoint;
	
	FVector RightVector = FVector::ZeroVector; 
	FVector ForwardVector = FVector::ZeroVector;
	
	HitResult.ImpactNormal.FindBestAxisVectors(ForwardVector,RightVector);

	TArray<FVector> PortalEdges;
	PortalEdges.Reserve(4);

	FVector BottomEdge = BottomEdge = HitPoint - (ForwardVector * (PortalsHalfHeight));
	FVector TopEdge = TopEdge = HitPoint + (ForwardVector * (PortalsHalfHeight));
	FVector LeftEdge = LeftEdge =  HitPoint - (RightVector * PortalsHalfWidth);
	FVector RightEdge = RightEdge = HitPoint +  (RightVector * PortalsHalfWidth);
	
	PortalEdges.Add(BottomEdge);
	PortalEdges.Add(TopEdge);
	PortalEdges.Add(LeftEdge);
	PortalEdges.Add(RightEdge);

	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(Character);
	QueryParams.AddIgnoredActor(Portal);
	
	TArray<FOverlapResult> OutOverlaps;
	
	for (const FVector& PortalEdge : PortalEdges)
	{
		OutOverlaps.Empty();
		GetWorld()->OverlapMultiByChannel(OutOverlaps, PortalEdge, FQuat::Identity, ECC_Visibility,
			FCollisionShape::MakeSphere(5.0f), QueryParams);
		
		
		// todo This 1 overlap check will work bad if there e.g. a wal that consists of 2 actors with the same material but diff actors, so probably just add boxoverlapp check
		// Also that is better practise to use physmaterial for this kind of check, consider changing to physmaterial
		if (OutOverlaps.Num() != 1)
		{
			return false;
		}
		
		const UPrimitiveComponent* OverlappedComponent = OutOverlaps[0].GetComponent();
		
		const UMaterialInterface* OverlappedMaterial = OverlappedComponent->GetMaterial(0);
		
		if(OverlappedMaterial != PortalSurfaceMaterial || HitResult.GetActor() != OutOverlaps[0].GetActor())
		{
			return false;
		}
	}
	
#if ENABLE_DRAW_DEBUG && 1
	DrawDebugSphere(GetWorld(), TopEdge, 10.f, 12, FColor::Purple, false, 10.f);
	DrawDebugSphere(GetWorld(), BottomEdge, 5.f, 12, FColor::Purple, false, 10.f);
	DrawDebugSphere(GetWorld(), RightEdge, 10.f, 12, FColor::Purple, false, 10.f);
	DrawDebugSphere(GetWorld(), LeftEdge, 10.f, 12, FColor::Purple, false, 10.f);
#endif
	
	return true;
}


void UamsuPortalGun::ShootPortal(AamsuPortal* Portal) const
{
	if (!IsValid(Character) || !IsValid(Character->GetController()))
	{
		return;
	}

	FHitResult AimedHit = GetAimedHitResult();
	

	if (!CanAdjustAndSpawnPortalHere(AimedHit, Portal))
	{
		return;	
	}

	
	const FVector PortalSpawnLocation = AimedHit.Location;
	const FRotator PortalSpawnRotation = AimedHit.ImpactNormal.Rotation();
	
	const FTransform SpawnTransform(PortalSpawnRotation,PortalSpawnLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Portal->SetActorTransform(SpawnTransform);
	
	
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
	
	Portal->OnPortalPlaced(true);
}

void UamsuPortalGun::FireLeft() 
{
	if (IsValid(PortalOne))
	{
		ShootPortal(PortalOne);
	}
}

void UamsuPortalGun::FireRight()
{
	if (IsValid(PortalTwo))
	{
		ShootPortal(PortalTwo);
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
	
	Character->PortalGun = this;
	
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

	Character->BroadcastOnGunPickUp();
	
	return true;
}

void UamsuPortalGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		Super::EndPlay(EndPlayReason);
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
	Super::EndPlay(EndPlayReason);
}
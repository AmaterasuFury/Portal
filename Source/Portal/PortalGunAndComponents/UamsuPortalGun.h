// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Portal/Portals/amsuPortal.h"
#include "UamsuPortalGun.generated.h"

class AamsuPortal;
class APortalCharacter;
class UInputAction;
class UPhysicalMaterial;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTAL_API UamsuPortalGun : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APortalProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireRightAction;

	/** Sets default values for this component's properties */
	UamsuPortalGun();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(APortalCharacter* TargetCharacter);

	void FireLeft();

	void FireRight();

	UPROPERTY(Transient)
	TObjectPtr<AamsuPortal> PortalOne;

	UPROPERTY(Transient)
	TObjectPtr<AamsuPortal> PortalTwo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AamsuPortal> PortalOneClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AamsuPortal> PortalTwoClass;
	
	/** Gets the HitResultOf the aim */
	UFUNCTION()
	FHitResult GetAimedHitResult(const AamsuPortal* IgnoredPortal, float InCheckDistance = 20000.f, ECollisionChannel InCollisionChannel = ECC_Visibility) const;

	/** The location of the default 'hidden' from the player locations of the portals */
	FVector SpawnLocation = FVector::ZeroVector;

	virtual void BeginPlay() override;
	
	void ShootPortal(AamsuPortal* Portal) const;

	
	/** Checks if anything is in front of it */
	bool IsEnoghFrontSpace(const FHitResult& HitResult) const;
	
protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:

	bool CanAdjustAndSpawnPortalHere(FHitResult & HitResult, AamsuPortal* Portal) const;
	
	/** The Character holding this weapon*/
	UPROPERTY(Transient)
	TObjectPtr<APortalCharacter> Character;

	/** The only surface that the portals can be spawned on */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> PortalSurfaceMaterial;

	float PortalsHalfWidth = 0.0f;
	float PortalsHalfHeight = 0.0f;
};

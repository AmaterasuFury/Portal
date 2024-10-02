// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "UamsuPortalGun.generated.h"

class AamsuPortal;
class APortalCharacter;
class UInputAction;

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

	UPROPERTY()
	TObjectPtr<AamsuPortal> PortalOne;

	UPROPERTY()
	TObjectPtr<AamsuPortal> PortalTwo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AamsuPortal> PortalOneClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AamsuPortal> PortalTwoClass;
	
	/** Gets the HitResultOf the aim */
	UFUNCTION()
	FHitResult GetAimedHitResult(float InCheckDistance = 20000.f,  ECollisionChannel InCollisionChannel = ECC_Visibility) const;

	/** The location of the default 'hidden' from the player locations of the portals */
	UPROPERTY(EditAnywhere)
	FVector SpawnLocation = FVector::ZeroVector;

	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	APortalCharacter* Character;
};

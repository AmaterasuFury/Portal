// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PortalCharacter.generated.h"

class UamsuPortalGun;
class UamsuInteractionDetectComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class AamsuPortal;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogHealthCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FGunPickedUp)

UCLASS(config=Game)
class APortalCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;
	
	/** A component to detect if the aimed actor implements Interactable interface */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact Detection Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UamsuInteractionDetectComponent> InteractDetectComponent;
	
public:
	APortalCharacter();	

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for the crouch input */
	void OnCrouch();
	void OnUnCrouch();

	/** Called for interact input */
	void Interact();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Is set when the character picks up the portal gun */
	UPROPERTY(Transient)
	TObjectPtr<UamsuPortalGun> PortalGun;
	
	FGunPickedUp OnGunPickedUp;

	// Executes the 
	void BroadcastOnGunPickUp();
	
	void DamageCharacter(float DamageGiven, float InDeltaTime);

	void HealthRegenerate(float HealPerSecond, float InDeltaTime);

	void Tick(float DeltaSeconds) override;

	void HealthCHeck();
	
	void Die();
	
private:
	//** The health that the character is going to be spawned with */
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(Transient)
	float Health = 100;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthRegeneratePerSecond = 20;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "amsuCrosshair.generated.h"

class UImage;

/**
 * 
 */
UCLASS(Abstract)
class PORTAL_API UamsuCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	/** The crosshair that should be always displayed, if the player got the portal gun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> MainCrosshair;

	/** Do not set any texture directly here, set it in the ActiveCrosshairPortalOneTexture, DisabledCrosshairPortalOneTexture */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CrosshairPortalOne;

	/** Do not set any texture directly here, set it in the ActiveCrosshairPortalOneTexture, DisabledCrosshairPortalOneTexture */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CrosshairPortalTwo;

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void SubscribeOnPortalGunPickedUp();
	
	void BindCrosshairDelegates();

	void UpdateCrosshairOne(bool bIsActive);
	void UpdateCrosshairTwo(bool IsActive);

	/** The texture u have to set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Crosshair")
	TObjectPtr<UTexture2D> ActiveCrosshairPortalOneTexture;

	/** The texture u have to set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Crosshair")
	TObjectPtr<UTexture2D> DisabledCrosshairPortalOneTexture;

	/** The texture u have to set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Crosshair")
	TObjectPtr<UTexture2D> ActiveCrosshairPortalTwoTexture;

	/** The texture u have to set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Crosshair")
	TObjectPtr<UTexture2D> DisabledCrosshairPortalTwoTexture;
	
private:
	FDelegateHandle BindCrosshairDelegateHandle;
};

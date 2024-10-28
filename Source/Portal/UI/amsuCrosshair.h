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

	/** The crosshair that should be displayed if the PortalOne is placed and is visible in the world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CrosshairPortalOne;

	/** The crosshair that should be displayed if the PortalTwo is placed and is visible in the world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CrosshairPortalTwo;

};

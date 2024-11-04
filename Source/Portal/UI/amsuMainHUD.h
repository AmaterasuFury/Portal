// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "amsuMainHUD.generated.h"

class UamsuCrosshair;

/**
 * 
 */
UCLASS()
class PORTAL_API AamsuMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	AamsuMainHUD();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UamsuCrosshair> CrosshairClass;

	UPROPERTY()
	TObjectPtr<UamsuCrosshair> Crosshair;
};

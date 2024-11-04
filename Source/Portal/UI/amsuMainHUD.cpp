// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuMainHUD.h"

#include "Blueprint/UserWidget.h"
#include "amsuCrosshair.h"

AamsuMainHUD::AamsuMainHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AamsuMainHUD::BeginPlay()
{
	Super::BeginPlay();

	Crosshair = CreateWidget<UamsuCrosshair>(GetOwningPlayerController(), CrosshairClass);
	Crosshair->AddToViewport();
}

void AamsuMainHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

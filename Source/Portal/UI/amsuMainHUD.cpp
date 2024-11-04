// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuMainHUD.h"

#include "Blueprint/UserWidget.h"
#include "amsuCrosshair.h"
#include "Misc/DataValidation.h"

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

#if WITH_EDITOR
EDataValidationResult AamsuMainHUD::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (CrosshairClass == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("amsuMainHUD.cpp:35 amsuMainHUD.cpp(35) CrosshairClass is not set")));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

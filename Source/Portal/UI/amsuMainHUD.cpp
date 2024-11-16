// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuMainHUD.h"

#include "Blueprint/UserWidget.h"
#include "amsuCrosshair.h"
#include "Misc/DataValidation.h"

AamsuMainHUD::AamsuMainHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AamsuMainHUD::BeginPlay()
{
	Super::BeginPlay();

	Crosshair = CreateWidget<UamsuCrosshair>(GetOwningPlayerController(), CrosshairClass);
	if (ensure(IsValid(Crosshair)))
	{
		Crosshair->AddToViewport();
	}
}

#if WITH_EDITOR
EDataValidationResult AamsuMainHUD::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result =  Super::IsDataValid(Context);

	if (CrosshairClass == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("CrosshairClass is not set (amsuMainHud")));
		Result = EDataValidationResult::Invalid;
	}
	return Result;
}
#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuCrosshair.h"

#include "Components/Image.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "Portal/Portals/amsuPortal.h"

void UamsuCrosshair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CrosshairPortalOne->SetVisibility(ESlateVisibility::Hidden);
	CrosshairPortalTwo->SetVisibility(ESlateVisibility::Hidden);
	
}

void UamsuCrosshair::UpdatePortalsState()
{
	const APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->PortalOne->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairOne);
		PortalCharacter->PortalTwo->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairTwo);
	}
}

void UamsuCrosshair::UpdateCrosshairOne()
{
	
}

void UamsuCrosshair::UpdateCrosshairTwo()
{
	
}

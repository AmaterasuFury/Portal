// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuCrosshair.h"

#include "Components/Image.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "Portal/PortalGunAndComponents/UamsuPortalGun.h"
#include "Portal/Portals/amsuPortal.h"

void UamsuCrosshair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CrosshairPortalOne->SetVisibility(ESlateVisibility::Hidden);
	CrosshairPortalTwo->SetVisibility(ESlateVisibility::Hidden);

	OnPortalGunPickedUpBind();
}

void UamsuCrosshair::OnPortalGunPickedUpBind()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->OnGunPickedUp.BindUObject(this, &UamsuCrosshair::BindCrosshairDelegates);
	}
}


void UamsuCrosshair::BindCrosshairDelegates()
{
	const APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->PortalGun->PortalOne->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairOne);
		PortalCharacter->PortalGun->PortalTwo->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairTwo);
	}
}

void UamsuCrosshair::UpdateCrosshairOne(bool IsActive)
{
	if (IsActive)  // TODO think if u need to add a function so u wont double the code
	{
		CrosshairPortalOne->SerVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CrosshairPortalOne->SerVisibility(ESlateVisibility::Visible);
	}
}

void UamsuCrosshair::UpdateCrosshairTwo(bool IsActive)
{
	if (IsActive)
	{
		
	}
	else
	{
		
	}
}

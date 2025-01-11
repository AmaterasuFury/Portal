// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuCrosshair.h"

#include "Components/Image.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "Portal/PortalGunAndComponents/UamsuPortalGun.h"
#include "Portal/Portals/amsuPortal.h"

void UamsuCrosshair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MainCrosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
	CrosshairPortalOne->SetVisibility(ESlateVisibility::Hidden);
	CrosshairPortalTwo->SetVisibility(ESlateVisibility::Hidden);

	SubscribeOnPortalGunPickedUp();
}

void UamsuCrosshair::SubscribeOnPortalGunPickedUp()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->OnGunPickedUp.AddUObject(this, &UamsuCrosshair::BindCrosshairDelegates);
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
// TODO think about changing the image instead of having 4 widgets (like probably use two different textures (for enabled/disabled states))
void UamsuCrosshair::UpdateCrosshairOne(bool IsActive)
{
	if (IsActive)  // TODO think if u need to add a function so u wont double the code
	{
		CrosshairPortalOne->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		CrosshairPortalOne->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UamsuCrosshair::UpdateCrosshairTwo(bool IsActive)
{
	if (IsActive)  // TODO think if u need to add a function so u wont double the code
	{
		CrosshairPortalTwo->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		CrosshairPortalTwo->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

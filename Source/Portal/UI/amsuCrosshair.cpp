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

void UamsuCrosshair::NativeDestruct()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->OnGunPickedUp.Clear();
	}
	
	Super::NativeDestruct();
}

void UamsuCrosshair::SubscribeOnPortalGunPickedUp()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		BindCrosshairDelegateHandle = PortalCharacter->OnGunPickedUp.AddUObject(this, &UamsuCrosshair::BindCrosshairDelegates);
	}
}


void UamsuCrosshair::BindCrosshairDelegates()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->PortalGun->PortalOne->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairOne);
		PortalCharacter->PortalGun->PortalTwo->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairTwo);
		
		PortalCharacter->OnGunPickedUp.Remove(BindCrosshairDelegateHandle);
	}
}
// TODO think about changing the image instead of having 4 widgets (like probably use two different textures (for enabled/disabled states))
void UamsuCrosshair::UpdateCrosshairOne(bool bIsActive)
{
	CrosshairPortalOne->SetVisibility(bIsActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	MainCrosshair->Brush.SetResourceObject()
	UObject* ResourceObject = 
}

void UamsuCrosshair::UpdateCrosshairTwo(bool IsActive)
{
	CrosshairPortalTwo->SetVisibility(IsActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuCrosshair.h"

#include "Components/Image.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "Portal/PortalGunAndComponents/UamsuPortalGun.h"
#include "Portal/Portals/amsuPortal.h"

void UamsuCrosshair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MainCrosshair->SetVisibility(ESlateVisibility::Collapsed);
	//CrosshairPortalOne->SetVisibility(ESlateVisibility::Hidden);
	//CrosshairPortalTwo->SetVisibility(ESlateVisibility::Hidden);

	// TODO Modify or delete and just dont set any in the UImage
	CrosshairPortalOne->SetBrushFromTexture(nullptr);
	//CrosshairPortalOne->SetBrushTintColor(FLinearColor::Transparent);
	CrosshairPortalTwo->SetBrushFromTexture(nullptr);
	//CrosshairPortalTwo->SetBrushTintColor(FLinearColor::Transparent);
	// ^ TODO Modify or delete and just dont set any in the UImag ^
	
	SubscribeOnPortalGunPickedUp();
}

void UamsuCrosshair::NativeDestruct()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		PortalCharacter->OnGunPickedUp.Remove(BindCrosshairDelegateHandle);
	}
	
	Super::NativeDestruct();
}

void UamsuCrosshair::SubscribeOnPortalGunPickedUp()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		BindCrosshairDelegateHandle = PortalCharacter->OnGunPickedUp.AddUObject(this, &UamsuCrosshair::HandleGunPickedUp);
	}
}


void UamsuCrosshair::HandleGunPickedUp()
{
	APortalCharacter* const PortalCharacter = GetOwningPlayerPawn<APortalCharacter>();
	if (IsValid(PortalCharacter))
	{
		MainCrosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		PortalCharacter->PortalGun->PortalOne->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairOne);
		PortalCharacter->PortalGun->PortalTwo->OnPortalStateChange.AddUObject(this, &UamsuCrosshair::UpdateCrosshairTwo);
		
		PortalCharacter->OnGunPickedUp.Remove(BindCrosshairDelegateHandle);
	}
}
// TODO think about changing the image instead of having 4 widgets (like probably use two different textures (for enabled/disabled states))
void UamsuCrosshair::UpdateCrosshairOne(bool bIsActive)
{
	CrosshairPortalOne->SetBrushFromTexture(bIsActive ? ActiveCrosshairPortalOneTexture : DisabledCrosshairPortalOneTexture);
}

void UamsuCrosshair::UpdateCrosshairTwo(bool bIsActive)
{
	CrosshairPortalTwo->SetBrushFromTexture(bIsActive ? ActiveCrosshairPortalTwoTexture : DisabledCrosshairPortalTwoTexture);
}

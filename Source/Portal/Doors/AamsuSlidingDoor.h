// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "amsuDoorBase.h"
#include "AamsuSlidingDoor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class PORTAL_API AamsuSlidingDoor : public AamsuDoorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AamsuSlidingDoor();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Doors")
	TObjectPtr<UStaticMeshComponent> MeshComponentLeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Doors")
	TObjectPtr<UStaticMeshComponent> MeshComponentRightDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Doors")
	TObjectPtr<UBoxComponent> BoxOverlapComponent;

	/** The Maximum Range the door can slide open */
	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	float SlideRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	float OpenSpeed = 90.0f;

	void OpenDoor(float InDeltaTime);
	void CloseDoor(float InDeltaTime);

	virtual void Open() override;
	virtual void Close() override;
	
private:
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TickDoorSlide (float InDeltaTime, bool bDoorOpens);
	
	bool bDoorIsOpening = false;
	float CurrentPosition = 0.0f;

	/** This is the time in how much seconds the door will be closed in case it was opened by calling Open() function manually*/
	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	float CloseInSeconds = 10;

	void IsOpeningAndTickOn();
};

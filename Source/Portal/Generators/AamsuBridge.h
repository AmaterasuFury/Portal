// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AamsuBridge.generated.h"

class UArrowComponent;

UCLASS()
class PORTAL_API AamsuBridge : public AActor
{
	GENERATED_BODY()

public:
	AamsuBridge();

	virtual void OnConstruction(const FTransform& Transform) override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TObjectPtr<UStaticMeshComponent> MeshBridgeGenerator;

	/** This Mesh should have the length of 100.f units (centimeters) to work properly */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TObjectPtr<UStaticMeshComponent> Bridge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> InitialBridgeTransform;

	/** The bridge will not spawn if the distance is bigger then Max Bridge Length */
	UPROPERTY(EditAnywhere, Category = "Bridge")
	float  MaxBridgeLength = 8000.f;
	
	UPROPERTY(EditAnywhere, Category = "Bridge")
	float BridgePartLength = 100;
	
protected:
	virtual void BeginPlay() override;

	// todo delete the BlueprintCallable
	UFUNCTION(BlueprintCallable)
	void SpawnBridge();
	
	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> BridgeParts;
};

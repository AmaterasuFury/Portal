// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AamsuGenerator.generated.h"

class UArrowComponent;

UCLASS()
class PORTAL_API AamsuGenerator : public AActor
{
	GENERATED_BODY()

public:
	AamsuGenerator();

	virtual void OnConstruction(const FTransform& Transform) override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TObjectPtr<UStaticMeshComponent> MeshBridgeGenerator;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TObjectPtr<UStaticMeshComponent> Bridge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> InitialBridgeTransform;
	
	UPROPERTY(EditAnywhere, Category = "Bridge")
	float  MaxBridgeLength = 8000.f;

	/** This is the length of the default mesh for the bridge in the units (centimeters) (the Vector.Y),
	 * based on which the bridge generation is going to happen */
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

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TObjectPtr<UStaticMeshComponent> MeshBridgeGenerator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TObjectPtr<UStaticMeshComponent> BridgePart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> InitialBridgeTransform;

	// Todo add the bridge category
	UPROPERTY(EditAnywhere)
	float MaxBridgeLength = 1000.f;

	// Todo add the bridge category
	UPROPERTY(EditAnywhere)
	double BridgePartLength = 100;
protected:
	virtual void BeginPlay() override;

	// todo delete the BlueprintCallable
	UFUNCTION(BlueprintCallable)
	void SpawnBridge();

	UFUNCTION(BlueprintCallable)
	void DestroyBridge();
	
	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> BridgeParts;
	
	// todo make a linetrace to get distance and use the distance to spawn the bridge
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "CookingLiquidDataAsset.h"
#include "GameFramework/Actor.h"
#include "CookableContainer.generated.h"

UCLASS()
class KITCHENSIMULATOR_API ACookableContainer : public AContainer
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACookableContainer();

	UFUNCTION(BlueprintCallable, Category = "Container state")
	void UpdateCooking(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSubclassOf<AIngredient> IngredientBlueprintClass;

	UPROPERTY(BlueprintReadWrite, Category = "Container components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	TObjectPtr<UCookingLiquidDataAsset> CookingLiquidDataAsset;
	
	UPROPERTY()
	TObjectPtr<UIngredientDataAsset> WaterDataAsset;

	UFUNCTION()
	void CheckForCookedLiquids();

	UFUNCTION()
	float GetLiquidsCookingTime();

	UFUNCTION()
	void SpawnIngredient(UIngredientDataAsset* IngredientDataAsset, EIngredientState IngredientState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

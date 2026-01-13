// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
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

	float ScrambledEggsCookingTime = 10.0f;
	float ScrambledEggsBasicAmount = 0.240f;
	TSubclassOf<AIngredient> IngredientBlueprintClass;

	UPROPERTY()
	TObjectPtr<UIngredientDataAsset> EggDataAsset;
	
	UPROPERTY(BlueprintReadWrite, Category = "Container components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Container content")
	float LiquidsCookingTime = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

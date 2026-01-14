// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientDataAsset.h"
#include "Engine/DataAsset.h"
#include "CookingLiquidDataAsset.generated.h"

USTRUCT()
struct FCookedLiquidIngredient
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Min;
	
	UPROPERTY(EditAnywhere)
	float Max;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UIngredientDataAsset> Ingredient;
};

USTRUCT()
struct FLiquidRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FCookedLiquidIngredient> Ingredients;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UIngredientDataAsset> ResultIngredient;

	UPROPERTY(EditAnywhere)
	EIngredientState ResultIngredientState;

	UPROPERTY(EditAnywhere)
	float CookingTime;

	UPROPERTY(EditAnywhere)
	float MinimalAmount;
};

/**
 * 
 */
UCLASS()
class KITCHENSIMULATOR_API UCookingLiquidDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FLiquidRecipe> Recipes;
};

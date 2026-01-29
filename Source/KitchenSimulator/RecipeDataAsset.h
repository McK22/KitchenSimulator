// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientDataAsset.h"
#include "Engine/DataAsset.h"
#include "RecipeDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FIngredientInRecipe
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UIngredientDataAsset* IngredientDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredientState State;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CookingTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FryingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LiquidAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count;
};

/**
 * 
 */
UCLASS()
class KITCHENSIMULATOR_API URecipeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIngredientInRecipe> Ingredients;
};

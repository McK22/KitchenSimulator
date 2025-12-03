// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientDataAsset.h"
#include "Engine/DataAsset.h"
#include "MainDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class KITCHENSIMULATOR_API UMainDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ingredients")
	TArray<UIngredientDataAsset*> IngredientDataAssets;
};

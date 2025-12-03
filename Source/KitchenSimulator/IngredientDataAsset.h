// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IngredientDataAsset.generated.h"

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	Whole UMETA(DisplayName="Whole"),
	Cracked UMETA(DisplayName="Cracked"),
	Cut UMETA(DisplayName="Cut")
};

USTRUCT(BlueprintType)
struct FIngredientTransition
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<EIngredientState> AllowedStates;
};

/**
 * 
 */
UCLASS()
class KITCHENSIMULATOR_API UIngredientDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ingredient States")
	TMap<EIngredientState, UStaticMesh*> Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ingredient States")
	TMap<EIngredientState, FIngredientTransition> Transitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ingredient Image")
	TSoftObjectPtr<UTexture2D> Image;
};

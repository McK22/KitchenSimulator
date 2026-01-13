// Fill out your copyright notice in the Description page of Project Settings.


#include "CookableContainer.h"

// Sets default values
ACookableContainer::ACookableContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UIngredientDataAsset> EggDataAssetObject(TEXT("/Game/DataAssets/Ingredients/DA_Egg.DA_Egg"));
	EggDataAsset = EggDataAssetObject.Object;

	static ConstructorHelpers::FClassFinder<AIngredient> IngredientBp(TEXT("/Game/Ingredients/BP_Ingredient.BP_Ingredient_C"));
	IngredientBlueprintClass = IngredientBp.Class;
}

// Called when the game starts or when spawned
void ACookableContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACookableContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACookableContainer::UpdateCooking(float DeltaTime)
{
	for (AIngredient* Ingredient : Ingredients)
	{
		Ingredient->Cook(DeltaTime);
	}

	for (auto& Entry : LiquidIngredients)
	{
		Entry.Value.CookingTime += DeltaTime;
	}
	
	const float LiquidFill = GetLiquidFill();
	if (LiquidFill > 0.0f)
	{
		
		LiquidMaterialInstance->SetScalarParameterValue(
			LiquidCookingProgressParameterName,
			LiquidsCookingTime / ScrambledEggsCookingTime
		);
		
		LiquidsCookingTime += DeltaTime;
		if (LiquidsCookingTime >= ScrambledEggsCookingTime)
		{
			const FTransform Transform(AddIngredientArea->GetComponentLocation());
			AIngredient* Ingredient = GetWorld()->SpawnActorDeferred<AIngredient>(
				IngredientBlueprintClass,
				Transform,
				this,
				GetInstigator()
			);

			if (Ingredient)
			{
				Ingredient->IngredientData = EggDataAsset;
				Ingredient->SetState(EIngredientState::Scrambled);
				Ingredient->FinishSpawning(Transform);
				AddIngredient(Ingredient);
			}
			LiquidsCookingTime = 0.0f;
			LiquidIngredients.Empty();
			UpdateLiquidMeshPosition();
		}
	}
}

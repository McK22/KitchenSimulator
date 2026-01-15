// Fill out your copyright notice in the Description page of Project Settings.


#include "CookableContainer.h"

// Sets default values
ACookableContainer::ACookableContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// static ConstructorHelpers::FObjectFinder<UIngredientDataAsset> EggDataAssetObject(TEXT("/Game/DataAssets/Ingredients/DA_Egg.DA_Egg"));
	// EggDataAsset = EggDataAssetObject.Object;
	//
	// static ConstructorHelpers::FObjectFinder<UIngredientDataAsset> PancakeDataAssetObject(TEXT("/Game/DataAssets/Ingredients/DA_Pancake.DA_Pancake"));
	// PancakeDataAsset = PancakeDataAssetObject.Object;
	//
	static ConstructorHelpers::FObjectFinder<UCookingLiquidDataAsset> CookingLiquidDataAssetObject(TEXT("/Game/DataAssets/DA_CookingLiquid.DA_CookingLiquid"));
	CookingLiquidDataAsset = CookingLiquidDataAssetObject.Object;

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
	
	if (GetLiquidFill() > 0.0f)
	{
		CheckForCookedLiquids();
	}
}

void ACookableContainer::CheckForCookedLiquids()
{
	const float LiquidsCookingTime = GetLiquidsCookingTime();
	const float LiquidIngredientsFill = GetLiquidFill();
	for (const auto& Recipe : CookingLiquidDataAsset->Recipes)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("CookingTime: %f"), LiquidsCookingTime));
		if (LiquidsCookingTime < Recipe.CookingTime)
		{
			continue;
		}

		bool IngredientsOk = true;
		for (const auto& Ingredient : Recipe.Ingredients)
		{
			if (!LiquidIngredients.Contains(Ingredient.Ingredient))
			{
				if (Ingredient.Min > 0.0f)
				{
					IngredientsOk = false;
					break;
				}

				continue;
			}
			
			const float IngredientAmount = LiquidIngredients[Ingredient.Ingredient].Amount / LiquidIngredientsFill;
			if (IngredientAmount < Ingredient.Min || IngredientAmount > Ingredient.Max)
			{
				IngredientsOk = false;
				break;
			}
		}

		if (IngredientsOk)
		{
			SpawnIngredient(Recipe.ResultIngredient, Recipe.ResultIngredientState);
		}
	}
}

float ACookableContainer::GetLiquidsCookingTime()
{
	float Total = 0.0f;
	for (const auto& Entry : LiquidIngredients)
	{
		Total += Entry.Value.CookingTime * Entry.Value.Amount;
	}

	return Total / GetLiquidFill();
}

void ACookableContainer::SpawnIngredient(UIngredientDataAsset* IngredientDataAsset, EIngredientState IngredientState)
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
		Ingredient->IngredientData = IngredientDataAsset;
		Ingredient->SetState(IngredientState);
		Ingredient->FinishSpawning(Transform);
		AddIngredient(Ingredient);
	}
	
	LiquidIngredients.Empty();
	UpdateLiquidMesh();
}

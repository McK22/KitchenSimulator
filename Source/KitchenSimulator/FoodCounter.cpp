// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodCounter.h"

#include "MyGameInstance.h"
#include "Plate.h"
#include <string>

// Sets default values
AFoodCounter::AFoodCounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	VisualMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SetRootComponent(VisualMesh);

	PlacingArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placing Area"));
	PlacingArea->SetCollisionProfileName(TEXT("OverlapAll"));
	PlacingArea->SetHiddenInGame(true);
	PlacingArea->SetupAttachment(VisualMesh);
}

// Called when the game starts or when spawned
void AFoodCounter::BeginPlay()
{
	if (PlacingArea)
	{
		PlacingArea->OnComponentBeginOverlap.AddDynamic(this, &AFoodCounter::OnPlacingAreaBeginOverlap);
		PlacingArea->OnComponentEndOverlap.AddDynamic(this, &AFoodCounter::OnPlacingAreaEndOverlap);
	}
	
	Super::BeginPlay();
}

// Called every frame
void AFoodCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodCounter::Serve()
{
	if (!Plate)
	{
		return;
	}

	TArray<AActor*> AttachedActors;
	Plate->GetAttachedActors(AttachedActors);
	if (AttachedActors.Num() == 0)
	{
		return;
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance->SelectedRecipe)
	{
		SaveResults(GameInstance);
	}
	
	for (auto& Ingredient : AttachedActors)
	{
		Ingredient->Destroy();
	}
	
	Plate->Destroy();
	Plate = nullptr;
}

void AFoodCounter::SaveResults(UMyGameInstance* GameInstance) const
{
	TArray<FIngredientInRecipe> Liquids;
	for (auto& IngredientInRecipe : GameInstance->SelectedRecipe->Ingredients)
	{
		if (IngredientInRecipe.State == EIngredientState::Liquid)
		{
			Liquids.Add(IngredientInRecipe);
			continue;
		}
		
		const AIngredient* IngredientOnPlate = nullptr;
		for (auto& Ingredient : Plate->Ingredients)
		{
			if (Ingredient->IngredientData == IngredientInRecipe.IngredientDataAsset)
			{
				IngredientOnPlate = Ingredient;
				break;
			}
		}
		
		FString s = IngredientInRecipe.IngredientDataAsset->Name.ToString();
		if (!IngredientOnPlate)
		{
			s = "Brakujacy skladnik: " + s;
		}
		else
		{
			s +=
				" - Stan: "
				+ IngredientStateMap[IngredientOnPlate->State]
				+ (
					IngredientOnPlate->State == IngredientInRecipe.State
					? " Ok"
					: " (Poprawny: " + IngredientStateMap[IngredientInRecipe.State] + ")"
				);

			if (IngredientOnPlate->CookingTime || IngredientInRecipe.CookingTime)
			{
				s += FString::Printf(
					TEXT("\n\tCzas gotowania: %d:%02d/%d:%02d minut"),
					static_cast<int>(ceil(IngredientOnPlate->CookingTime)) / 60,
					static_cast<int>(ceil(IngredientOnPlate->CookingTime)) % 60,
					static_cast<int>(ceil(IngredientInRecipe.CookingTime)) / 60,
					static_cast<int>(ceil(IngredientInRecipe.CookingTime)) % 60
				);
			}
			
			if (IngredientOnPlate->FryingTime || IngredientInRecipe.FryingTime)
			{
				s += FString::Printf(
					TEXT("\n\tCzas smażenia: %d:%02d/%d:%02d minut"),
					static_cast<int>(ceil(IngredientOnPlate->FryingTime)) / 60,
					static_cast<int>(ceil(IngredientOnPlate->FryingTime)) % 60,
					static_cast<int>(ceil(IngredientInRecipe.FryingTime)) / 60,
					static_cast<int>(ceil(IngredientInRecipe.FryingTime)) % 60
				);
			}
		}
		
		GameInstance->Results.Add(FText::FromString(s));
	}
	
	for (auto& OnPlate : Plate->Ingredients)
	{
		bool IsInRecipe = false;
		for (auto& InRecipe : GameInstance->SelectedRecipe->Ingredients)
		{
			if (InRecipe.State != EIngredientState::Liquid && OnPlate->IngredientData == InRecipe.IngredientDataAsset)
			{
				IsInRecipe = true;
				break;
			}
		}
		
		if (!IsInRecipe)
		{
			GameInstance->Results.Add(FText::FromString("Nadmiarowy skladnik: " + OnPlate->IngredientData->Name.ToString()));
		}
	}
	
	SaveResultsForLiquids(GameInstance, Liquids);
}

void AFoodCounter::SaveResultsForLiquids(UMyGameInstance* GameInstance, TArray<FIngredientInRecipe> LiquidIngredients) const
{
	for (auto& IngredientInRecipe : LiquidIngredients)
	{
		FString s = IngredientInRecipe.IngredientDataAsset->Name.ToString();
		if (!Plate->LiquidIngredients.Contains(IngredientInRecipe.IngredientDataAsset))
		{
			s = "Brakujacy skladnik: " + s;
		}
		else
		{
			auto& Liquid = Plate->LiquidIngredients[IngredientInRecipe.IngredientDataAsset];
			s += FString::Printf(
				TEXT(" - %.2f/%.2f ml"),
				Liquid.Amount * 1000.0f,
				IngredientInRecipe.LiquidAmount * 1000.0f
			);
			if (Liquid.CookingTime || IngredientInRecipe.CookingTime)
			{
				s += FString::Printf(
					TEXT("\n\tCzas gotowania: %d:%02d/%d:%02d minut"),
					static_cast<int>(ceil(Liquid.CookingTime)) / 60,
					static_cast<int>(ceil(Liquid.CookingTime)) % 60,
					static_cast<int>(ceil(IngredientInRecipe.CookingTime)) / 60,
					static_cast<int>(ceil(IngredientInRecipe.CookingTime)) % 60
				);
			}
		}

		GameInstance->Results.Add(FText::FromString(s));
	}

	for (auto& OnPlate : Plate->LiquidIngredients)
	{
		bool IsInRecipe = false;
		for (auto& InRecipe : LiquidIngredients)
		{
			if (OnPlate.Key == InRecipe.IngredientDataAsset)
			{
				IsInRecipe = true;
				break;
			}
		}
		
		if (!IsInRecipe)
		{
			GameInstance->Results.Add(FText::FromString(FString::Printf(
				TEXT("Nadmiarowy skladnik: %s %.2fml"),
				*OnPlate.Key->Name.ToString(),
				OnPlate.Value.Amount * 1000.0f
			)));
		}
	}
}

void AFoodCounter::OnPlacingAreaBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)
{
	if (Plate)
	{
		return;
	}
	
	APlate* OverlappingPlate = Cast<APlate>(OtherActor);
	if (!OverlappingPlate)
	{
		return;
	}
	
	Plate = OverlappingPlate;
	OverlappedComponent->SetGenerateOverlapEvents(false);

	const FTransform NewTransform(
		FQuat(FRotator(0.0f, Plate->GetActorRotation().Yaw, 0.0f)),
		OverlappedComponent->GetComponentLocation(),
		Plate->GetActorScale()
	);
	
	Plate->GetVisualMesh()->SetPhysicsLinearVelocity(FVector());
	Plate->GetVisualMesh()->SetPhysicsAngularVelocityInRadians(FVector());
	Plate->SetActorTransform(NewTransform, true);
	
	OverlappedComponent->SetGenerateOverlapEvents(true);
}

void AFoodCounter::OnPlacingAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	)
{
	if (OtherActor == Plate)
	{
		Plate = nullptr;
	}
}

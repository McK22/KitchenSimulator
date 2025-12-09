#pragma once

#include "CoreMinimal.h"
#include "IngredientDataAsset.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

UCLASS()
class KITCHENSIMULATOR_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

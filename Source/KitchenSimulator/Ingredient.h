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

	UFUNCTION(BlueprintNativeEvent, Category = "State")
	void Cook(float DeltaTime);

	void Cook_Implementation(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Cooking time in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxCookingTime = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CookingTime = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

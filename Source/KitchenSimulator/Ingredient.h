#pragma once

#include "CoreMinimal.h"
#include "IngredientDataAsset.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

class AFoodCounter;
class ACookableContainer;

USTRUCT(BlueprintType)
struct FIngredientStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EIngredientState State = EIngredientState::Whole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta=(ExposeOnSpawn="true"))
	UIngredientDataAsset* IngredientData;
};

UCLASS()
class KITCHENSIMULATOR_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredient();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta=(ExposeOnSpawn="true"))
	UIngredientDataAsset* IngredientData;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	bool TryMakeTransition(EIngredientState NewState);

	bool TryMakeTransition_Implementation(EIngredientState NewState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void Cook(float DeltaTime, bool InWater);

	void Cook_Implementation(float DeltaTime, bool InWater);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void DisableCollision() const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void EnableCollision() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EIngredientState State = EIngredientState::Whole;
	
	// Cooking time in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxFryingTime = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CookingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FryingTime = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TMap<UIngredientDataAsset*, FLiquidIngredientStruct> LiquidComponents;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool SetState(EIngredientState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	friend ACookableContainer;
	friend AFoodCounter;
};

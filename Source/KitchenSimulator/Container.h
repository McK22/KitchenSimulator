#pragma once

#include "CoreMinimal.h"
#include "Ingredient.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

UCLASS()
class KITCHENSIMULATOR_API AContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AContainer();

	UFUNCTION(BlueprintCallable, Category = "Container content")
	void AddIngredient(AIngredient* Ingredient);

	UFUNCTION(BlueprintCallable, Category = "Container content")
	void AddLiquidIngredient(FIngredientStruct Ingredient, float AmountLiters);

	UFUNCTION(BlueprintPure, Category = "Components")
	UStaticMeshComponent* GetVisualMesh() const { return VisualMesh; }

	UFUNCTION(BlueprintCallable, Category = "Container content")
	float GetLiquidFill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAddIngredientAreaBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	UFUNCTION()
	void OnAddIngredientAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* LiquidIngredientsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	UStaticMeshComponent* AddIngredientArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container content")
	TArray<AIngredient*> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container content")
	TMap<UIngredientDataAsset*, float> LiquidIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	float MaxLiquidHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	float MinLiquidHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	float CapacityLiters;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

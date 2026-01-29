#pragma once

#include "CoreMinimal.h"
#include "Ingredient.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

class AFoodCounter;

UCLASS()
class KITCHENSIMULATOR_API AContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	float PourRate = 0.06f;

	UFUNCTION(BlueprintCallable, Category = "Container content")
	void AddIngredient(AIngredient* Ingredient);

	UFUNCTION(BlueprintCallable, Category = "Container content")
	void AddLiquidIngredient(UIngredientDataAsset* Ingredient, float AmountLiters);

	UFUNCTION(BlueprintPure, Category = "Components")
	UStaticMeshComponent* GetVisualMesh() const { return VisualMesh; }

	UFUNCTION(BlueprintCallable, Category = "Container content")
	float GetLiquidFill() const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	AContainer* GetContainerBelow() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const FName TotalLiquidParameterName = "TotalLiquidAmount";
	const FName LiquidCookingProgressParameterName = "CookingProgress";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* LiquidIngredientsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	UStaticMeshComponent* AddIngredientArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container content")
	TArray<AIngredient*> Ingredients;

	UPROPERTY(EditAnywhere, Category = "Container content")
	TMap<UIngredientDataAsset*, FLiquidIngredientStruct> LiquidIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	FVector MinLiquidLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	FVector MaxLiquidLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	FVector MinLiquidScale = {1.0f, 1.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	FVector MaxLiquidScale = {1.0f, 1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	float CapacityLiters;

	UPROPERTY(BlueprintReadOnly, Category = "Container content")
	UMaterialInstanceDynamic* LiquidMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container type")
	bool CanCollectLiquidFromOtherContainers;

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

	UFUNCTION()
	void OnLiquidIngredientBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRotatedDown() const;

	UFUNCTION(BlueprintCallable, Category = "Container content")
	void UpdateLiquidMesh() const;

	UFUNCTION(BlueprintCallable, Category = "Container content")
	void PourLiquid(const float LiquidAmountToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Container content")
	void DetachIngredients();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	friend AFoodCounter;
};

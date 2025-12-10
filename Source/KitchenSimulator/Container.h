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

	UFUNCTION(BlueprintPure, Category = "Components")
	UStaticMeshComponent* GetVisualMesh() const { return VisualMesh; }

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container content")
	UStaticMeshComponent* AddIngredientArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container content")
	TArray<AIngredient*> Ingredients;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

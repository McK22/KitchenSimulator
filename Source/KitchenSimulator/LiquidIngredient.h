#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "IngredientDataAsset.h"
#include "GameFramework/Actor.h"
#include "LiquidIngredient.generated.h"

UCLASS()
class KITCHENSIMULATOR_API ALiquidIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALiquidIngredient();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta=(ExposeOnSpawn="true"))
	UIngredientDataAsset* IngredientData;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	float PourRate = 0.06f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* PouringPoint;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRotatedDown() const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	AContainer* GetContainerBelow() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

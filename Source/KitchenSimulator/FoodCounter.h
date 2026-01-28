#pragma once

#include "CoreMinimal.h"
#include "Plate.h"
#include "GameFramework/Actor.h"
#include "FoodCounter.generated.h"

UCLASS()
class KITCHENSIMULATOR_API AFoodCounter : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoodCounter();

	UFUNCTION(BlueprintCallable)
	void Serve();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PlacingArea;

	UPROPERTY(BlueprintReadWrite)
	APlate* Plate;
	
	UFUNCTION()
	void OnPlacingAreaBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	UFUNCTION()
	void OnPlacingAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

public:	
	virtual void Tick(float DeltaTime) override;
};

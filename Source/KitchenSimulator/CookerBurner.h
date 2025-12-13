#pragma once

#include "CoreMinimal.h"
#include "CookableContainer.h"
#include "Components/SceneComponent.h"
#include "CookerBurner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KITCHENSIMULATOR_API UCookerBurner : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCookerBurner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Burner state")
	bool IsOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Burner state")
	ACookableContainer* Container;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBurnerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	UFUNCTION()
	void OnBurnerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

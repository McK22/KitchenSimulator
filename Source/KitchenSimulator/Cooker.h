#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cooker.generated.h"

UCLASS()
class KITCHENSIMULATOR_API ACooker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACooker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Manage containers | Pull a container")
	void OnBurnerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

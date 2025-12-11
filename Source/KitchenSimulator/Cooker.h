#pragma once

#include "CoreMinimal.h"
#include "CookerBurner.h"
#include "GameFramework/Actor.h"
#include "Cooker.generated.h"

UCLASS()
class KITCHENSIMULATOR_API ACooker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACooker();

	UFUNCTION(BlueprintCallable, Category = "Cooker components")
	UCookerBurner* GetBurner(const int Row, const int Column);
	
	UFUNCTION(BlueprintCallable, Category = "Cooker components")
	bool SetBurner(UCookerBurner* Burner, const int Row, const int Column);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker components")
	TArray<UCookerBurner*> Burners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker components")
	int BurnerRows = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker components")
	int BurnerColumns = 2;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

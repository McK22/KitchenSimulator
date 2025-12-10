#include "Cooker.h"

#include "CookableContainer.h"

// Sets default values
ACooker::ACooker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACooker::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACooker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#include "Cooker.h"

// Sets default values
ACooker::ACooker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Burners.SetNum(BurnerRows * BurnerColumns);
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

UCookerBurner* ACooker::GetBurner(const int Row, const int Column)
{
	if (Row < 0 || Row >= BurnerRows || Column < 0 || Column >= BurnerColumns)
	{
		return nullptr;
	}

	return Burners[Row * BurnerColumns + Column];
}

bool ACooker::SetBurner(UCookerBurner* Burner, const int Row, const int Column)
{
	if (Row < 0 || Row >= BurnerRows || Column < 0 || Column >= BurnerColumns)
	{
		return false;
	}

	Burners[Row * BurnerColumns + Column] = Burner;
	return true;
}


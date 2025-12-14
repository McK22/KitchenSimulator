#include "Ingredient.h"

// Sets default values
AIngredient::AIngredient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Mesh->SetSimulatePhysics(true);
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AIngredient::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AIngredient::SetState(EIngredientState NewState)
{
	if (!IngredientData
		|| !IngredientData->Meshes.Contains(NewState)
		|| !Mesh->SetStaticMesh(IngredientData->Meshes[NewState]))
	{
		return false;
	}
	
	State = NewState;
	return true;
}

bool AIngredient::TryMakeTransition(EIngredientState NewState)
{
	if (!IngredientData || !IngredientData->Transitions[State].AllowedStates.Contains(NewState))
	{
		return false;
	}

	SetState(NewState);
	return true;
}


void AIngredient::Cook_Implementation(float DeltaTime)
{
	CookingTime += DeltaTime;
}

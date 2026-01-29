#include "Ingredient.h"

#include "MyGameInstance.h"

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

	UStaticMesh* NewMesh = IngredientData->Meshes[NewState];
	const int32 MaterialCount = NewMesh->GetStaticMaterials().Num();
	for (int i = 0; i < MaterialCount; i++)
	{
		Mesh->SetMaterial(i, NewMesh->GetMaterial(i));
	}
	
	State = NewState;
	return true;
}

bool AIngredient::TryMakeTransition_Implementation(EIngredientState NewState)
{
	if (!IngredientData
		|| !IngredientData->Transitions.Contains(State)
		|| !IngredientData->Transitions[State].AllowedStates.Contains(NewState))
	{
		return false;
	}

	SetState(NewState);
	return true;
}

void AIngredient::Cook_Implementation(float DeltaTime, bool InWater)
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (InWater)
	{
		CookingTime += DeltaTime * GameInstance->TimeMultiplier;
	}
	else
	{
		FryingTime += DeltaTime * GameInstance->TimeMultiplier;
	}
}

void AIngredient::DisableCollision() const
{
	Mesh->SetCollisionProfileName("OverlapAll");
	Mesh->SetEnableGravity(false);
	Mesh->SetSimulatePhysics(false);
}

void AIngredient::EnableCollision() const
{
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	Mesh->SetEnableGravity(true);
	Mesh->SetSimulatePhysics(true);
}

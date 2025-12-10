#include "Container.h"

// Sets default values
AContainer::AContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	VisualMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	VisualMesh->SetSimulatePhysics(true);
	SetRootComponent(VisualMesh);

	AddIngredientArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Add Ingredient Area"));
	AddIngredientArea->SetCollisionProfileName(TEXT("OverlapAll"));
	AddIngredientArea->SetHiddenInGame(true);
	AddIngredientArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AContainer::BeginPlay()
{
	Super::BeginPlay();
	if (AddIngredientArea)
	{
		AddIngredientArea->OnComponentBeginOverlap.AddDynamic(this, &AContainer::OnAddIngredientAreaBeginOverlap);
		AddIngredientArea->OnComponentEndOverlap.AddDynamic(this, &AContainer::OnAddIngredientAreaEndOverlap);
	}
}

// Called every frame
void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AContainer::AddIngredient(AIngredient* Ingredient)
{
	Ingredients.Add(Ingredient);
	FVector NewLocation = FVector(
		Ingredient->GetActorLocation().X,
		Ingredient->GetActorLocation().Y,
		AddIngredientArea->GetComponentLocation().Z
	);
	Ingredient->SetActorLocation(NewLocation);
	Ingredient->SetActorRotation({0.0, Ingredient->GetActorRotation().Yaw, 0.0});

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true
	);
	Ingredient->AttachToActor(this, AttachmentRules, NAME_None);
}

void AContainer::OnAddIngredientAreaBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)
{
	if (AIngredient* Ingredient = Cast<AIngredient>(OtherActor))
	{
		AddIngredient(Ingredient);
	}
}

void AContainer::OnAddIngredientAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	)
{
	if (AIngredient* Ingredient = Cast<AIngredient>(OtherActor))
	{
		Ingredients.Remove(Ingredient);
	}
}


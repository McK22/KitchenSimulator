#include "LiquidIngredient.h"

// Sets default values
ALiquidIngredient::ALiquidIngredient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	MeshComponent->SetSimulatePhysics(true);
	SetRootComponent(MeshComponent);
	
	PouringPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Pouring Point"));
	PouringPoint->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ALiquidIngredient::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALiquidIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IngredientData && IsRotatedDown())
	{
		if (AContainer* ContainerBelow = GetContainerBelow())
		{
			ContainerBelow->AddLiquidIngredient(IngredientData, PourRate * DeltaTime);
		}	
	}
}

bool ALiquidIngredient::IsRotatedDown() const
{
	const FRotator Rotation = GetActorRotation();
	return abs(Rotation.Pitch) > 90.0f || abs(Rotation.Roll) > 90.0f;
}

AContainer* ALiquidIngredient::GetContainerBelow() const
{
	FHitResult HitResult;
	const FVector Start = PouringPoint->GetComponentLocation();
	const FVector End = Start + (FVector::DownVector * 400.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	const bool Hit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (Hit)
	{
		return Cast<AContainer>(HitResult.GetActor());
	}

	return nullptr;
}
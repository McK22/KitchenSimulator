#include "Container.h"

#include "VectorTypes.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AContainer::AContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	VisualMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	VisualMesh->SetSimulatePhysics(true);
	SetRootComponent(VisualMesh);

	LiquidIngredientsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Liquid Ingredients Mesh"));
	LiquidIngredientsMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	// LiquidIngredientsMesh->SetWorldScale3D({0.0f, 0.0f, 0.0f});
	LiquidIngredientsMesh->SetupAttachment(RootComponent);

	AddIngredientArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Add Ingredient Area"));
	AddIngredientArea->SetCollisionProfileName(TEXT("OverlapAll"));
	AddIngredientArea->SetHiddenInGame(true);
	AddIngredientArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AContainer::BeginPlay()
{
	if (AddIngredientArea)
	{
		AddIngredientArea->OnComponentBeginOverlap.AddDynamic(this, &AContainer::OnAddIngredientAreaBeginOverlap);
		AddIngredientArea->OnComponentEndOverlap.AddDynamic(this, &AContainer::OnAddIngredientAreaEndOverlap);
	}

	if (LiquidIngredientsMesh)
	{
		LiquidIngredientsMesh->OnComponentBeginOverlap.AddDynamic(this, &AContainer::OnLiquidIngredientBeginOverlap);
	}
	
	LiquidMaterialInstance = LiquidIngredientsMesh->CreateDynamicMaterialInstance(0);
	UpdateLiquidMesh();
	Super::BeginPlay();
}

// Called every frame
void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRotatedDown())
	{
		PourLiquid(PourRate * DeltaTime);
		DetachIngredients();
	}
}

void AContainer::AddIngredient(AIngredient* Ingredient)
{
	Ingredients.Add(Ingredient);
	const FVector NewLocation = FVector(
		Ingredient->GetActorLocation().X,
		Ingredient->GetActorLocation().Y,
		AddIngredientArea->GetComponentLocation().Z
	);
	// Ingredient->DisableCollision();
	Ingredient->SetActorRotation({0.0, Ingredient->GetActorRotation().Yaw, 0.0});
	Ingredient->SetActorLocation(NewLocation, true);

	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true
	);
	Ingredient->AttachToActor(this, AttachmentRules, NAME_None);
}

void AContainer::AddLiquidIngredient(UIngredientDataAsset* Ingredient, float AmountLiters, float NewCookingTime)
{
	const float LiquidFill = GetLiquidFill();
	AmountLiters = FMath::Min(CapacityLiters - LiquidFill, AmountLiters);
	if (AmountLiters < 1e-6)
	{
		return;
	}
	
	if (!LiquidIngredients.Contains(Ingredient))
	{
		LiquidIngredients.Add(Ingredient, {});
	}

	auto& [Amount, CookingTime] = LiquidIngredients[Ingredient];
	Amount += AmountLiters;
	CookingTime = (NewCookingTime * AmountLiters + CookingTime * (Amount - AmountLiters)) / Amount;

	UpdateLiquidMesh();
}

float AContainer::GetLiquidFill() const
{
	float Result = 0.0f;
	for (const auto Entry : LiquidIngredients)
	{
		Result += Entry.Value.Amount;
	}

	return Result;
}

AContainer* AContainer::GetContainerBelow() const
{
	FHitResult HitResult;
	const FVector Start = GetActorLocation();
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
		// Ingredient->EnableCollision();
	}
}

void AContainer::OnLiquidIngredientBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)
{
	AContainer* OtherContainer = Cast<AContainer>(OtherActor);
	if (OtherContainer && OtherContainer->CanCollectLiquidFromOtherContainers)
	{
		const float CurrentFill = GetLiquidFill();
		const float LiquidToTransfer = FMath::Min(CurrentFill, OtherContainer->CapacityLiters - OtherContainer->GetLiquidFill());
		const float Ratio = LiquidToTransfer / CurrentFill;
		for (auto& Liquid : LiquidIngredients)
		{
			float CurrentAmountToRemove = Liquid.Value.Amount * Ratio;
			CurrentAmountToRemove = FMath::Min(CurrentAmountToRemove, Liquid.Value.Amount);
			Liquid.Value.Amount -= CurrentAmountToRemove;
			OtherContainer->AddLiquidIngredient(Liquid.Key, CurrentAmountToRemove);
		}	
	}
}

bool AContainer::IsRotatedDown() const
{
	const FRotator Rotation = GetActorRotation();
	return abs(Rotation.Pitch) > 90.0f || abs(Rotation.Roll) > 90.0f;
}

void AContainer::UpdateLiquidMesh() const
{
	const float LiquidFill = GetLiquidFill();
	const bool Visible = LiquidFill > 1e-4;
	LiquidIngredientsMesh->SetVisibility(Visible);
	if (!Visible)
	{
		return;
	}

	const float Ratio = LiquidFill / CapacityLiters;
	const FVector NewLocation = FMath::Lerp(MinLiquidLocation, MaxLiquidLocation, Ratio);	
	LiquidIngredientsMesh->SetRelativeLocation(NewLocation);

	const FVector NewScale = FMath::Lerp(MinLiquidScale, MaxLiquidScale, Ratio);
	LiquidIngredientsMesh->SetRelativeScale3D(NewScale);

	if (LiquidFill < 1e-6)
	{
		return;
	}
	
	LiquidMaterialInstance->SetScalarParameterValue(TotalLiquidParameterName, LiquidFill);
	for (auto& Ingredient : LiquidIngredients)
	{
		LiquidMaterialInstance->SetScalarParameterValue(
			Ingredient.Key->LiquidMaterialParameterName,
			Ingredient.Value.Amount
		);
	}
}

void AContainer::PourLiquid(const float LiquidAmountToRemove)
{
	const float TotalLiquidAmount = GetLiquidFill();
	const float Ratio = LiquidAmountToRemove / TotalLiquidAmount;
	if (AContainer* ContainerBelow = GetContainerBelow())
	{
		for (auto& Liquid : LiquidIngredients)
		{
			float CurrentAmountToRemove = Liquid.Value.Amount * Ratio;
			CurrentAmountToRemove = FMath::Min(CurrentAmountToRemove, Liquid.Value.Amount);
			Liquid.Value.Amount -= CurrentAmountToRemove;
			ContainerBelow->AddLiquidIngredient(Liquid.Key, CurrentAmountToRemove);
		}
	}
	else
	{
		for (auto& Liquid : LiquidIngredients)
		{
			float CurrentAmountToRemove = Liquid.Value.Amount * Ratio;
			CurrentAmountToRemove = FMath::Min(CurrentAmountToRemove, Liquid.Value.Amount);
			Liquid.Value.Amount -= CurrentAmountToRemove;
		}
	}

	UpdateLiquidMesh();
}

void AContainer::DetachIngredients()
{
	const FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
	for (const auto& Ingredient : Ingredients)
	{
		Ingredient->DetachFromActor(Rules);
	}
}

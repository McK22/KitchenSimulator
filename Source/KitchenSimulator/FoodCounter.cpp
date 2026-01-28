// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodCounter.h"

#include "Plate.h"

// Sets default values
AFoodCounter::AFoodCounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	VisualMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SetRootComponent(VisualMesh);

	PlacingArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placing Area"));
	PlacingArea->SetCollisionProfileName(TEXT("OverlapAll"));
	PlacingArea->SetHiddenInGame(true);
	PlacingArea->SetupAttachment(VisualMesh);
}

// Called when the game starts or when spawned
void AFoodCounter::BeginPlay()
{
	if (PlacingArea)
	{
		PlacingArea->OnComponentBeginOverlap.AddDynamic(this, &AFoodCounter::OnPlacingAreaBeginOverlap);
		PlacingArea->OnComponentEndOverlap.AddDynamic(this, &AFoodCounter::OnPlacingAreaEndOverlap);
	}
	
	Super::BeginPlay();
}

// Called every frame
void AFoodCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodCounter::Serve()
{
	if (!Plate)
	{
		return;
	}

	Plate->Destroy();
}

void AFoodCounter::OnPlacingAreaBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)
{
	if (Plate)
	{
		return;
	}
	
	APlate* OverlappingPlate = Cast<APlate>(OtherActor);
	if (!OverlappingPlate)
	{
		return;
	}
	
	Plate = OverlappingPlate;
	OverlappedComponent->SetGenerateOverlapEvents(false);

	const FTransform NewTransform(
		FQuat(FRotator(0.0f, Plate->GetActorRotation().Yaw, 0.0f)),
		OverlappedComponent->GetComponentLocation(),
		Plate->GetActorScale()
	);
	
	Plate->GetVisualMesh()->SetPhysicsLinearVelocity(FVector());
	Plate->GetVisualMesh()->SetPhysicsAngularVelocityInRadians(FVector());
	Plate->SetActorTransform(NewTransform);
	
	OverlappedComponent->SetGenerateOverlapEvents(true);
}

void AFoodCounter::OnPlacingAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	)
{
	if (OtherActor == Plate)
	{
		Plate = nullptr;
	}
}

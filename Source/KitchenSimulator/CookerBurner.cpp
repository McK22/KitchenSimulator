#include "CookerBurner.h"

#include "CookableContainer.h"

// Sets default values for this component's properties
UCookerBurner::UCookerBurner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	OnComponentBeginOverlap.AddDynamic(this, &UCookerBurner::OnBurnerBeginOverlap);
	SetHiddenInGame(true);
	// ...
}


// Called when the game starts
void UCookerBurner::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionProfileName(TEXT("OverlapAll"));
	// ...
	
}


// Called every frame
void UCookerBurner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCookerBurner::OnBurnerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)
{
	ACookableContainer* Container = Cast<ACookableContainer>(OtherActor);
	if (!Container)
	{
		return;
	}

	OverlappedComponent->SetGenerateOverlapEvents(false);

	const FTransform NewTransform(
		FQuat(FRotator(0.0f, Container->GetActorRotation().Yaw, 0.0f)),
		OverlappedComponent->GetComponentLocation(),
		Container->GetActorScale()
	);
	
	Container->GetVisualMesh()->SetPhysicsLinearVelocity(FVector());
	Container->GetVisualMesh()->SetPhysicsAngularVelocityInRadians(FVector());
	Container->SetActorTransform(NewTransform);
	
	OverlappedComponent->SetGenerateOverlapEvents(true);
}

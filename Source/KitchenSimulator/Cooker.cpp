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

void ACooker::OnBurnerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor)
{
	ACookableContainer* Container = Cast<ACookableContainer>(OtherActor);
	if (!Container)
	{
		return;
	}

	OverlappedComponent->SetGenerateOverlapEvents(false);

	FTransform newTransform(
		FQuat(FRotator(0.0f, Container->GetActorRotation().Yaw, 0.0f)),
		OverlappedComponent->GetComponentLocation(),
		FVector(1.0f)
	);
	
	Container->GetMeshComponent()->SetPhysicsLinearVelocity(FVector());
	Container->GetMeshComponent()->SetPhysicsAngularVelocityInRadians(FVector());
	Container->SetActorTransform(newTransform);
	
	OverlappedComponent->SetGenerateOverlapEvents(true);
}

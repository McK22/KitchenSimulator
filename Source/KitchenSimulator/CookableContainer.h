// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CookableContainer.generated.h"

UCLASS()
class KITCHENSIMULATOR_API ACookableContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACookableContainer();

	UFUNCTION(BlueprintPure, Category = "Container components")
	UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Container components")
	UStaticMeshComponent* MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

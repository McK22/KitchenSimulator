#pragma once

#include "CoreMinimal.h"
#include "RecipeDataAsset.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MyGameInstance.generated.h"

UCLASS()
class KITCHENSIMULATOR_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ActiveMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URecipeDataAsset* SelectedRecipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Results;
};

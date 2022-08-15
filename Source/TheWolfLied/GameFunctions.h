// Made by Kristjanmn

#pragma once

#include "TWLGameInstance.h"
#include "GameFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE(FFunctionName);

class ATWLWorldManager;
class UTWLGameInstance;

UCLASS()
class THEWOLFLIED_API UGameFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
	void ChangePlayerController(APlayerController* OldController, APlayerController* NewController);

	UFUNCTION(BlueprintPure, Category = "GameFunctions")
	static void FloatInRange(float ValueIn, float Min, float Max, float &ValueOut);

	UFUNCTION(BlueprintPure, Category = "GameFunctions")
	static bool IsIntegerInRange(int ValueIn, int Min, int Max);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
	void DelayTimer(UWorld* World, float Seconds);

	ATWLWorldManager* GetWorldManager();
};
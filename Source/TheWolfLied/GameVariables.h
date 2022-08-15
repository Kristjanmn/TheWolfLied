// Made by Kristjanmn

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameVariables.generated.h"

UCLASS()
class THEWOLFLIED_API UGameVariables : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
public:
	//ShowConfirmMessages
	UFUNCTION(BlueprintPure, Category = "Game Variables")
	static bool GetShowConfirmMessages();

	UFUNCTION(BlueprintCallable, Category = "Game Variables")
	static void SetShowConfirmMessages(const bool ShowConfirmMessages);

	//Time of Day
	UFUNCTION(BlueprintCallable, Category = "Game Variables")
	static int GetMinutesPerDayNight();

	UFUNCTION()
	static void SetMinutesPerDayNight(int MinutesPerDayNight);

	//Configured
	UFUNCTION()
	static bool GetConfigured();

	UFUNCTION()
	static void SetConfigured(const bool Configured);

	UFUNCTION()
	static int GetInventoryColumns();

	UFUNCTION()
	static void SetInventoryColumns(int Columns);

	UFUNCTION(BlueprintCallable, Category = "Game Variables")
	static int GetInventoryIconSize();

	UFUNCTION()
	static void SetInventoryIconSize(int IconSize);

	//Get game version
	UFUNCTION(BlueprintPure, Category = "Game Variables")
	static FString GetProjectVersion();
};
// Made by Kristjanmn

#pragma once

#include "Engine/GameInstance.h"
#include "../MoviePlayer/Public/MoviePlayer.h"
#include "TWLGameInstance.generated.h"

class ATWLWorldManager;
class ATWLPlayerController;
class UTWLQuest;

UCLASS()
class THEWOLFLIED_API UTWLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTWLGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	ATWLWorldManager* WorldManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	bool bInMainMenu = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Time")
	int TimeOfDayHours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Time")
	int TimeOfDayMinutes;

	// For World, maybe different lenghts in different worlds? Defined it here to not hard code it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Config")
	int SecondsInMinute = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Config")
	int MinutesInHour = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Config")
	int HoursInDay = 24;

	// Save/stats related variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Stats")
	int DaysInGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Stats")
	int TimePlayed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|Quests")
	TArray<UTWLQuest*> Quests;		// Right place or not? Maybe in QuestManager (Will it work properly?) or on character (unlikely)?

	void Loading();
	void Loading_InProgress();
	void Loading_Finish();
	virtual void BeginLoadingScreen(const FString &MapName);
	virtual	void EndLoadingScreen();
	TArray<FSubLevelStatus> GetSubLevelsStatus(UWorld* World);
};
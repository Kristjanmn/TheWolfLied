// Made by Kristjanmn

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TWLScriptManager.generated.h"

class ATWLWorldManager;
class ATWLLifeManager;
class ATWLDialogManager;
class ATWLQuestManager;
class UTWLQuest;
class ATWLWeatherManager;
class ATWLPlayerController;
class ATWLCharacter;
class ATWLItem;

// Script enums
UENUM(BlueprintType)
enum class EPassTime : uint8
{
	For		UMETA(DisplayName = "WaitFor"),
	Until	UMETA(DisplayName = "WaitUntil")
};

UCLASS()
class THEWOLFLIED_API ATWLScriptManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATWLScriptManager();

	ATWLWorldManager* WorldManager;
	ATWLLifeManager* LifeManager;
	ATWLDialogManager* DialogManager;
	ATWLQuestManager* QuestManager;
	ATWLWeatherManager* WeatherManager;
	ATWLPlayerController* PlayerController;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PassTime(EPassTime Method, int Hours, int Minutes);

	UFUNCTION()
	void GiveItem(FName From, FName To, FName ItemID);

	UFUNCTION()
	void GiveMoney(FName From, FName To, int Amount);

	UFUNCTION()
	void ChangeName(ATWLCharacter* Character, FString NewName);

	// Quest
	UFUNCTION()
	void UpdateQuestDescription(FName QuestID, FText Description);

	UFUNCTION()
	void SetQuestState(FName QuestID, EQuestState State);

	UFUNCTION()
	void SetQuestPhase(FName QuestID, FName Phase);

	// Dialog
	UFUNCTION()		// CharacterID -1 defaults to NPC from DialogManager.
	void SetDialogLocked(FName CharacterID, FName LineID, bool Locked);

	UFUNCTION()		// Same as above, but for multiple lines.
	void SetDialogsLocked(FName CharacterID, TArray<FName> LineIDs, bool Locked);

	UFUNCTION()		// Enables to change the NextLine inside dialog
	void SetNextLine(FName CharacterID = "-1", FName LineID = "INIT", FName NextLine = "0");

	// Weather
	UFUNCTION()		// Set weather simply by it's name.
	void SetWeather(FName WeatherName = "Clear");

	// Other
	const FString EnumToString(const TCHAR* Enum, int32 EnumValue);
};

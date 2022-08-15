// Made by Kristjanmn

#pragma once

#include "GameFramework/GameModeBase.h"
#include "TWLWorldManager.h"
#include "TWLPlayerController.h"
#include "TWLGameMode.generated.h"

class ATWLWorldManager;
class ATWLCharacter;
class ATWLItem;

UCLASS()
class THEWOLFLIED_API ATWLGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATWLGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	// Variables
	ATWLPlayerController* PlayerController;
	ATWLWorldManager* WorldManager;

	// Functions
	void UpdateGameMode();

	UFUNCTION(Exec)
	void SetTimeOfDay(int Hours, int Minutes);

	void GiveItem(ATWLCharacter* To, ATWLCharacter* From, ATWLItem* Item);
};

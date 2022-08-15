// Made by Kristjanmn

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TWLQuest.h"
#include "TWLQuestManager.generated.h"

class ATWLWorldManager;
class ATWLDialogManager;
class ATWLScriptManager;

UCLASS()
class THEWOLFLIED_API ATWLQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWLQuestManager(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ATWLWorldManager* WorldManager;
	ATWLDialogManager* DialogManager;
	ATWLScriptManager* ScriptManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTWLQuest*> Quests;

	void UpdateQuestDescription(FName QuestID, FText Description);
	void SetQuestState(FName QuestID, EQuestState State);
	void SetQuestPhase(FName QuestID, FName Phase);
};
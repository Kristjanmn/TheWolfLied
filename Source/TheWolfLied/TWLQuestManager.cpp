// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLQuestManager.h"
#include "TWLWorldManager.h"
#include "TWLDialogManager.h"
#include "TWLScriptManager.h"

// Sets default values
ATWLQuestManager::ATWLQuestManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATWLQuestManager::BeginPlay()
{
	Super::BeginPlay();

	UObjectLibrary* QuestLibrary = UObjectLibrary::CreateLibrary(UTWLQuest::StaticClass(), true, GIsEditor);
	QuestLibrary->AddToRoot();
	QuestLibrary->LoadBlueprintsFromPath(TEXT("/Game/Data/Quest"));
	TArray<UBlueprintGeneratedClass*> LibraryQuests;
	QuestLibrary->GetObjects<UBlueprintGeneratedClass>(LibraryQuests);

	int i;
	for (i = 0; i < LibraryQuests.Num(); i++)
	{
		if (!LibraryQuests[i]->GetName().StartsWith("SKEL_"))
		{
			UTWLQuest* Quest = Cast<UTWLQuest>(LibraryQuests[i]->GetDefaultObject());
			Quest->QuestManager = this;
			Quests.AddUnique(Quest);
			UE_LOG(LogQuestManager, Log, TEXT("Added quest %s"), *Quest->ID.ToString());
		}
	}
}

// Called every frame
void ATWLQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATWLQuestManager::UpdateQuestDescription(FName QuestID, FText Description)
{
	UTWLQuest* Quest = nullptr;
	int i;
	for (i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i]->ID == QuestID)
		{
			Quest = Quests[i];
		}
	}

	if (!Quest)
	{
		UE_LOG(LogQuestManager, Fatal, TEXT("UpdateQuestDescription in QuestManager failed! Invalid QuestID %s"), *QuestID.ToString());
	}

	FString NewDescription = *Quest->Description.ToString();
	NewDescription.Append("\n---------------\n");
	NewDescription.Append(*Description.ToString());
	Quest->Description = FText::AsCultureInvariant(*NewDescription);
}

void ATWLQuestManager::SetQuestState(FName QuestID, EQuestState State)
{
	UTWLQuest* Quest = nullptr;
	int i;
	for (i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i]->ID == QuestID)
		{
			Quest = Quests[i];
		}
	}

	if (!Quest)
	{
		UE_LOG(LogQuestManager, Fatal, TEXT("SetQuestState in QuestManager failed! Invalid QuestID %s"), *QuestID.ToString());
	}

	if (Quest->State != EQuestState::Completed && Quest->State != EQuestState::Failed)
	{
		if (Quest->State != State)
		{
			Quest->SetState(State);
			// Add visual/audio effect
		}
	}
}

void ATWLQuestManager::SetQuestPhase(FName QuestID, FName Phase)
{
	UTWLQuest* Quest = nullptr;
	int i;
	for (i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i]->ID == QuestID)
		{
			Quest = Quests[i];
		}
	}

	if (!Quest)
	{
		UE_LOG(LogQuestManager, Fatal, TEXT("SetQuestPhase in QuestManager failed! Invalid QuestID %s"), *QuestID.ToString());
	}

	Quest->SetPhase(Phase);
	// Add visual/audio effect
	UE_LOG(LogQuestManager, Log, TEXT("%s phase %s"), *QuestID.ToString(), *Phase.ToString());
}
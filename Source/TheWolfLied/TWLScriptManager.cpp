// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLScriptManager.h"
#include "TWLWorldManager.h"
#include "TWLLifeManager.h"
#include "TWLDialogManager.h"
#include "TWLQuestManager.h"
#include "TWLQuest.h"
#include "TWLWeatherManager.h"
#include "TWLPlayerController.h"
#include "TWLCharacter.h"
#include "TWLItem.h"

// Sets default values
ATWLScriptManager::ATWLScriptManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATWLScriptManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATWLScriptManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATWLScriptManager::PassTime(EPassTime Method, int Hours, int Minutes)
{
	int Hour = 0;
	int Minute = 0;
	switch (Method)
	{
	case EPassTime::For:
		WorldManager->GetTime(Hour, Minute);
		Hour = (Hour + Hours);
		Minute = (Minute + Minutes);
		WorldManager->SetTimeOfDay(Hour, Minute);
		break;
	case EPassTime::Until:
		WorldManager->SetTimeOfDay(Hours, Minutes);
		break;
	}
}

void ATWLScriptManager::GiveItem(FName From, FName To, FName ItemID)
{
	ATWLCharacter* FromCharacter = nullptr;
	ATWLCharacter* ToCharacter = nullptr;
	ATWLItem* Item = nullptr;
	if (From == "PC")
	{
		FromCharacter = DialogManager->PC;
		ToCharacter = DialogManager->NPC;
	}
	else if (From == "NPC")
	{
		FromCharacter = DialogManager->NPC;
		ToCharacter = DialogManager->PC;
	}
	else  //give item from NPC to NPC
	{
		int i;
		for (i = 0; i < WorldManager->LifeManager->Characters.Num(); i++)
		{
			if (WorldManager->LifeManager->Characters[i]->CharacterID == From)
			{
				FromCharacter = WorldManager->LifeManager->Characters[i];
			}
			if (WorldManager->LifeManager->Characters[i]->CharacterID == To)
			{
				ToCharacter = WorldManager->LifeManager->Characters[i];
			}
		}
	}
	if (!FromCharacter || !ToCharacter)
	{
		UE_LOG(LogScriptManager, Fatal, TEXT("One of these characters was not found: %s or %s"), *From.ToString(), *To.ToString());
	}
	for (int i = 0; i < FromCharacter->Inventory.Num(); i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s"), *FromCharacter->Inventory[i]->ItemID.ToString()));
		if (FromCharacter->Inventory[i]->ItemID == ItemID)
		{
			Item = FromCharacter->Inventory[i];
		}
	}
	if (Item)
	{
		FromCharacter->Inventory.Remove(Item);
		ToCharacter->Inventory.Add(Item);
		UE_LOG(LogScriptManager, Log, TEXT("%s gave %s to %s"), *FromCharacter->Name, *Item->ItemID.ToString(), *ToCharacter->Name);
	}
	else
	{
		UE_LOG(LogScriptManager, Error, TEXT("Character %s does not have item %s!"), *FromCharacter->Name, *ItemID.ToString());
	}
}

void ATWLScriptManager::GiveMoney(FName From, FName To, int Amount)
{
	ATWLCharacter* FromCharacter = nullptr;
	ATWLCharacter* ToCharacter = nullptr;
	ATWLItem* Currency = nullptr;
	if (From == "PC")
	{
		FromCharacter = DialogManager->PC;
		ToCharacter = DialogManager->NPC;
	}
	else if (From == "NPC")
	{
		FromCharacter = DialogManager->NPC;
		ToCharacter = DialogManager->PC;
	}
	else
	{
		int i;
		for (i = 0; i < WorldManager->LifeManager->Characters.Num(); i++)
		{
			if (WorldManager->LifeManager->Characters[i]->CharacterID == From)
			{
				FromCharacter = WorldManager->LifeManager->Characters[i];
			}
			if (WorldManager->LifeManager->Characters[i]->CharacterID == To)
			{
				ToCharacter = WorldManager->LifeManager->Characters[i];
			}
		}
	}
	if (!FromCharacter || !ToCharacter)
	{
		UE_LOG(LogScriptManager, Fatal, TEXT("One of these characters was not found: %s or %s"), *From.ToString(), *To.ToString());
	}
	// work some magic here
}

void ATWLScriptManager::ChangeName(ATWLCharacter* Character, FString NewName)
{
	Character->Name = NewName;
	Character->UpdateName();
}

// Quest
void ATWLScriptManager::UpdateQuestDescription(FName QuestID, FText Description)	// Move all that shit over here to remove this annoying shit error shit storm! and also the enums
{
	QuestManager->UpdateQuestDescription(QuestID, Description);
}

void ATWLScriptManager::SetQuestState(FName QuestID, EQuestState State)
{
	QuestManager->SetQuestState(QuestID, State);
}

void ATWLScriptManager::SetQuestPhase(FName QuestID, FName Phase)
{
	QuestManager->SetQuestPhase(QuestID, Phase);
}

// Dialog
void ATWLScriptManager::SetDialogLocked(FName CharacterID, FName LineID, bool Locked)
{
	FString ContextString;
	if (CharacterID == "-1")
	{
		if (DialogManager)
		{
			if (LineID == "-1")
			{
				DialogManager->NPC->DialogData->FindRow<FDialogData>(DialogManager->LineID, ContextString)->Locked = Locked;
				UE_LOG(LogScriptManager, Log, TEXT("Locked line %s for %s"), *DialogManager->LineID.ToString(), *DialogManager->NPC->CharacterID.ToString());
			}
			else
			{
				DialogManager->NPC->DialogData->FindRow<FDialogData>(LineID, ContextString)->Locked = Locked;
				UE_LOG(LogScriptManager, Log, TEXT("Locked line %s for %s"), *LineID.ToString(), *DialogManager->NPC->CharacterID.ToString());
			}
		}
		else
		{
			UE_LOG(LogScriptManager, Error, TEXT("CharacterID not set and not in dialog."));
			UE_LOG(LogScriptManager, Log, TEXT("Locked line %s for %s"), *DialogManager->LineID.ToString(), *CharacterID.ToString());
		}
	}
	else
	{
		LifeManager->GetCharacterByID(CharacterID)->DialogData->FindRow<FDialogData>(LineID, ContextString)->Locked = Locked;
		UE_LOG(LogScriptManager, Log, TEXT("Locked line %s for %s"), *DialogManager->LineID.ToString(), *CharacterID.ToString());
	}
}

void ATWLScriptManager::SetDialogsLocked(FName CharacterID, TArray<FName> LineIDs, bool Locked)
{
	FString ContextString;
	UDataTable* DialogData = nullptr;
	if (CharacterID == "-1")
	{
		if (DialogManager)
		{
			DialogData = DialogManager->NPC->DialogData;
		}
		else
		{
			UE_LOG(LogScriptManager, Error, TEXT("CharacterID not set and not in dialog."));
		}
	}
	else if (CharacterID.IsValid())
	{
		DialogData = LifeManager->GetCharacterByID(CharacterID)->DialogData;
	}
	else
	{
		UE_LOG(LogScriptManager, Fatal, TEXT("CharacterID was not set!"));
	}
	if (DialogData)
	{
		int i;
		for (i = 0; i < LineIDs.Num(); i++)
		{
			DialogData->FindRow<FDialogData>(LineIDs[i], ContextString)->Locked = Locked;
			UE_LOG(LogScriptManager, Log, TEXT("Changed Locked state to %s for line %s for %s"), Locked ? TEXT("True") : TEXT("False"), *LineIDs[i].ToString(), *CharacterID.ToString());
		}
	}
	else
	{
		UE_LOG(LogScriptManager, Fatal, TEXT("DialogData was not found for character %s"), *CharacterID.ToString())
	}
}

void ATWLScriptManager::SetNextLine(FName CharacterID, FName LineID, FName NextLine)
{
	FString ContextString;
	if (CharacterID == "-1")
	{
		if (DialogManager)
		{
			DialogManager->NPC->DialogData->FindRow<FDialogData>(LineID, ContextString)->NextLine = NextLine;
		}
		else
		{
			UE_LOG(LogScriptManager, Error, TEXT("CharacterID not set and not in dialog."));
		}
	}
	else if (CharacterID.IsValid())
	{
		LifeManager->GetCharacterByID(CharacterID)->DialogData->FindRow<FDialogData>(LineID, ContextString)->NextLine = NextLine;
		UE_LOG(LogScriptManager, Log, TEXT("Set NextLine to %s for line %s in %s"), *NextLine.ToString(), *LineID.ToString(), *CharacterID.ToString());
	}
	else
	{
		UE_LOG(LogScriptManager, Fatal, TEXT("CharacterID was not set!"));
	}
}

// Weather
void ATWLScriptManager::SetWeather(FName WeatherName) {
	if (WeatherManager) {
		UE_LOG(LogScriptManager, Log, TEXT("Calling WeatherManager to set weather to %s"), *WeatherName.ToString());
		WeatherManager->SetWeather(WeatherName);
	} else {
		UE_LOG(LogScriptManager, Fatal, TEXT("WeatherManager could not be found!"));
	}
}

// Other
const FString ATWLScriptManager::EnumToString(const TCHAR* Enum, int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
	if (!EnumPtr)
		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
	return EnumPtr->GetDisplayNameTextByIndex(EnumValue).ToString();
#else
	return EnumPtr->GetEnumName(EnumValue);
#endif
}
// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLQuest.h"
#include "TWLQuestManager.h"
#include "TWLScriptManager.h"

void UTWLQuest::SetState(EQuestState NewState)
{
	ScriptManager = QuestManager->ScriptManager;
	if (State == EQuestState::UnAccepted || State == EQuestState::Accepted)			// Run the script on accepting the quest, does not run if quest is for some reason set to failed.
	{
		State = NewState;
		FString ContextString;
		QuestRow = Phases->FindRow<FQuestData>(Phase, ContextString);
		if (QuestRow->Script.Num() > 0)
		{
			if (!ScriptManager)
			{
				UE_LOG(LogGame, Fatal, TEXT("ScriptManager not set in Quest %s"), *ID.ToString());
			}
			int i;
			for (i = 0; i < QuestRow->Script.Num(); i++)
			{
				FOutputDeviceDebug Debug;
				ScriptManager->CallFunctionByNameWithArguments(*QuestRow->Script[i], Debug, NULL, true);
			}
		}
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Trying to set finished quest %s state to %s!"), *ID.ToString(), *ScriptManager->EnumToString(TEXT("EQuestState"), StaticCast<uint8>(NewState)));
	}
	UE_LOG(LogGame, Log, TEXT("%s %s"), *ID.ToString(), *ScriptManager->EnumToString(TEXT("EQuestState"), StaticCast<uint8>(NewState)));
}

void UTWLQuest::SetPhase(FName PhaseName)
{
	// TODO: Prevent going back in Phases, force only going forward.
	if (State == EQuestState::Accepted)
	{
		ScriptManager = QuestManager->ScriptManager;
		Phase = PhaseName;
		FString ContextString;
		QuestRow = Phases->FindRow<FQuestData>(Phase, ContextString);
		if (!QuestRow)
		{
			UE_LOG(LogGame, Fatal, TEXT("Could not find phase %s in quest %s"), *Phase.ToString(), *ID.ToString());
		}

		if (!QuestRow->Description.IsEmpty())
		{
			FString NewDescription;
			if (!Description.IsEmpty())
			{
				NewDescription = *Description.ToString();
				NewDescription.Append("\n---------------\n");
				NewDescription.Append(*QuestRow->Description.ToString());
			}
			else
			{
				NewDescription = *QuestRow->Description.ToString();
			}
			Description = FText::AsCultureInvariant(NewDescription);
		}

		if (QuestRow->Script.Num() > 0)
		{
			if (!ScriptManager)
			{
				UE_LOG(LogGame, Fatal, TEXT("ScriptManager not set in Quest %s"), *ID.ToString());
			}
			int i;
			for (i = 0; i < QuestRow->Script.Num(); i++)
			{
				FOutputDeviceDebug Debug;
				ScriptManager->CallFunctionByNameWithArguments(*QuestRow->Script[i], Debug, NULL, true);
			}
		}
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Trying to set phase %s in quest %s that is not in accepted state!"), *PhaseName.ToString(), *ID.ToString());
	}
}
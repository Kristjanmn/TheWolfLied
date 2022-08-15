// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLDialogManager.h"
#include "TWLWorldManager.h"
#include "TWLQuestManager.h"
#include "TWLScriptManager.h"
#include "TWLUserWidget.h"
#include "TWLPlayerController.h"
#include "TWLCharacter.h"

// Sets default values
ATWLDialogManager::ATWLDialogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATWLDialogManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATWLDialogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATWLDialogManager::DialogInit(UDataTable* InDialogData, FString InPCName, FString InNPCName)
{
	if (GetWorld())
	{
		/*if (WorldManager->DialogCues.Num() == 0)
		{
			WorldManager->LoadDialogCues();
		}*/
		DialogData = InDialogData;
		PlayerController->WidgetDialog = CreateWidget<UTWLUserWidget>(PlayerController, PlayerController->WidgetDialogClass);
		PlayerController->WidgetDialog->WidgetType = EWidgetType::Dialog;
		PlayerController->WidgetDialog->DialogManager = this;
		PlayerController->WidgetDialog->PlayerController = PlayerController;
		PCName = InPCName;
		NPCName = InNPCName;
		PlayerController->WidgetDialog->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
		PlayerController->PlayerCharacter->bBusy = true;
		DialogStart();
	}
	else
	{
		UE_LOG(LogGame, Fatal, TEXT("Can't get world"));
	}
}

void ATWLDialogManager::DialogStart()
{
	FString ContextString;
	if (LineID != FName(TEXT("None")))
	{
		DialogRow = DialogData->FindRow<FDialogData>(LineID, ContextString);
		WaitTime = DialogRow->WaitTime;
		if (!DialogRow->Locked)
		{
			DialogType = DialogRow->Type;
			DialogSpeaker = DialogRow->Speaker;

			// Run script(s)
			if (DialogRow->Script.Num() > 0)
			{
				int i;
				for (i = 0; i < DialogRow->Script.Num(); i++)
				{
					//const TCHAR* Command = *DialogRow->Script[i];						// Those two are for if I use
					//PlayerController->ConsoleCommand(DialogRow->Script[i]);			// FString, instead of FName.
					//UObject::CallFunctionByNameWithArguments(Command, Debug, NULL, true);
					FOutputDeviceDebug Debug;
					ScriptManager->CallFunctionByNameWithArguments(*DialogRow->Script[i], Debug, NULL, true);	// Can only call to UFUNCTION
				}
			}

			if (LineID == FName(TEXT("INIT")))			// If Initialization line, then skip to next line as this line is for scripting only.
			{
				LineID = DialogRow->NextLine;
				WaitTime = 0.02f;						// Fixes double choices bug
				FTimerHandle InitTimer;
				GetWorld()->GetTimerManager().SetTimer(InitTimer, this, &ATWLDialogManager::DialogNext, 0.01f, false);		// Fixes double choices and text visibility bug, which persisted with WaitTime method.
			}

			switch (DialogSpeaker)
			{
			case EDialogSpeaker::None:
				PlayerController->WidgetDialog->SpeakerName = FText::GetEmpty();		// *Background screams of horror*
				break;
			case EDialogSpeaker::PC:
				PlayerController->WidgetDialog->SpeakerName = FText::AsCultureInvariant(PCName);
				break;
			case EDialogSpeaker::NPC:
				PlayerController->WidgetDialog->SpeakerName = FText::AsCultureInvariant(NPCName);
				break;
			}

			switch (DialogType)
			{
			case EDialogType::End:
				bLastLine = true;
				DialogNext();
				break;
			case EDialogType::Next:
				DialogNext();
				break;
			case EDialogType::Choice:
				DialogChoice();
				break;
			}
		}
		else
		{
			LineID = DialogRow->NextLine;
			DialogNext_Finish();
		}
	}
}

void ATWLDialogManager::DialogEnd()
{
	PlayerController->PlayerCharacter->bBusy = false;
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->WidgetDialog->RemoveFromParent();
	this->Destroy();
}

void ATWLDialogManager::DialogNext()
{
	if (LineID != FName(TEXT("INIT")))
	{
		PlayerController->WidgetDialog->DialogVisibility = ESlateVisibility::Visible;
	}
	PlayerController->WidgetDialog->PlayAnimation(PlayerController->WidgetDialog->DialogOpen);
	LineID = DialogRow->NextLine;
	PlayerController->WidgetDialog->DialogText = DialogRow->Text;
	if (!DialogRow->Cue.IsNone())
	{
		USoundWave* SoundWave = nullptr;
		for (int i = 0; i < WorldManager->DialogCues.Num(); i++)
		{
			if (WorldManager->DialogCues[i]->GetName() == DialogRow->Cue.ToString())
			{
				SoundWave = WorldManager->DialogCues[i];
			}
		}
		if (!SoundWave)
		{
			UE_LOG(LogGame, Fatal, TEXT("No cue found %s"), *DialogRow->Cue.ToString());
		}
		if (DialogSpeaker == EDialogSpeaker::PC)
		{
			PC->CharacterVoice->SetSound(SoundWave);
			PC->CharacterVoice->Play();
		}
		else if (DialogSpeaker == EDialogSpeaker::NPC)
		{
			NPC->CharacterVoice->SetSound(SoundWave);
			NPC->CharacterVoice->Play();
		}
		else// is narrator mayhaps
		{
			UE_LOG(LogGame, Fatal, TEXT("Cue is set, but no speaker defined"));
		}
	}
	if (!NPC)		// If other is not a character then don\t check, it crashes if checks, my english broke
	{
		if (WaitTime > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(DialogTimerHandle, this, &ATWLDialogManager::DialogNext_Finish, WaitTime, false);	// FALSE was set as THIS for some reason, can't quite remember..
		}
		else
		{
			DialogNext_Finish();
		}
	}
	else if (WaitTime > 0.f && !PC->CharacterVoice->IsPlaying() && !NPC->CharacterVoice->IsPlaying())									// If time is not set or is zero, prevents getting stuck in dialog menu.
	{
		GetWorld()->GetTimerManager().SetTimer(DialogTimerHandle, this, &ATWLDialogManager::DialogNext_Finish, WaitTime, false);
	}
	else if (PC->CharacterVoice->IsPlaying() || NPC->CharacterVoice->IsPlaying())
	{
		DialogVoice();
	}
	else
	{
		DialogNext_Finish();
	}
}

void ATWLDialogManager::DialogNext_Finish()
{
	PlayerController->WidgetDialog->PlayAnimation(PlayerController->WidgetDialog->DialogClose);
	PlayerController->WidgetDialog->DialogVisibility = ESlateVisibility::Collapsed;
	GetWorld()->GetTimerManager().ClearTimer(DialogTimerHandle);
	if (DialogSpeaker == EDialogSpeaker::PC)
	{
		if (PC->CharacterVoice->IsPlaying())
		{
			PC->CharacterVoice->Stop();
		}
	}
	else if (DialogSpeaker == EDialogSpeaker::NPC)
	{
		if (NPC->CharacterVoice->IsPlaying())
		{
			NPC->CharacterVoice->Stop();
		}
	}
	if (bLastLine)
	{
		DialogEnd();
	}
	else
	{
		DialogStart();
	}
}

void ATWLDialogManager::DialogChoice()
{
	PlayerController->WidgetDialog->ChoicesVisibility = ESlateVisibility::Visible;
	PlayerController->WidgetDialog->PlayAnimation(PlayerController->WidgetDialog->ChoicesOpen);
	PlayerController->WidgetDialog->DialogManager = this;
	int i;
	FString ContextString;
	for (i = 0; i < DialogRow->Choices.Num(); i++)
	{
		FDialogData* ChoiceRow = DialogData->FindRow<FDialogData>(DialogRow->Choices[i], ContextString);
		if (!ChoiceRow->Locked)
		{
			PlayerController->WidgetDialog->AddDialogButton(ChoiceRow, DialogRow->Choices[i]);
		}
	}
	if (!PlayerController->WidgetDialog->ChoicesBox->HasAnyChildren())
	{
		UE_LOG(LogGame, Error, TEXT("DialogManager has ran out of choices and will exit dialog with %s!"), *NPCName);
		DialogEnd();
	}
}

void ATWLDialogManager::DialogVoice()
{
	if (PC->CharacterVoice->IsPlaying() || NPC->CharacterVoice->IsPlaying())
	{
		DialogVoice_Wait();
	}
	else
	{
		DialogVoice_Finish();
	}
}

void ATWLDialogManager::DialogVoice_Wait()
{
	FTimerHandle DialogVoiceTimer;
	GetWorld()->GetTimerManager().SetTimer(DialogVoiceTimer, this, &ATWLDialogManager::DialogVoice, 0.2f, false);
}

void ATWLDialogManager::DialogVoice_Finish()
{
	DialogNext_Finish();
}
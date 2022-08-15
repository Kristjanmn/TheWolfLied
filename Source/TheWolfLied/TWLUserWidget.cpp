// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLUserWidget.h"
#include "TWLPlayerController.h"
#include "TWLCharacter.h"
#include "GameFunctions.h"
#include "TWLWorldManager.h"
#include "TWLDialogManager.h"
#include "TWLQuestManager.h"
#include "TWLQuest.h"
#include "TWLItem.h"
#include "TWLGameInstance.h"
#include "GameVariables.h"

void UTWLUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UTWLGameInstance* GameInstance = Cast<UTWLGameInstance>(GetWorld()->GetGameInstance());
	PlayerController = Cast<ATWLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UTWLUserWidget::AddDialogButton(struct FDialogData* ButtonInfo, FName ChoiceRowName)
{
	DialogButton = CreateWidget<UTWLUserWidget>(PlayerController, PlayerController->DialogButtonClass);
	DialogButton->ChoiceText = ButtonInfo->Text;
	DialogButton->DialogWidgetRef = this;
	DialogButton->RowName = ChoiceRowName;
	ChoicesBox->AddChild(DialogButton);
}

void UTWLUserWidget::AddQuestButton(UTWLQuest* QuestRef)
{
	QuestButton = CreateWidget<UTWLUserWidget>(PlayerController, PlayerController->QuestButtonClass);
	QuestButton->QuestText = FText::AsCultureInvariant(QuestRef->Title);
	QuestButton->JournalWidgetRef = this;
	QuestButton->QuestManager = QuestManager;
	QuestButton->Quest = QuestRef;
	QuestsBox->AddChild(QuestButton);
	UE_LOG(LogGame, Log, TEXT("Added button for %s"), *QuestRef->Title);
}

void UTWLUserWidget::DialogChoicePressed(FName ChoiceRowName)
{
	if (!DialogManager)
	{
		UE_LOG(LogGame, Fatal, TEXT("DialogManager not set!"));
	}
	PlayAnimation(ChoicesClose);
	ChoicesBox->ClearChildren();
	ChoicesVisibility = ESlateVisibility::Collapsed;
	DialogManager->LineID = ChoiceRowName;
	DialogManager->DialogStart();
}

void UTWLUserWidget::JournalQuestPressed(UTWLQuest* QuestRef)
{
	if (!QuestManager)
	{
		UE_LOG(LogGame, Fatal, TEXT("QuestManager not set!"));
	}
	if (!QuestRef)
	{
		UE_LOG(LogGame, Fatal, TEXT("QuestRef not going thru!"));
	}
	QuestDescriptionText = QuestRef->Description;
}

void UTWLUserWidget::ResumeGame()
{
	PlayerController->SetPause(false);
	PlayerController->bShowMouseCursor = false;
	RemoveFromParent();
	FTimerHandle WidgetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &UTWLUserWidget::ResumeGame_Finish, 0.02f, false);
}

void UTWLUserWidget::ResumeGame_Finish()
{
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void UTWLUserWidget::LoadInventory()
{
	ATWLCharacter* Character = PlayerController->PlayerCharacter;
	InventoryGridPanel->ClearChildren();
	int MaxColumns = UGameVariables::GetInventoryColumns();
	int Row = 0;
	int Column = 0;
	int i;
	for (i = 0; i < Character->Inventory.Num(); i++)
	{
		InventoryButton = CreateWidget<UTWLUserWidget>(PlayerController, PlayerController->WidgetInventoryButtonClass);
		InventoryButton->ItemReference = Character->Inventory[i];
		UGridSlot* ItemSlot = InventoryGridPanel->AddChildToGrid(InventoryButton);
		ItemSlot->SetColumn(Column);
		ItemSlot->SetRow(Row);
		Column++;
		if (Column == MaxColumns)
		{
			Row++;
			Column = 0;
		}
		//UE_LOG(LogGame, Error, TEXT("Added item %s to inventory widget!"), *InventoryButton->ItemReference->ItemID.ToString());
	}
}

void UTWLUserWidget::InventoryItemPressed(ATWLItem* ItemRef)
{
	if(ItemRef) UE_LOG(LogGame, Error, TEXT("Item %s in inventory pressed"), *ItemRef->ItemID.ToString());
	ItemRef->Use(PlayerController->PlayerCharacter);
	PlayerController->WidgetInventory->LoadInventory();
}
// Made by Kristjanmn

#pragma once

#include "Blueprint/UserWidget.h"
#include "TWLUserWidget.generated.h"

class ATWLPlayerController;
class ATWLCharacter;
class UGameFunctions;
class ATWLWorldManager;
class ATWLDialogManager;
class ATWLQuestManager;
class UTWLQuest;
class ATWLItem;
class UGameVariables;

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None			UMETA(DisplayName = "None"),
	Interaction		UMETA(DisplayName = "Interaction"),
	Dialog			UMETA(DisplayName = "Dialog"),
	Inventory		UMETA(DisplayName = "Inventory")
};

UCLASS()
class THEWOLFLIED_API UTWLUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	ATWLPlayerController* PlayerController;
	UGameFunctions* GameFunctions;
	ATWLWorldManager* WorldManager;
	ATWLDialogManager* DialogManager;
	ATWLQuestManager* QuestManager;

	UTWLUserWidget* DialogButton;
	UTWLUserWidget* QuestButton;
	UTWLUserWidget* InventoryButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTWLUserWidget* DialogWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTWLUserWidget* JournalWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWidgetType WidgetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractionName = "Unknown";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UVerticalBox* ChoicesBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ChoiceColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UVerticalBox* QuestsBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestDescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor QuestColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility DialogVisibility = ESlateVisibility::Collapsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility ChoicesVisibility = ESlateVisibility::Collapsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTWLQuest* Quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGridPanel* InventoryGridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATWLItem* ItemReference;

	// Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UWidgetAnimation* DialogOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UWidgetAnimation* DialogClose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UWidgetAnimation* ChoicesOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UWidgetAnimation* ChoicesClose;

	void AddDialogButton(struct FDialogData* ButtonInfo, FName ChoiceRowName);
	void AddQuestButton(UTWLQuest* QuestRef);

	UFUNCTION(BlueprintCallable)
	void DialogChoicePressed(FName ChoiceRowName);

	UFUNCTION(BlueprintCallable)
	void JournalQuestPressed(UTWLQuest* QuestRef);

	UFUNCTION(BlueprintCallable)
	void ResumeGame();
	void ResumeGame_Finish();

	void LoadInventory();

	UFUNCTION(BlueprintCallable)
	void InventoryItemPressed(ATWLItem* ItemRef);

	// TODO: overrride NativeOnKeyDown to cut down blueprint usage.
};
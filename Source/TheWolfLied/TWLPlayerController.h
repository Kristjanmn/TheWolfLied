// Made by Kristjanmn

#pragma once

#include "GameFunctions.h"
#include "TWLUserWidget.h"
#include "TWLCharacter.h"
#include "GameFramework/PlayerController.h"
#include "TWLPlayerController.generated.h"

class ATWLWorldManager;
class ATWLDialogManager;
class ATWLQuestManager;
class UTWLQuest;

UCLASS()
class THEWOLFLIED_API ATWLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATWLPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void UpdateCharacter();

	void CreateGameMenu();

	UGameFunctions* GameFunctions;
	ATWLWorldManager* WorldManager;		// To make my life easier
	ATWLQuestManager* QuestManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ATWLCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraMinZoom = 100.f;
	float CameraMaxZoom = 500.f;
	float CameraZoomStepping = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UTWLUserWidget* WidgetHUD;
	UTWLUserWidget* WidgetDevHUD;
	UClass* WidgetHUDClass;
	UClass* WidgetDevHUDClass;
	UTWLUserWidget* WidgetGameMenu;
	UClass* WidgetGameMenuClass;
	UTWLUserWidget* WidgetDialog;
	UClass* WidgetDialogClass;
	UClass* DialogButtonClass;
	UTWLUserWidget* WidgetStats;
	UClass* WidgetStatsClass;
	UTWLUserWidget* WidgetInventory;
	UTWLUserWidget* WidgetInventoryButton;
	UClass* WidgetInventoryClass;
	UClass* WidgetInventoryButtonClass;
	UTWLUserWidget* WidgetSkills;
	UClass* WidgetSkillsClass;
	UTWLUserWidget* WidgetJournal;
	UClass* WidgetJournalClass;
	UClass* QuestButtonClass;

	bool FPCamera = false;

	// Input functions

	// Key
	UFUNCTION(BlueprintCallable)
	void EscapePressed();
	void CameraZoomIn();
	void CameraZoomOut();
	void ChangeCamera();
	void SkipDialog();

	// Action
	void Action();
	void Sneak();
	void MoveFaster();
	void MoveSlower();
	void Jump();
	void Inventory();
	void Stats();
	void Skills();
	void Journal();

	// Axis
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
};
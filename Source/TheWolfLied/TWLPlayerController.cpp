// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLPlayerController.h"
#include "TWLWorldManager.h"
#include "TWLDialogManager.h"
#include "TWLQuestManager.h"
#include "TWLQuest.h"

ATWLPlayerController::ATWLPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetHUDObj(TEXT("/Game/UI/W_HUD"));
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetDevHUDObj(TEXT("/Game/UI/W_DevHUD"));
	if (WidgetHUDObj.Succeeded())
	{
		WidgetHUDClass = WidgetHUDObj.Class;
		WidgetDevHUDClass = WidgetDevHUDObj.Class;
	}

	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetGameMenuObj(TEXT("/Game/UI/W_GameMenu"));
	if (WidgetGameMenuObj.Succeeded())
	{
		WidgetGameMenuClass = WidgetGameMenuObj.Class;
	}
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetDialogObj(TEXT("/Game/UI/W_Dialog"));
	WidgetDialogClass = WidgetDialogObj.Class;
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> DialogButtonObj(TEXT("/Game/UI/W_DialogButton"));
	DialogButtonClass = DialogButtonObj.Class;
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetStatsObj(TEXT("/Game/UI/W_Stats"));
	WidgetStatsClass = WidgetStatsObj.Class;
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetInventoryObj(TEXT("/Game/UI/W_Inventory"));
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetInventoryButtonObj(TEXT("/Game/UI/W_InventoryButton"));
	WidgetInventoryClass = WidgetInventoryObj.Class;
	WidgetInventoryButtonClass = WidgetInventoryButtonObj.Class;
	//static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetSkillsObj(TEXT("/Game/UI/W_Skills"));
	//WidgetSkillsClass = WidgetSkillsObj.Class;
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> WidgetJournalObj(TEXT("/Game/UI/W_Journal"));
	WidgetJournalClass = WidgetJournalObj.Class;
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> QuestButtonObj(TEXT("/Game/UI/W_QuestButton"));
	QuestButtonClass = QuestButtonObj.Class;
}

void ATWLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UpdateCharacter();

	WidgetHUD = CreateWidget<UTWLUserWidget>(this, WidgetHUDClass);
	WidgetHUD->AddToViewport();
	//WidgetDevHUD = CreateWidget<UTWLUserWidget>(this, WidgetDevHUDClass);
	//WidgetDevHUD->AddToViewport();
	//PlayerCharacter->Name = FString(TEXT("Hero"));
}

void ATWLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Key binds
	this->InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ATWLPlayerController::EscapePressed);
	this->InputComponent->BindKey(EKeys::MouseScrollUp, IE_Pressed, this, &ATWLPlayerController::CameraZoomIn);
	this->InputComponent->BindKey(EKeys::MouseScrollDown, IE_Pressed, this, &ATWLPlayerController::CameraZoomOut);
	this->InputComponent->BindKey(EKeys::F, IE_Pressed, this, &ATWLPlayerController::ChangeCamera);
	this->InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ATWLPlayerController::SkipDialog);

	// Action binds
	this->InputComponent->BindAction("Action", IE_Pressed, this, &ATWLPlayerController::Action);
	this->InputComponent->BindAction("Sneak", IE_Pressed, this, &ATWLPlayerController::Sneak);
	this->InputComponent->BindAction("MoveFaster", IE_Pressed, this, &ATWLPlayerController::MoveFaster);
	this->InputComponent->BindAction("MoveSlower", IE_Pressed, this, &ATWLPlayerController::MoveSlower);
	this->InputComponent->BindAction("Jump", IE_Pressed, this, &ATWLPlayerController::Jump);
	this->InputComponent->BindAction("Inventory", IE_Pressed, this, &ATWLPlayerController::Inventory);
	this->InputComponent->BindAction("Stats", IE_Pressed, this, &ATWLPlayerController::Stats);
	this->InputComponent->BindAction("Skills", IE_Pressed, this, &ATWLPlayerController::Skills); //
	this->InputComponent->BindAction("Journal", IE_Pressed, this, &ATWLPlayerController::Journal);

	// Axis binds
	this->InputComponent->BindAxis("MoveForward", this, &ATWLPlayerController::MoveForward);
	this->InputComponent->BindAxis("MoveRight", this, &ATWLPlayerController::MoveRight);
	this->InputComponent->BindAxis("Turn", this, &ATWLPlayerController::Turn);
	this->InputComponent->BindAxis("LookUp", this, &ATWLPlayerController::LookUp);
}

// Update character
void ATWLPlayerController::UpdateCharacter()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->bIsPlayer = false;
		PlayerCharacter->PlayerController = nullptr;
	}
	PlayerCharacter = Cast<ATWLCharacter>(this->GetCharacter());
	PlayerCharacter->PreparePlayerComponents();
	this->SetViewTarget(PlayerCharacter);
	PlayerCharacter->PlayerController = this;
	PlayerCharacter->bIsPlayer = true;
}

void ATWLPlayerController::CreateGameMenu()
{
	this->Pause();
	this->bShowMouseCursor = true;
	WidgetGameMenu = CreateWidget<UTWLUserWidget>(this, WidgetGameMenuClass);
	WidgetGameMenu->AddToViewport();
	WidgetGameMenu->PlayerController = this;
	this->SetInputMode(FInputModeUIOnly());
}

// Input functions

// Key
void ATWLPlayerController::EscapePressed()
{
	if (PlayerCharacter)
	{
		if (PlayerCharacter->bBusy)
		{
			SkipDialog();
		}
		else
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATWLPlayerController::CreateGameMenu, 0.02f, false);
		}
	}
	else
	{
		// In case there is no PlayerCharacter set.
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATWLPlayerController::CreateGameMenu, 0.02f, false);
	}
}

// Camera functions
void ATWLPlayerController::CameraZoomIn()
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		if ((PlayerCharacter->CameraArm->TargetArmLength - CameraZoomStepping) >= CameraMinZoom)
		{
			PlayerCharacter->CameraArm->TargetArmLength = (PlayerCharacter->CameraArm->TargetArmLength - CameraZoomStepping);
		}
	}
}

void ATWLPlayerController::CameraZoomOut()
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		if ((PlayerCharacter->CameraArm->TargetArmLength + CameraZoomStepping) <= CameraMaxZoom)
		{
			PlayerCharacter->CameraArm->TargetArmLength = (PlayerCharacter->CameraArm->TargetArmLength + CameraZoomStepping);
		}
	}
}

void ATWLPlayerController::ChangeCamera()
{
	if (FPCamera)
	{
		FPCamera = false;
		PlayerCharacter->TPCamera->Activate();
		PlayerCharacter->FPCamera->Deactivate();
	}
	else
	{
		FPCamera = true;
		PlayerCharacter->FPCamera->Activate();
		PlayerCharacter->TPCamera->Deactivate();
	}
}

//Skipping dialogue sometimes crashes the game.
void ATWLPlayerController::SkipDialog()
{
	if (WorldManager->DialogManager)
	{
		WorldManager->DialogManager->PC->CharacterVoice->Stop();
		WorldManager->DialogManager->NPC->CharacterVoice->Stop();
		if (GetWorld()->GetTimerManager().IsTimerActive(WorldManager->DialogManager->DialogTimerHandle))
		{
			WorldManager->DialogManager->DialogNext_Finish();
		}
	}
}

// Action
void ATWLPlayerController::Action()
{
	if (PlayerCharacter->bCombatMode)
	{
		// attack
		ATWLCharacter* Target = Cast<ATWLCharacter>(PlayerCharacter->AC_Interact->HighlightedInteraction);
		if (Target)
		{
			Target->RecieveDamage(100.f);
		}
	}
	else if (!PlayerCharacter->bBusy)
	{
		PlayerCharacter->AC_Interact->AttemptInteraction();
	}
}

void ATWLPlayerController::Sneak()
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		if (PlayerCharacter->MovementType == ECharacterMovement::Sneak)
		{
			PlayerCharacter->SetMovement(PlayerCharacter->LastMovement);
		}
		else
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Sneak);
		}
	}
}

void ATWLPlayerController::MoveFaster()
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		if (PlayerCharacter->MovementType == ECharacterMovement::Walk)
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Jog);
		}
		else if (PlayerCharacter->MovementType == ECharacterMovement::Jog)
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Run);
		}
		else if (PlayerCharacter->MovementType == ECharacterMovement::Run)
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Sprint);
		}
	}
}

void ATWLPlayerController::MoveSlower()
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		if (PlayerCharacter->MovementType == ECharacterMovement::Sprint)
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Run);
		}
		else if (PlayerCharacter->MovementType == ECharacterMovement::Run)
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Jog);
		}
		else if (PlayerCharacter->MovementType == ECharacterMovement::Jog)
		{
			PlayerCharacter->SetMovement(ECharacterMovement::Walk);
		}
	}
}

void ATWLPlayerController::Jump()
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		PlayerCharacter->Jump();
	}
	/*// spawn a fucking orange instead		use this code to drop an item mayhaps
	FTransform Transform;
	Transform.SetLocation(PlayerCharacter->GetActorLocation() + (PlayerCharacter->GetActorForwardVector() * 250.f));
	ATWLItem* ActorToSpawn = GetWorld()->SpawnActorDeferred<ATWLItem>(ATWLItem::StaticClass(), Transform);
	ActorToSpawn->ItemID = "Orange_Giant";
	ActorToSpawn->Name = "A giant fucking orange";
	ActorToSpawn->UpdateName();
	ActorToSpawn->ItemCategory = EItemCategory::Food;
	ActorToSpawn->PotionType = EPotionType::LP;
	ActorToSpawn->UseValue = 10;
	ActorToSpawn->Mesh->SetRelativeScale3D(FVector(5, 5, 5));
	ActorToSpawn->FinishSpawning(Transform);*/
}

void ATWLPlayerController::Inventory()
{
	if (WidgetInventory)
	{
		bShowMouseCursor = false;
		PlayerCharacter->bBusy = false;
		WidgetInventory->RemoveFromViewport();
		WidgetInventory = nullptr;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		WidgetInventory = CreateWidget<UTWLUserWidget>(this, WidgetInventoryClass);
		WidgetInventory->AddToViewport();
		WidgetInventory->LoadInventory();
		PlayerCharacter->bBusy = true;
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		//	Make sure widgets wont overlap and stuff, might need better way in future. Same for Stats widget, maybe use Tabs for UI in general? Hida anything that is not needed at that time while keeping it ready and running.
		if (WidgetJournal)
		{
			WidgetJournal->RemoveFromViewport();
			WidgetJournal = nullptr;
		}
	}
}

void ATWLPlayerController::Stats()
{
	if (WidgetStats)
	{
		WidgetStats->RemoveFromViewport();
		WidgetStats = nullptr;
		PlayerCharacter->TeleportTo(FVector(0, 0, 800), FRotator(0, 0, 0));	// Remove later!
	}
	else
	{
		WidgetStats = CreateWidget<UTWLUserWidget>(this, WidgetStatsClass);
		WidgetStats->AddToViewport();
	}
}

void ATWLPlayerController::Skills()
{
	if (WidgetSkills)
	{
		WidgetSkills->RemoveFromViewport();
		WidgetSkills = nullptr;
	}
	else
	{
		WidgetSkills = CreateWidget<UTWLUserWidget>(this, WidgetSkillsClass);
		WidgetSkills->AddToViewport();
	}
}

void ATWLPlayerController::Journal()
{
	if (WidgetJournal)
	{
		bShowMouseCursor = false;
		PlayerCharacter->bBusy = false;
		WidgetJournal->RemoveFromParent();
		WidgetJournal->RemoveFromViewport();
		WidgetJournal = nullptr;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		bShowMouseCursor = true;
		PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
		PlayerCharacter->bBusy = true;
		WidgetJournal = CreateWidget<UTWLUserWidget>(this, WidgetJournalClass);
		WidgetJournal->PlayerController = this;
		WidgetJournal->QuestManager = QuestManager;
		WidgetJournal->AddToViewport();
		int i;
		for (i = 0; i < QuestManager->Quests.Num(); i++)
		{
			if (QuestManager->Quests[i]->State != EQuestState::UnAccepted)
			{
				WidgetJournal->AddQuestButton(QuestManager->Quests[i]);
			}
		}
		SetInputMode(FInputModeGameAndUI());
		//	Make sure widgets wont overlap and stuff, might need better way in future. Same for Stats widget, maybe use Tabs for UI in general? Hida anything that is not needed at that time while keeping it ready and running.
		if (WidgetInventory)
		{
			WidgetInventory->RemoveFromViewport();
			WidgetInventory = nullptr;
		}
	}
}

// Axis
void ATWLPlayerController::MoveForward(float AxisValue)
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), AxisValue);
	}
}

void ATWLPlayerController::MoveRight(float AxisValue)
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), AxisValue);
	}
}

void ATWLPlayerController::Turn(float AxisValue)
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		PlayerCharacter->AddControllerYawInput(AxisValue);
	}
}

void ATWLPlayerController::LookUp(float AxisValue)
{
	if (PlayerCharacter && !PlayerCharacter->bBusy)
	{
		PlayerCharacter->AddControllerPitchInput(AxisValue);
	}
}
// Made by Kristjanmn

#include "TheWolfLied.h"
#include "AC_Interact.h"
#include "TWLItem.h"
#include "TWLCharacter.h"
#include "GameFunctions.h"
#include "TWLPlayerController.h"
#include "TWLUserWidget.h"
#include "TWLWorldManager.h"
#include "TWLDialogManager.h"
#include "TWLScriptManager.h"

// Sets default values for this component's properties
UAC_Interact::UAC_Interact(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;	// No longer needed
	PrimaryComponentTick.bCanEverTick = true;

	InteractionTrigger = CreateDefaultSubobject<USphereComponent>("InteractionTrigger");
	InteractionTrigger->SetSphereRadius(TriggerRadius);
	InteractionTrigger->SetMobility(EComponentMobility::Movable);
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &UAC_Interact::TriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &UAC_Interact::TriggerEndOverlap);

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>("InteractionWidget");
	static ConstructorHelpers::FClassFinder<UTWLUserWidget> InteractionWidgetObj(TEXT("/Game/UI/W_InteractionUI"));
	if (InteractionWidgetObj.Succeeded())
	{
		InteractionWidgetClass = InteractionWidgetObj.Class;
	}
	InteractionWidget->SetWidgetClass(InteractionWidgetClass);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetVisibility(false);
	//InteractionWidget->AttachToComponent(InteractionTrigger, FAttachmentTransformRules::SnapToTargetNotIncludingScale);	// Results in handle error
	InteractionWidget->SetupAttachment(InteractionTrigger);
	InteractionWidget->SetDrawSize(FVector2D(200, 250));
	bCanInteract = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DialogTable(TEXT("/Game/Data/Dialog/PassTimeDialog"));
	PassTimeDialog = DialogTable.Object;
}

// Called when the game starts
void UAC_Interact::BeginPlay()
{
	Super::BeginPlay();
	if (!bCanInteract)
	{
		InteractionTrigger->DestroyComponent();
		//PrimaryComponentTick.TickInterval = 3.f;	tick once every x seconds
	}
	// Have parent call for it instead
	//UpdateName();		// possibly temporary for reasons ;; UE4.21 access violation, widget does not yet exist, delay or call from parent
}

// Called every frame
void UAC_Interact::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	UpdateCharacter(DeltaTime);
	if (bCanInteract)
	{
		InteractionTrigger->SetWorldLocation(this->GetOwner()->GetActorLocation());
	}
	else
	{
		InteractionWidget->SetWorldLocation(this->GetOwner()->GetActorLocation());
	}
}

void UAC_Interact::UpdateCharacter(float DeltaTime)
{
	CheckInteraction();
}

// Check for interaction
void UAC_Interact::CheckInteraction()
{
	if (AvailableInteractions.Num() > 0)
	{
		if (OwningCharacter->bCombatMode)
		{
			GetBestTarget();
		}
		else
		{
			GetBestInteraction();
		}
	}
	else
	{
		SetCurrentInteraction(nullptr);
		HighlightedInteraction = nullptr;
	}
}

// Get best interaction
void UAC_Interact::GetBestInteraction()
{
	TArray<float> ActorDots;
	int MaxFloatIndex = 0;
	float MaxFloat = 0;
	FVector CameraForward = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetActorForwardVector();
	FVector CharacterLocation = OwningCharacter->GetActorLocation();
	for (int i = 0; i < AvailableInteractions.Num(); i++)
	{
		AActor* AvailableInteraction = AvailableInteractions[i];
		FVector InteractionLocation = AvailableInteraction->GetActorLocation();
		float DotProd = FVector::DotProduct((InteractionLocation - CharacterLocation).GetSafeNormal(), CameraForward);
		ActorDots.Add(DotProd);
	}
	for (int i = 0; i < ActorDots.Num(); i++)
	{
		float Value = ActorDots[i];
		if (Value > MaxFloat)
		{
			MaxFloat = Value;
			MaxFloatIndex = i;
		}
	}
	if (MaxFloat > InteractionDotThreshold)
	{
		SetCurrentInteraction(AvailableInteractions[MaxFloatIndex]);
	}
	else
	{
		SetCurrentInteraction(nullptr);
		HighlightedInteraction = nullptr;
	}
}

void UAC_Interact::GetBestTarget()
{
	TArray<float> ActorDots;
	int MaxFloatIndex = 0;
	float MaxFloat = 0;
	FVector CharacterLocation = OwningCharacter->GetActorLocation();
	FVector CameraForward = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetActorForwardVector();
	for (int i = 0; i < AvailableInteractions.Num(); i++)
	{
		AActor* AvailableInteraction = AvailableInteractions[i];
		//ATWLCharacter* Target = Cast<ATWLCharacter>(AvailableInteraction);
		//if ((AvailableInteraction->IsA(ATWLCharacter::StaticClass())) && (Target->CharacterState != ECharacterState::Dead))
		FVector InteractionLocation = AvailableInteraction->GetActorLocation();
		float DotProd = FVector::DotProduct((InteractionLocation - CharacterLocation).GetSafeNormal(), CameraForward);
		ActorDots.Add(DotProd);
	}
	for (int i = 0; i < ActorDots.Num(); i++)
	{
		float Value = ActorDots[i];
		AActor* AvailableInteraction = AvailableInteractions[i];
		ATWLCharacter* Target = Cast<ATWLCharacter>(AvailableInteraction);
		if ((Value > MaxFloat) && (AvailableInteraction->IsA(ATWLCharacter::StaticClass())) && (Target->CharacterState != ECharacterState::Dead))
		{
			MaxFloat = Value;
			MaxFloatIndex = i;
		}
	}
	if ((MaxFloat > InteractionDotThreshold) && (ActorDots.Num() > 0))
	{
		SetCurrentInteraction(AvailableInteractions[MaxFloatIndex]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *AvailableInteractions[MaxFloatIndex]->GetName()));
	}
	else
	{
		SetCurrentInteraction(nullptr);
		HighlightedInteraction = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No target"));
	}
}

// Try to interact
void UAC_Interact::AttemptInteraction()
{
	if (HighlightedInteraction)
	{
		Interact();
	}
}

// Interact
void UAC_Interact::Interact()
{
	EInteractionType InteractableType = HighlightedInteraction->FindComponentByClass<UAC_Interact>()->InteractionType;
	EInteractionUseType InteractableUseType = HighlightedInteraction->FindComponentByClass<UAC_Interact>()->InteractionUseType;
	Item = Cast<ATWLItem>(HighlightedInteraction);
	class ATWLPlayerController* PlayerController = OwningCharacter->PlayerController;
	ATWLCharacter* HighlightedCharacter = Cast<ATWLCharacter>(HighlightedInteraction);
	WorldManager = PlayerController->WorldManager;
	switch (InteractableType)
	{
	case EInteractionType::None:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interaction type not set!"));
		break;
	case EInteractionType::Pickup:
		OwningCharacter->Inventory.Add(Item);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::Printf(TEXT("%i %s, %s"), OwningCharacter->Inventory.Num(), *Item->Name, *Item->ItemID.ToString()));
		HighlightedInteraction->Destroy();
		UE_LOG(LogGame, Log, TEXT("%s picked up %s"), *OwningCharacter->Name, *Item->ItemID.ToString());
		break;
	case EInteractionType::Talk:
		WorldManager->SpawnDialogManager();
		DialogManager = WorldManager->DialogManager;
		DialogManager->PC = OwningCharacter;				// This might not always be player
		DialogManager->NPC = HighlightedCharacter;
		DialogManager->DialogInit(HighlightedCharacter->DialogData, OwningCharacter->Name, HighlightedCharacter->Name);
		break;
	case EInteractionType::Use:
		int ReqHave = 0;	// Change name to something more sane
		int FoundIndex;
		TArray<TSubclassOf<ATWLItem>> HighlightedUseRequirements = HighlightedInteraction->FindComponentByClass<UAC_Interact>()->UseRequirements;
		TArray<TSubclassOf<ATWLItem>> InventoryClasses;
		for (int i = 0; i < OwningCharacter->Inventory.Num(); i++)
		{
			TSubclassOf<ATWLItem> InventoryItem = OwningCharacter->Inventory[i]->GetClass();	// Occasional access violation error //when interracting with another AC component?
			InventoryClasses.Add(InventoryItem);
		}
		for (int i = 0; i < HighlightedUseRequirements.Num(); i++)	// Loop thru all required items and find them in players inventory
		{
			FoundIndex = InventoryClasses.Find(HighlightedUseRequirements[i]);
			if (FoundIndex >= 0)
			{
				ReqHave++;
			}
		}
		if (ReqHave == HighlightedUseRequirements.Num())	// Make sure player has the required items
		{
			switch (InteractableUseType)
			{
			case EInteractionUseType::None:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interaction use type not set!"));
				UE_LOG(LogGame, Error, TEXT("%s has no interaction type set!"), *HighlightedInteraction->FindComponentByClass<UAC_Interact>()->Name);
				break;
			case EInteractionUseType::WorldUse:
				// if item use check if have item, if with ui add proper ui, else play proper animation and call function to target
				break;
			case EInteractionUseType::ItemUse:

				break;
			case EInteractionUseType::Storage:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No storage interface!"));
				// Take everything
				if (HighlightedCharacter)
				{
					for (int i = 0; i < HighlightedCharacter->Inventory.Num(); i++)
					{
						WorldManager->ScriptManager->GiveItem(HighlightedCharacter->CharacterID, OwningCharacter->CharacterID, HighlightedCharacter->Inventory[i]->ItemID);		//occasional A.V.
						//sometimes the world manager is not there, resulting in Access Violation
					}
				}
				break;
			case EInteractionUseType::PassTime:
				WorldManager->SpawnDialogManager();
				DialogManager = WorldManager->DialogManager;
				DialogManager->DialogInit(PassTimeDialog, OwningCharacter->Name, "Bed");
				break;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You do not have the required items!"));
		}
		break;
	}
}

void UAC_Interact::AddInteraction(AActor* AddActor)
{
	AvailableInteractions.AddUnique(AddActor);
}

void UAC_Interact::RemoveInteraction(AActor* RemoveActor)
{
	AvailableInteractions.Remove(RemoveActor);
}

// Set current interaction
void UAC_Interact::SetCurrentInteraction(AActor* Interaction)
{
	UWidgetComponent* Widget;
	if ((Interaction != nullptr) && (Interaction->GetOwner() != GetWorld()->GetFirstPlayerController()->GetCharacter()) && (bPlayerControlled))	// Should work, but only in single player
	{
		if (Interaction != HighlightedInteraction)
		{
			if (HighlightedInteraction != nullptr)
			{
				bool Highlight = false;
				Widget = HighlightedInteraction->FindComponentByClass<UWidgetComponent>();
				Widget->SetVisibility(Highlight);
			}
			HighlightedInteraction = Interaction;
			bool Highlight = true;
			Widget = HighlightedInteraction->FindComponentByClass<UWidgetComponent>();
			Widget->SetVisibility(Highlight);
		}
		else
		{
			bool Highlight = true;
			Widget = HighlightedInteraction->FindComponentByClass<UWidgetComponent>();
			Widget->SetVisibility(Highlight);
		}
	}
	else
	{
		if (HighlightedInteraction != nullptr)
		{
			bool Highlight = false;
			Widget = HighlightedInteraction->FindComponentByClass<UWidgetComponent>();
			Widget->SetVisibility(Highlight);
		}
		else
		{
			HighlightedInteraction = nullptr;
		}
	}
}

// Interaction trigger overlap events
void UAC_Interact::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetComponentsByClass(UAC_Interact::StaticClass()).Num() > 0)
	{
		if (OtherActor != this->GetOwner())
		{
			AddInteraction(OtherActor);
		}
	}
}

void UAC_Interact::TriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetComponentsByClass(UAC_Interact::StaticClass()).Num() > 0)
	{
		RemoveInteraction(OtherActor);
	}
}

void UAC_Interact::UpdateName()
{
	UTWLUserWidget* Widget = Cast<UTWLUserWidget>(InteractionWidget->GetUserWidgetObject());
	Widget->InteractionName = Name;
}
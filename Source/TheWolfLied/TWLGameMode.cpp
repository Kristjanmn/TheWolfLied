// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLGameMode.h"
#include "TWLWorldManager.h"
#include "TWLCharacter.h"
#include "TWLItem.h"
#include "TWLGameInstance.h"

ATWLGameMode::ATWLGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerCharacterObj(TEXT("Blueprint'/Game/Game/Player/testPlayer.testPlayer'"));// DemoPlayer.DemoPlayer'"));//testplayer.testplayer'"));
	DefaultPawnClass = (UClass*)PlayerCharacterObj.Object->GeneratedClass;//*/ATWLCharacter::StaticClass();
	PlayerControllerClass = ATWLPlayerController::StaticClass();
}

void ATWLGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ATWLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UTWLGameInstance* GameInstance = Cast<UTWLGameInstance>(GetGameInstance());
	GameInstance->Loading();
	FTimerHandle GameModeTimer;
	GetWorld()->GetTimerManager().SetTimer(GameModeTimer, this, &ATWLGameMode::UpdateGameMode, 0.02f, false);
}

void ATWLGameMode::UpdateGameMode()
{
	WorldManager = PlayerController->WorldManager;
}

void ATWLGameMode::SetTimeOfDay(int Hours, int Minutes)
{
	if (WorldManager)
	{
		WorldManager->SetTimeOfDay(Hours, Minutes);
	}
}

void ATWLGameMode::GiveItem(ATWLCharacter* To, ATWLCharacter* From, ATWLItem* Item)
{
	To->Inventory.Add(Item);
	From->Inventory.Remove(Item);
}
// Made by Kristjanmn

#include "TheWolfLied.h"
#include "GameFunctions.h"
#include "TWLGameInstance.h"
#include "TWLWorldManager.h"

void UGameFunctions::ChangePlayerController(APlayerController* OldController, APlayerController* NewController)
{
	APlayerController* OldPC = OldController;
	APlayerController* NewPC = NewController;

	//void AGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC);	Need to create custom game mode for this!
}

void UGameFunctions::FloatInRange(float ValueIn, float Min, float Max, float &ValueOut)
{
	float Value = ValueIn;
	float Minimum = Min;
	float Maximum = Max;
	if (Value > Maximum)
	{
		ValueOut = Maximum;
	}
	else if (Value < Minimum)
	{
		ValueOut = Minimum;
	}
	else
	{
		ValueOut = Value;
	}
}

bool UGameFunctions::IsIntegerInRange(int ValueIn, int Min, int Max)
{
	int Value = ValueIn;
	int Minimum = Min;
	int Maximum = Max;
	if ((Value >= Minimum) && (Value <= Maximum))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UGameFunctions::DelayTimer(UWorld* World, float Seconds)
{
	//int Milliseconds = (Seconds * 1000);
	//std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
	float EndTime = (World->TimeSeconds + Seconds);

	while (EndTime > World->TimeSeconds)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Waiting")));
	}
}

ATWLWorldManager* UGameFunctions::GetWorldManager()
{
	UTWLGameInstance* GameInstance = Cast<UTWLGameInstance>(GetWorld()->GetGameInstance());
	ATWLWorldManager* WorldManager = GameInstance->WorldManager;
	return WorldManager;
}
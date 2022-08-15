// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLLifeManager.h"
#include "TWLWorldManager.h"
#include "TWLScriptManager.h"
#include "TWLCharacter.h"


// Sets default values
ATWLLifeManager::ATWLLifeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATWLLifeManager::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogLifeManager, Log, TEXT("LifeManager BeginPlay finished"));
}

// Called every frame
void ATWLLifeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATWLCharacter* ATWLLifeManager::GetCharacterByID(FName CharacterID)
{
	ATWLCharacter* Character = nullptr;
	int i;
	for (i = 0; i < Characters.Num(); i++)
	{
		if (Characters[i]->CharacterID == CharacterID)
		{
			Character = Characters[i];
		}
	}
	if (Character)
	{
		return Character;
	}
	else
	{
		UE_LOG(LogLifeManager, Error, TEXT("Could not GetCharacterByID %s"), *CharacterID.ToString());
		return nullptr;
	}
}

void ATWLLifeManager::AddCharacter(ATWLCharacter* Character)
{
	if (Character)
	{
		bool CharacterIDExists = false;
		int index = INDEX_NONE;
		if (Characters.Num() > 0)
		{
			int i;
			for (i = 0; i < Characters.Num(); i++)
			{
				if (Characters[i]->CharacterID == Character->CharacterID)
				{
					CharacterIDExists = true;
				}
			}
			index = Characters.Find(Character);
		}
		if (CharacterIDExists)				// Performance heavy when spawning many but works.
		{
			FString NewCharacterID;
			int CharacterIDExtension;
			for (CharacterIDExtension = 0; CharacterIDExists; CharacterIDExtension++)
			{
				NewCharacterID = *Character->CharacterID.ToString();
				NewCharacterID.Append("_");
				NewCharacterID.AppendInt(CharacterIDExtension);
				int i;
				bool CharacterIDAvailable = true;
				for (i = 0; i < Characters.Num(); i++)
				{
					if (*Characters[i]->CharacterID.ToString() == NewCharacterID)
					{
						CharacterIDAvailable = false;
					}
				}
				if (CharacterIDAvailable)
				{
					CharacterIDExists = false;
					Character->CharacterID = FName(*NewCharacterID);
				}
				index = Characters.Find(Character);
			}
		}
		if ((index == INDEX_NONE) && (!CharacterIDExists))
		{
			Characters.Add(Character);
			UE_LOG(LogLifeManager, Log, TEXT("Added character %s to Characters array!"), *Character->CharacterID.ToString());
		}
	}
	else
	{
		UE_LOG(LogLifeManager, Fatal, TEXT("No valid character reference!"));
	}
}

void ATWLLifeManager::RemoveCharacter(ATWLCharacter* Character)
{
	if (Character)
	{
		int index = Characters.Find(Character);
		if (index == INDEX_NONE)
		{
			UE_LOG(LogLifeManager, Error, TEXT("Character %s not in the list!"), *Character->CharacterID.ToString());
		}
		else
		{
			Characters.Remove(Character);
			UE_LOG(LogLifeManager, Log, TEXT("Removed character %s from Characters array!"), *Character->CharacterID.ToString());
		}
	}
	else
	{
		UE_LOG(LogLifeManager, Fatal, TEXT("No valid character reference!"));
	}
}
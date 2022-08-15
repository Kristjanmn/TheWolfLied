// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLItem.h"
#include "TWLCharacter.h"
#include "TWLWorldManager.h"

ATWLItem::ATWLItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AC_Interact = ObjectInitializer.CreateDefaultSubobject<UAC_Interact>(this, "AC_Interact");
	AC_Interact->InteractionType = EInteractionType::Pickup;
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/Meshes/Consumables/Food/OrangeSmall"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	Mesh->SetSimulatePhysics(true);
}

void ATWLItem::BeginPlay()
{
	Super::BeginPlay();
	UpdateName();
}

void ATWLItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATWLItem::UpdateName()
{
	AC_Interact->Name = Name;
	AC_Interact->UpdateName();
}

void ATWLItem::Use(ATWLCharacter* User)
{
	if (!User)
	{
		UE_LOG(LogGame, Fatal, TEXT("Item %s can't get User!"), *ItemID.ToString());
	}

	UE_LOG(LogGame, Error, TEXT("%s used %s"), *User->CharacterID.ToString(), *ItemID.ToString());
	Owner = User;

	// Temporary variables
	float Health = 0;
	float Energy = 0;
	float Stamina = 0;

	switch (ItemCategory)
	{
	case EItemCategory::None:
		UE_LOG(LogGame, Error, TEXT("Item %s category set to None"), *ItemID.ToString());
		break;
	case EItemCategory::Weapon:
		if ((User->Strength >= RequiredStrength) && (User->Dexterity >= RequiredDexterity))
		{
			// Equip weapon
			UE_LOG(LogGame, Error, TEXT("Equiped weapon %s"), *ItemID.ToString());
		}
		else
		{
			FString lackingCue;
			if (User->Strength < RequiredStrength)
			{
				// "I'm not strong enough"
				UE_LOG(LogGame, Error, TEXT("I'm not strong enough"));
				lackingCue = "PC_BrothelLocation";
			}
			else if (User->Dexterity < RequiredDexterity)
			{
				// "I'm not dexterous enough"
				UE_LOG(LogGame, Error, TEXT("I'm not dexterous enough"));
				lackingCue = "NPC_BrothelLocation";
			}
			USoundWave* SoundWave = nullptr;
			for (int i = 0; i < User->AC_Interact->WorldManager->DialogCues.Num(); i++)
			{
				if (User->AC_Interact->WorldManager->DialogCues[i]->GetName() == lackingCue)
				{
					SoundWave = User->AC_Interact->WorldManager->DialogCues[i];
				}
			}
			User->CharacterVoice->SetSound(SoundWave);
			/*if(User->bIsPlayer)*/ User->CharacterVoice->Play();	//only play sound if player uses
		}
		break;
	case EItemCategory::Armor:
		UE_LOG(LogGame, Error, TEXT("Equiped armor %s"), *ItemID.ToString());
		break;
	case EItemCategory::Food:
		UE_LOG(LogGame, Error, TEXT("Ate food %s"), *ItemID.ToString());
		Remove();
		break;
	case EItemCategory::Potion:
		switch (PotionType)
		{
		case EPotionType::Health:
			Health = (User->Health + UseValue);
			if (Health > User->MaxHealth)
			{
				Health = User->MaxHealth;
			}
			User->Health = Health;
			Remove();
			break;
		case EPotionType::MaxHealth:
			User->MaxHealth = (User->MaxHealth + UseValue);
			Remove();
			break;
		case EPotionType::Energy:
			Energy = (User->Energy + UseValue);
			if (Energy > User->MaxEnergy)
			{
				Energy = User->MaxEnergy;
			}
			User->Energy = Energy;
			Remove();
			break;
		case EPotionType::MaxEnergy:
			User->MaxEnergy = (User->MaxEnergy + UseValue);
			Remove();
			break;
		case EPotionType::Stamina:
			Stamina = (User->Stamina + UseValue);
			if (Stamina > User->MaxStamina)
			{
				Stamina = User->MaxStamina;
			}
			User->Stamina = Stamina;
			Remove();
			break;
		case EPotionType::MaxStamina:
			User->MaxStamina = (User->MaxStamina + UseValue);
			Remove();
			break;
		case EPotionType::Experience:
			User->RecieveExperience(UseValue);
			break;
		case EPotionType::LP:
			User->LearningPoints = (User->LearningPoints + UseValue);
			Remove();
			break;
		case EPotionType::Strength:
			User->Strength = (User->Strength + UseValue);
			Remove();
			break;
		case EPotionType::Dexterity:
			User->Dexterity = (User->Dexterity + UseValue);
			Remove();
			break;
		}
		break;
	case EItemCategory::Magic:
		switch (MagicType)
		{
		case EMagicType::Scroll:
			break;
		case EMagicType::Rune:
			break;
		}
		break;
	case EItemCategory::Docs:
		// Display on screen, probably need a few more variables for this one, such as text and/or image to be shown.
		break;
	case EItemCategory::Other:
		break;
	}
}

void ATWLItem::Remove()
{
	if (Owner)
	{
		Owner->Inventory.Remove(this);
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Item %s has no owner!"), *ItemID.ToString());
	}
	this->Destroy();
}
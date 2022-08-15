// Made by Kristjanmn

#pragma once

#include "GameFramework/Actor.h"
#include "AC_Interact.h"
#include "TWLItem.generated.h"

class ATWLCharacter;

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None		UMETA(DisplayName = "None"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Armor		UMETA(DisplayName = "Armor"),
	Food		UMETA(DisplayName = "Food"),
	Potion		UMETA(DisplayName = "Potion"),
	Magic		UMETA(DisplayName = "Magic"),
	Docs		UMETA(DisplayName = "Docs"),
	Other		UMETA(DisplayName = "Other")
};

// Only used when ItemCategory is Weapon
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	OneHAnded	UMETA(DisplayName = "One-Handed"),
	TwoHanded	UMETA(DisplayName = "Two-Handed"),
	Bow			UMETA(DisplayName = "Bow"),
	Crossbow	UMETA(DisplayName = "Crossbow")
};

// Only used when ItemCategory is Armor
UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Cloth		UMETA(DisplayName = "Cloth"),
	Leather		UMETA(DisplayName = "Leather"),
	Mail		UMETA(DisplayName = "Mail"),
	Plate		UMETA(DisplayName = "Plate")
};

// Only used when ItemCategory is Potion
UENUM(BlueprintType)
enum class EPotionType : uint8
{
	Health		UMETA(DisplayName = "Health"),
	MaxHealth	UMETA(DisplayName = "MaxHealth"),
	Energy		UMETA(DisplayName = "Energy"),
	MaxEnergy	UMETA(DisplayName = "MaxEnergy"),
	Stamina		UMETA(DisplayName = "Stamina"),
	MaxStamina	UMETA(DisplayName = "MaxStamina"),
	Experience	UMETA(DisplayName = "Experience"),
	LP			UMETA(DisplayName = "LearningPoints"),
	Strength	UMETA(DisplayName = "Strength"),
	Dexterity	UMETA(DisplayName = "Dexterity")
};

// Only used when ItemCategory is Magic
UENUM(BlueprintType)
enum class EMagicType : uint8
{
	Scroll		UMETA(DisplayName = "Scroll"),
	Rune		UMETA(DisplayName = "Rune")
};

UCLASS()
class THEWOLFLIED_API ATWLItem : public AActor
{
	GENERATED_BODY()

public:
	ATWLItem(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_Interact* AC_Interact;

	// Should be named properly for easy to find during runtime.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemImage;			// some default image or something

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArmorType ArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPotionType PotionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicType MagicType;

	// Owning character or such where it's stored.
	ATWLCharacter* Owner;		// !!!Maybe move it to AC_Interact for advanced functionality

	// Ammount of health or such the item gives when consumed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UseValue;

	// Market value of this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemValue;

	// Requirements
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredDexterity;

	void UpdateName();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Use(ATWLCharacter* User);

	// After using one use item
	void Remove();
};
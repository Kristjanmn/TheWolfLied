// Made by Kristjanmn

#pragma once

#include "GameFramework/Character.h"
#include "AC_Interact.h"
#include "GameVariables.h"
#include "TWLItem.h"
#include "TWLCharacter.generated.h"

class UGameFunctions;
class ATWLWorldManager;
class ATWLLifeManager;			// Might rename it to LifeManager or something later as it's for managing NPCs.
class ATWLPlayerController;
class UTWLGameInstance;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Sleep		UMETA(DisplayName = "Sleep"),
	Downed		UMETA(DisplayName = "Downed"),
	Dead		UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ECharacterMovement : uint8
{
	Walk		UMETA(DisplayName = "Walk"),
	Jog			UMETA(DisplayName = "Jog"),
	Run			UMETA(DisplayName = "Run"),
	Sprint		UMETA(DisplayName = "Sprint"),
	Sneak		UMETA(DisplayName = "Sneak")
};

UCLASS()
class THEWOLFLIED_API ATWLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATWLCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Properties
	UGameFunctions* GameFunctions;
	ATWLPlayerController* PlayerController;

	// Actor Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|ActorComponent")
	UAC_Interact* AC_Interact;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|ActorComponent")	// Don't need to show these 3 in blueprint as they are set automaticaly while playing.
	USpringArmComponent* CameraArm;

	// Third person camera
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|ActorComponent")
	UCameraComponent* TPCamera;

	// First person camera
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|ActorComponent")
	UCameraComponent* FPCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Voice")
	UAudioComponent* CharacterVoice;

	// Character
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FName CharacterID;			// Character's ID for scripting, abaility to call to character by ID from WorldManager or something and edit variables.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString Name = "None";		// Character's name displayed ingame
	bool bIsPlayer = false;		// For scripting and future possible coop/multiplayer

	// Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Status")
	ECharacterState CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Status")
	ECharacterMovement MovementType;	// CharacterMovement is already defined in Character.h
	ECharacterMovement LastMovement;	// Used when toggleing between sneak

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Dialog")
	UDataTable* DialogData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Status")
	bool bBusy = false;
	bool bCombatMode = false;			// probably temporary, rather check what player has in hand or if is casting any spells
	bool bJumping = false;

	// Arrays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Item")
	TArray<ATWLItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Item")
	TArray<TSubclassOf<ATWLItem>> Skinnables;

	// Start inventory to be made on spawn, this is temporary until I find out how to reference assets from asset browser and dynamicaly make editable instance of the item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Item")
	TArray<TSubclassOf<ATWLItem>> DefaultInventory;

	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float MinExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float MaxExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float Health = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float MaxHealth = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float Energy = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float MaxEnergy = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float Stamina = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats")
	float MaxStamina = 10;

	// Stats Regen
	float StaminaMult = 1;
	float EnergyMult = 1;

	// Leveling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Leveling")
	float ExperiencePerLevel = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Leveling")
	float HealthPerLevel = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Leveling")
	int LearningPointsPerLevel = 10;

	// Skills
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Skills")
	int LearningPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Skills")
	int Strength = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Skills")
	int Dexterity = 10;

	// Weapon Skills
	int OneHandedSkill = 0;
	int TwoHandedSkill = 0;
	int BowSkill = 0;
	int CrossbowSkill = 0;

	// Movement Skills
	int Sneak = 0;
	int Accrobat = 0;

	// Magic Skills
	int MagicCircle = 0;

	// Hunting Skills
	bool SkinAnimal = false;
	bool SkinReptile = false;
	bool SkinBeast = false;
	bool ExtractTeeth = false;
	bool ExtractClaws = false;
	bool ExtractHorn = false;
	bool TakeMeat = false;
	bool TakeTrophy = false;

	// Requirements
	float StaminaToJump = 5;

	// Functions
	void BeginPlayFailsafe();
	void BeginPlayFailsafeLoop();

	void UpdateName();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void PreparePlayerComponents();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void DestroyPlayerComponents();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void LevelUp();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void RecieveExperience(float XP);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void RecieveDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetMovement(ECharacterMovement Movement);

	void Jump();

	void CreateInventory();
};
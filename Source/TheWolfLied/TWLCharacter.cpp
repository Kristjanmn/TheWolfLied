// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLCharacter.h"
#include "GameFunctions.h"
#include "TWLWorldManager.h"
#include "TWLLifeManager.h"
#include "TWLPlayerController.h"
#include "TWLGameInstance.h"

// Sets default values
ATWLCharacter::ATWLCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Game/Meshes/Characters/Human/Base_Rig/Human_Base_Rig"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Game/Animations/Human/ABP_Human_Male"));
	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->AddLocalOffset(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetAnimInstanceClass(AnimBlueprint.Object->GeneratedClass);
	//SetRootComponent(GetMesh());

	AC_Interact = ObjectInitializer.CreateDefaultSubobject<UAC_Interact>(this, "AC_Interact");
	AC_Interact->bCanInteract = true;
	AC_Interact->OwningCharacter = this;
	AC_Interact->InteractionType = EInteractionType::Talk;

	CharacterVoice = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, "Voice");
	CharacterVoice->SetRelativeLocation(FVector(0, 0, 70));

	CharacterMovement = this->GetCharacterMovement();
	CharacterMovement->AirControl = 0.f;
	CharacterMovement->bOrientRotationToMovement = true;

	if (MaxExperience <= 0)
	{
		MaxExperience = ExperiencePerLevel;
	}

	MovementType = ECharacterMovement::Jog;
	CharacterMovement->MaxWalkSpeed = 250.f;

	static ConstructorHelpers::FObjectFinder<UDataTable> DialogTable(TEXT("/Game/Data/Dialog/DefaultDialog"));
	DialogData = DialogTable.Object;
}

// Called when the game starts or when spawned
void ATWLCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateName();
	CreateInventory();

	if (CharacterID == "None")
	{
		UE_LOG(LogGame, Warning, TEXT("CharacterID is not valid! This could lead to scripting errors! Name: %s"), *Name);
	}

	BeginPlayFailsafe();
}

// Called every frame
void ATWLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Stamina regen
	if (Stamina >= MaxStamina)
	{
		Stamina = MaxStamina;
	}
	else
	{
		Stamina = (Stamina + ((DeltaTime * (Energy / 10.f)) * StaminaMult));
	}

	// Energy regen
	if (Energy >= MaxEnergy)
	{
		Energy = MaxEnergy;
	}
	else
	{
		Energy = (Energy + ((DeltaTime * 1.f) * EnergyMult));
	}

	// Jump
	if (CharacterMovement->IsMovingOnGround())
	{
		bJumping = false;
	}

	// Camera reset
	if (bIsPlayer)
	{
		//GetControlRotation();
	}
}

// Called to bind functionality to input
void ATWLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATWLCharacter::BeginPlayFailsafe()
{
	UTWLGameInstance* GameInstance = Cast<UTWLGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance->WorldManager && GameInstance->WorldManager->LifeManager)
	{
		GameInstance->WorldManager->LifeManager->AddCharacter(this);
	}
	else
	{
		BeginPlayFailsafeLoop();
	}
}

void ATWLCharacter::BeginPlayFailsafeLoop()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATWLCharacter::BeginPlayFailsafe, 0.2f, false);
}

void ATWLCharacter::UpdateName()
{
	AC_Interact->Name = Name;
	AC_Interact->UpdateName();
}

void ATWLCharacter::PreparePlayerComponents()
{
	CameraArm = NewObject<USpringArmComponent>(this, USpringArmComponent::StaticClass(), "CameraArm");
	TPCamera = NewObject<UCameraComponent>(this, UCameraComponent::StaticClass(), "ThirdPersonCamera");
	FPCamera = NewObject<UCameraComponent>(this, UCameraComponent::StaticClass(), "FirstPersonCamera");

	CameraArm->RegisterComponent();
	TPCamera->RegisterComponent();
	FPCamera->RegisterComponent();

	/*if (PlayerController->FPCamera)	crash
	{
		FPCamera->Activate();
		TPCamera->Deactivate();
	}
	else
	{
		TPCamera->Activate();
		FPCamera->Deactivate();
	}*/

	CameraArm->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TPCamera->AttachToComponent(CameraArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	FPCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	FPCamera->SetRelativeLocationAndRotation(FVector(0, 0, 170), FRotator(0, 90, 0));
	FPCamera->bUsePawnControlRotation = true;
	CameraArm->SetRelativeLocationAndRotation(FVector(0, 0, 140), FRotator(0, 90, 0));
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bDoCollisionTest = true;
	//CameraArm->bEnableCameraLag = true;	// Maybe fetch this from user config or something
	/*CameraArm->bInheritPitch = true;
	CameraArm->bInheritRoll = true;
	CameraArm->bInheritYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;*/
	AC_Interact->bPlayerControlled = true;
}

void ATWLCharacter::DestroyPlayerComponents()
{
	AC_Interact->bPlayerControlled = false;
	TPCamera->Deactivate();
	FPCamera->Deactivate();
	TPCamera->UnregisterComponent();
	FPCamera->UnregisterComponent();
	CameraArm->UnregisterComponent();

	TPCamera->DestroyComponent();
	FPCamera->DestroyComponent();
	CameraArm->DestroyComponent();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player components destroyed"));
}

void ATWLCharacter::LevelUp()
{
	if (Experience >= MaxExperience)
	{
		MinExperience = MaxExperience;
		MaxExperience = (MaxExperience + ((Level + 1) * ExperiencePerLevel) + ExperiencePerLevel);
		Health = Health + HealthPerLevel;
		MaxHealth = MaxHealth + HealthPerLevel;
		LearningPoints = LearningPoints + LearningPointsPerLevel;
		// Play level up sound
		UE_LOG(LogGame, Log, TEXT("%s leveled up"), *Name);
		LevelUp();
	}
}

void ATWLCharacter::RecieveExperience(float XP)
{
	Experience = (Experience + XP);
	if (Experience >= MaxExperience)
	{
		LevelUp();
	}
}

void ATWLCharacter::RecieveDamage(float Damage)
{
	Health = (Health - Damage);
	if (Health <= 0)
	{
		Health = 0;
		Die();
	}
}

void ATWLCharacter::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	this->GetController()->UnPossess();
	CharacterState = ECharacterState::Dead;
	AC_Interact->InteractionType = EInteractionType::Use;
	AC_Interact->InteractionUseType = EInteractionUseType::Storage;
}

void ATWLCharacter::SetMovement(ECharacterMovement Movement)
{
	LastMovement = MovementType;
	MovementType = Movement;
	// Access violation crash
	//FTransform Transform;
	//Transform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 250.f));
	//ATWLCharacter* ActorToSpawn = GetWorld()->SpawnActorDeferred<ATWLCharacter>(ATWLCharacter::StaticClass(), Transform);
	//ActorToSpawn->Name = "Diego";
	//ActorToSpawn->CharacterID = "Diego";
	//ActorToSpawn->UpdateName();
	//ActorToSpawn->FinishSpawning(Transform);
	bCombatMode = true;
	AC_Interact->InteractionTrigger->SetSphereRadius(1500.f);
	switch (MovementType)
	{
	case ECharacterMovement::Walk:
		CharacterMovement->MaxWalkSpeed = 120.f;
		break;
	case ECharacterMovement::Jog:
		CharacterMovement->MaxWalkSpeed = 250.f;
		break;
	case ECharacterMovement::Run:
		CharacterMovement->MaxWalkSpeed = 370.f;
		break;
	case ECharacterMovement::Sprint:
		CharacterMovement->MaxWalkSpeed = 500.f;
		break;
	case ECharacterMovement::Sneak:
		CharacterMovement->MaxWalkSpeed = 50.f;
		break;
	}
}

void ATWLCharacter::Jump()
{
	if ((Stamina >= StaminaToJump) && (CharacterMovement->IsMovingOnGround()))
	{
		//Stamina = Stamina - StaminaToJump; disabled for now
		CharacterMovement->DoJump(false);
		bJumping = true;
		bCombatMode = false;
	}
}

void ATWLCharacter::CreateInventory()	// Currently only way to pick items in editor and make it into inventory.
{
	for (int i = 0; i < DefaultInventory.Num(); i++)
	{
		ATWLItem* NewItem = NewObject<ATWLItem>(this, DefaultInventory[i], FName(*DefaultInventory[i]->GetDefaultObject<ATWLItem>()->Name));
		if (NewItem->ItemID == "None")
		{
			UE_LOG(LogGame, Warning, TEXT("%s: Item %s has no ItemID assigned!"), *Name, *NewItem->Name);
		}
		NewItem->Owner = this;			// Does not work
		Inventory.Add(NewItem);
	}
	DefaultInventory.Empty();
}
// Made by Kristjanmn

#pragma once

#include "TWLUserWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "AC_Interact.generated.h"

class ATWLCharacter;
class ATWLItem;
class UGameFunctions;
class ATWLWorldManager;
class ATWLDialogManager;

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None		UMETA(DisplayName = "None"),
	Talk		UMETA(DisplayName = "Talk"),
	Pickup		UMETA(DisplayName = "PickUp"),
	Use			UMETA(DisplayName = "Use")
};

UENUM(BlueprintType)		// Pretty fucking not done me thinks
enum class EInteractionUseType : uint8
{
	None		UMETA(DisplauName = "None"),
	WorldUse	UMETA(DisplayName = "WorldUse"),	// Chair, bench, things like that without UI
	ItemUse		UMETA(DisplayName = "ItemUse"),		// Use with or without UI
	Storage		UMETA(DisplayName = "Storage"),		// Use to store and take items with UI
	PassTime	UMETA(DisplayName = "PassTime")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEWOLFLIED_API UAC_Interact : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Interact(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Variables
	TArray<AActor*> AvailableInteractions;
	AActor* HighlightedInteraction;
	float InteractionDotThreshold = 0.4f;//0.5;//0.85;	// Perfect?
	FString Name;
	ATWLCharacter* OwningCharacter;
	ATWLItem* Item;
	UGameFunctions* GameFunctions;
	ATWLWorldManager* WorldManager;
	ATWLDialogManager* DialogManager;
	UDataTable* PassTimeDialog;

	bool bPlayerControlled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInteractionType InteractionType = EInteractionType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInteractionUseType InteractionUseType = EInteractionUseType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<TSubclassOf<ATWLItem>> UseRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = false;

	USphereComponent* InteractionTrigger;
	float TriggerRadius = 500.f;

	UWidgetComponent* InteractionWidget;
	UClass* InteractionWidgetClass;

	// Update character
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UpdateCharacter(float DeltaTime);

	// Check for interaction
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void CheckInteraction();

	// Get best interaction
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void GetBestInteraction();

	virtual void GetBestTarget();

	// Try to interact
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void AttemptInteraction();

	// Interact
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void AddInteraction(AActor* AddActor);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void RemoveInteraction(AActor* RemoveActor);

	// Set current interaction
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void SetCurrentInteraction(AActor* Interaction);

	// Interaction trigger overlap events
	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateName();
};
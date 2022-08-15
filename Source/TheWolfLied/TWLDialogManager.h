// Made by Kristjanmn

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TWLDialogManager.generated.h"

class ATWLWorldManager;
class ATWLQuestManager;
class ATWLScriptManager;
class UTWLUserWidget;
class ATWLPlayerController;
class ATWLCharacter;

UENUM(BlueprintType)
enum class EDialogType : uint8
{
	End		UMETA(DisplayName = "End"),
	Next	UMETA(DisplayName = "Next"),
	Choice	UMETA(DisplayName = "Choice")
};

UENUM(BlueprintType)
enum class EDialogSpeaker : uint8
{
	None	UMETA(DisplayName = "None"),
	PC		UMETA(DisplayName = "Player"),
	NPC		UMETA(DisplayName = "NPC")
};

USTRUCT(BlueprintType)
struct FDialogData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FDialogData()
		: Type()
		, NextLine()
		, Choices()
		, Speaker()
		, Cue()
		, WaitTime()
		, Text()
		, Script()
		, Locked()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDialogType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Choices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDialogSpeaker Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Script;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Locked;
};

UCLASS()
class THEWOLFLIED_API ATWLDialogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWLDialogManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ATWLWorldManager* WorldManager;
	ATWLQuestManager* QuestManager;
	ATWLScriptManager* ScriptManager;
	ATWLPlayerController* PlayerController;
	FTimerHandle DialogTimerHandle;
	UDataTable* DialogData;
	ATWLCharacter* PC;
	ATWLCharacter* NPC;
	FString NPCName;
	FString PCName;
	FDialogData* DialogRow;
	EDialogType DialogType;
	EDialogSpeaker DialogSpeaker;
	FName LineID = FName(TEXT("INIT"));
	bool bLastLine = false;
	float WaitTime = 0.f;

	void DialogInit(UDataTable* InDialogData, FString InPCName, FString InNPCName);
	void DialogStart();
	void DialogEnd();
	void DialogNext();
	void DialogNext_Finish();		// Called after timer in DialogNext
	void DialogChoice();
	void DialogVoice();
	void DialogVoice_Wait();
	void DialogVoice_Finish();
};
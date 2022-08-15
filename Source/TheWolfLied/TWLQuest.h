// Made by Kristjanmn

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TWLQuest.generated.h"

class ATWLQuestManager;
class ATWLScriptManager;

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainQuest		UMETA(DisplayName = "MainQuest"),
	SideQuest		UMETA(DisplayName = "SideQuest")
};

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	UnAccepted		UMETA(DisplayName = "UnAccepted"),
	Accepted		UMETA(DisplayName = "Accepted"),
	Completed		UMETA(DisplayName = "Completed"),
	Failed			UMETA(DisplayName = "Failed")
};

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FQuestData()
		: Description()
		, Script()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Script;
};

UCLASS(Blueprintable)	// Made it visible in editor
class THEWOLFLIED_API UTWLQuest : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EQuestType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EQuestState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName Phase = "INIT";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UDataTable* Phases;

	FQuestData* QuestRow;

	ATWLQuestManager* QuestManager;
	ATWLScriptManager* ScriptManager;

	void SetState(EQuestState NewState);
	void SetPhase(FName PhaseName);
};
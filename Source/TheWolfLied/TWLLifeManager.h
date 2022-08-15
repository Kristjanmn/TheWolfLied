// Made by Kristjanmn

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TWLLifeManager.generated.h"

class ATWLWorldManager;
class ATWLScriptManager;
class ATWLCharacter;

UCLASS()
class THEWOLFLIED_API ATWLLifeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWLLifeManager();

	ATWLWorldManager* WorldManager;
	TArray<ATWLCharacter*> Characters;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ATWLCharacter* GetCharacterByID(FName CharacterID);

	void AddCharacter(ATWLCharacter* Character);

	void RemoveCharacter(ATWLCharacter* Character);
};
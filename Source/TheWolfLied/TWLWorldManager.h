// Made by Kristjanmn

#pragma once

#include "GameFramework/Actor.h"
#include "GameVariables.h"
#include "TWLGameInstance.h"
#include "TWLWorldManager.generated.h"

class ATWLLifeManager;
class ATWLDialogManager;
class ATWLQuestManager;
class ATWLScriptManager;
class ATWLWeatherManager;
class ATWLPlayerController;
class ATWLCharacter;

UCLASS()
class THEWOLFLIED_API ATWLWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWLWorldManager(const FObjectInitializer& ObjectInitializer);

	ATWLPlayerController* PlayerController;

	FTimeline TimeCurveTimeline;

	FTimeline SunCurveTimeline;

	FTimeline SunCurvePitchTimeline;

	FTimeline SunCurveYawTimeline;

	float DELTATIME;

	float TimeMultiplier;

	float RawTime;

	float SunPitchRotation;

	float SunYawRotation;

	UTWLGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATWLLifeManager* LifeManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATWLDialogManager* DialogManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATWLQuestManager* QuestManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATWLScriptManager* ScriptManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATWLWeatherManager* WeatherManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundWave*> DialogCues;		// This probably takes up a lot of memory on runtime in the end, but right now I have no idea where else to put it, DialogManager is out of question for obvious reasons.

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void TickDayNightTimeline();

	UFUNCTION()
	void UpdateDayNightTime(float Value);

	UFUNCTION()
	void UpdateSunPitchRotation(float Value);

	UFUNCTION()
	void UpdateSunYawRotation(float Value);

	UFUNCTION(BlueprintCallable, Category = "World")
	void GetSunRotation(float &pitch, float &yaw);

	UFUNCTION()
	float GetTimeMultiplier();

	UFUNCTION(BlueprintCallable, Category = "World")
	void SetTimeOfDay(int Hours, int Minutes);

	UFUNCTION(BlueprintCallable, Category = "World")
	void SetSunRotation(ADirectionalLight* Sun);

	void SpawnDialogManager();

	void LoadDialogCues();

	void GetTime(int &Hours, int &Minutes);
};
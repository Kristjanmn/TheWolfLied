// Made by Kristjanmn

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TWLWeatherManager.generated.h"

class ATWLWorldManager;

USTRUCT(BlueprintType)
struct FWeatherData
{
	GENERATED_USTRUCT_BODY()

public:
	FWeatherData()
		: Name()
		, Probability()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Probability;
};


UCLASS()
class THEWOLFLIED_API ATWLWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWLWeatherManager(const FObjectInitializer& ObjectInitializer);

	ATWLWorldManager* WorldManager;
	bool bInitiated = false;
	TArray<FWeatherData> WeatherConditions;

	// Weather Actors
	//Weather			// Rain, snow, clouds emmitters all inside this actor, feed variables to it's emmitters to change amount of particles.
	//Fog

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();
	void SetWeather(FName WeatherName);
};
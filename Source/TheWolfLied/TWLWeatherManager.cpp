// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLWeatherManager.h"
#include "TWLWorldManager.h"

// Sets default values
ATWLWeatherManager::ATWLWeatherManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATWLWeatherManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATWLWeatherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATWLWeatherManager::Init()
{
	UE_LOG(LogWeatherManager, Log, TEXT("Beginning initialization"));
	if (!WorldManager) {
		UE_LOG(LogWeatherManager, Fatal, TEXT("WeatherManager initialization failed! Could not reach WorldManager!"));
	}
}

void ATWLWeatherManager::SetWeather(FName WeatherName)
{
	if (WeatherName.IsNone()) {
		UE_LOG(LogWeatherManager, Fatal, TEXT("Trying to set weather without name given"));
	}
	FWeatherData newWeather;
	int i;
	for (i = 0; i < WeatherConditions.Num(); i++) {
		if (WeatherConditions[i].Name == WeatherName) {
			newWeather = WeatherConditions[i];
		}
	}
	if (newWeather.Name.IsNone()) {
		UE_LOG(LogWeatherManager, Fatal, TEXT("Could not set weather to %s"), *WeatherName.ToString());
	}
}
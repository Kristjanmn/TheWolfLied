// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLWorldManager.h"
#include "TWLLifeManager.h"
#include "TWLDialogManager.h"
#include "TWLQuestManager.h"
#include "TWLScriptManager.h"
#include "TWLWeatherManager.h"
#include "TWLPlayerController.h"
#include "TWLCharacter.h"

// Sets default values
ATWLWorldManager::ATWLWorldManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UCurveFloat> TimeCurve(TEXT("CurveFloat'/Game/Game/TimeCurve.TimeCurve'"));
	const ConstructorHelpers::FObjectFinder<UCurveFloat> SunPitchCurve(TEXT("CurveFloat'/Game/Game/SunPitchCurve.SunPitchCurve'"));
	const ConstructorHelpers::FObjectFinder<UCurveFloat> SunYawCurve(TEXT("CurveFloat'/Game/Game/SunYawCurve.SunYawCurve'"));
	TimeCurveTimeline = FTimeline{};
	FOnTimelineFloat InterptTimeFunction{};
	InterptTimeFunction.BindUFunction(this, "UpdateDayNightTime");
	FOnTimelineFloat InterptSunPitchFunction{};
	InterptSunPitchFunction.BindUFunction(this, "UpdateSunPitchRotation");
	FOnTimelineFloat InterptSunYawFunction{};
	InterptSunYawFunction.BindUFunction(this, "UpdateSunYawRotation");
	TimeCurveTimeline.AddInterpFloat(TimeCurve.Object, InterptTimeFunction);
	//DayNightTimelineMultiplier = GetDayNightTimelineMultiplier(); Results in crash EXCEPTION_ACCESS_VIOLATION
	SunCurvePitchTimeline.AddInterpFloat(SunPitchCurve.Object, InterptSunPitchFunction);
	SunCurveYawTimeline.AddInterpFloat(SunYawCurve.Object, InterptSunYawFunction);
}

// Called when the game starts or when spawned
void ATWLWorldManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ATWLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(PlayerController)	// Check in case of spectator mode, else crashes with access violation
	{
		PlayerController->WorldManager = this;
	}

	FTransform Transform;
	Transform.SetLocation(FVector(0, 0, 0));
	ATWLLifeManager* LifeSpawn = GetWorld()->SpawnActorDeferred<ATWLLifeManager>(ATWLLifeManager::StaticClass(), Transform);
	LifeManager = LifeSpawn;
	LifeSpawn->FinishSpawning(Transform);
	ATWLQuestManager* QuestSpawn = GetWorld()->SpawnActorDeferred<ATWLQuestManager>(ATWLQuestManager::StaticClass(), Transform);
	QuestManager = QuestSpawn;
	QuestSpawn->FinishSpawning(Transform);
	ATWLScriptManager* ScriptSpawn = GetWorld()->SpawnActorDeferred<ATWLScriptManager>(ATWLScriptManager::StaticClass(), Transform);
	ScriptManager = ScriptSpawn;
	ScriptSpawn->FinishSpawning(Transform);
	ATWLWeatherManager* WeatherSpawn = GetWorld()->SpawnActorDeferred<ATWLWeatherManager>(ATWLWeatherManager::StaticClass(), Transform);
	WeatherManager = WeatherSpawn;
	WeatherSpawn->FinishSpawning(Transform);

	LifeManager->WorldManager = this;
	QuestManager->WorldManager = this;
	QuestManager->DialogManager = DialogManager;
	QuestManager->ScriptManager = ScriptManager;
	ScriptManager->WorldManager = this;
	ScriptManager->LifeManager = LifeManager;
	ScriptManager->QuestManager = QuestManager;
	ScriptManager->WeatherManager = WeatherManager;
	ScriptManager->PlayerController = PlayerController;
	WeatherManager->WorldManager = this;
	PlayerController->WorldManager = this;
	PlayerController->QuestManager = QuestManager;

	GameInstance = Cast<UTWLGameInstance>(GetGameInstance());
	GameInstance->WorldManager = this;
	TimeMultiplier = GetTimeMultiplier();
	TimeCurveTimeline.SetPlayRate(1 / TimeMultiplier);
	TimeCurveTimeline.SetLooping(true);
	TimeCurveTimeline.PlayFromStart();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATWLWorldManager::TickDayNightTimeline, DELTATIME, true, 0.0f);
	SetTimeOfDay(8, 0);
	if (GameInstance->bInMainMenu)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), "MainMenu", true, true, LatentInfo);
		SetTimeOfDay(3, 0);
	}
	LoadDialogCues();
	UE_LOG(LogWorldManager, Log, TEXT("WorldManager BeginPlay finished"));
	WeatherManager->Init();
}

// Called every frame
void ATWLWorldManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	DELTATIME = DeltaTime;
	TickDayNightTimeline();
}

/** The function which gets called from the timeline tick */
void ATWLWorldManager::UpdateDayNightTime(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Timeline: timeline: %f  value:%f"), SunCurveTimeline.GetPlaybackPosition(), Value));
	RawTime = TimeCurveTimeline.GetPlaybackPosition();
	GameInstance->TimeOfDayHours = floor(RawTime * GameInstance->HoursInDay);
	GameInstance->TimeOfDayMinutes = floor(((RawTime * GameInstance->HoursInDay) - floor(RawTime * GameInstance->HoursInDay)) * GameInstance->MinutesInHour);
	// do something
	SunCurvePitchTimeline.SetPlaybackPosition(RawTime, true, true);
	SunCurveYawTimeline.SetPlaybackPosition(RawTime, true, true);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Sun: timeline: %f  value: %f; Time: timeline: %f"), SunCurveTimeline.GetPlaybackPosition(), SunRotation, TimeCurveTimeline.GetPlaybackPosition()));
}

void ATWLWorldManager::UpdateSunPitchRotation(float Value)
{
	SunPitchRotation = Value;
}

void ATWLWorldManager::UpdateSunYawRotation(float Value)
{
	SunYawRotation = Value;
}

void ATWLWorldManager::GetSunRotation(float &pitch, float &yaw)
{
	pitch = SunPitchRotation;
	yaw = SunYawRotation;
}

/** Function which gets called from the Timer to call EffectProgress */
void ATWLWorldManager::TickDayNightTimeline()
{
	if (TimeCurveTimeline.IsPlaying())
	{
		TimeCurveTimeline.TickTimeline(DELTATIME);// / DayNightTimelineMultiplier);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Playing"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("(%f)"), SunRotation));
	}
	else
	{
		//GetWorldTimerManager().ClearTimer(this, &ATWLWorldManager::TickDayNightTimeline);
		//DayNightTimeline.PlayFromStart();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not playing"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Timeline stopped!")));
		SetLifeSpan(0);
	}
}

float ATWLWorldManager::GetTimeMultiplier()
{
	float TimelineLength = TimeCurveTimeline.GetTimelineLength();// 1
	int SecondsInMinute = GameInstance->SecondsInMinute;// 60
	int MinutesInHour = GameInstance->MinutesInHour;// 60
	int HoursInDay = GameInstance->HoursInDay;// 24
	int MinutesPerDayNight = UGameVariables::GetMinutesPerDayNight();// 90
	int X = HoursInDay * MinutesInHour;// 24 * 60 = 1440
	int Y = X / MinutesPerDayNight;// 1440 / 90 = 16
	int Z = ((MinutesInHour * SecondsInMinute) / Y) * HoursInDay;// ((60 * 60) = 3600 / 16) = 225 * 24 = 5400
	return Z;
}

void ATWLWorldManager::SetTimeOfDay(int Hours, int Minutes)
{
	float Hour = Hours;
	float Minute = Minutes;
	float NewTime = ((Hour + (Minute / GameInstance->MinutesInHour)) / GameInstance->HoursInDay);// ((x + (y / 60)) / 24) = ((12 + 0.33) / 24) = 0.51
	TimeCurveTimeline.SetPlaybackPosition(NewTime, true, true);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Time set to %i:%i (%f)"), Hours, Minutes, NewTime));
}

void ATWLWorldManager::SetSunRotation(ADirectionalLight* Sun)
{
	FRotator OldRot = Sun->GetActorRotation();
	float x = OldRot.Roll;
	float y = OldRot.Pitch;
	float z = OldRot.Yaw;
	if (OldRot.Roll > SunPitchRotation)
	{
		x = (OldRot.Roll - SunPitchRotation);
	}
	else
	{
		x = (SunPitchRotation - OldRot.Roll);
	}
	FRotator NewRot = OldRot.Add(0, 0, 0);
	Sun->SetActorRotation(NewRot);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("(%f)"), x));
}

void ATWLWorldManager::SpawnDialogManager()
{
	FTransform Transform;
	Transform.SetLocation(FVector(0, 0, 0));
	ATWLDialogManager* DialogSpawn = GetWorld()->SpawnActorDeferred<ATWLDialogManager>(ATWLDialogManager::StaticClass(), Transform);
	DialogManager = DialogSpawn;
	DialogSpawn->FinishSpawning(Transform);
	DialogManager->WorldManager = this;
	DialogManager->QuestManager = QuestManager;
	DialogManager->ScriptManager = ScriptManager;
	DialogManager->PlayerController = PlayerController;
	ScriptManager->DialogManager = DialogManager;
}

/** Load all dialog cues for DialogManager to use in dialog. */
void ATWLWorldManager::LoadDialogCues()		// Make this part of initial startup
{
	DialogCues.Empty();
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(USoundWave::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath("/Game/Audio/Dialog");
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);
	for (int i = 0; i < AssetDatas.Num(); i++)
	{
		DialogCues.Add(Cast<USoundWave>(AssetDatas[i].GetAsset()));
	}
}

void ATWLWorldManager::GetTime(int &Hours, int &Minutes)
{
	Hours = GameInstance->TimeOfDayHours;
	Minutes = GameInstance->TimeOfDayMinutes;
}
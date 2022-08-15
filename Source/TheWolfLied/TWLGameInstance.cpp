// Made by Kristjanmn

#include "TheWolfLied.h"
#include "TWLGameInstance.h"
#include "TWLWorldManager.h"
#include "TWLPlayerController.h"
#include "TWLQuest.h"

UTWLGameInstance::UTWLGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UTWLGameInstance::Init()
{
	UGameInstance::Init();

	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UTWLGameInstance::BeginLoadingScreen);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UTWLGameInstance::EndLoadingScreen);
}

void UTWLGameInstance::Loading()
{
	GetWorld()->GetWorldSettings()->GlobalGravityZ = 0.f;
	GetWorld()->GetWorldSettings()->bGlobalGravitySet = true;
	TArray<ULevel*> Levels = GetWorld()->GetLevels();
	TArray<FSubLevelStatus> SubLevelState = GetSubLevelsStatus(GetWorld());
	int LevelsLoaded = 0;
	int i;
	for (i = 0; i < SubLevelState.Num(); i++)
	{
		if ((SubLevelState[i].StreamingStatus == EStreamingStatus::LEVEL_Loaded) || (SubLevelState[i].StreamingStatus == EStreamingStatus::LEVEL_Visible))// && (SubLevelState[i].PackageName != "/Game/Environment/Levels/World"))	// Has to have full path at the moment, maybe find a workaround?
		{
			LevelsLoaded++;
		}
	}
	if (LevelsLoaded > 2) // 1
	{
		Loading_Finish();
		FTimerHandle LoadingTimer;
	}
	else
	{
		Loading_InProgress();
	}
}

void UTWLGameInstance::Loading_InProgress()
{
	FTimerHandle LoadingTimer;
	GetWorld()->GetTimerManager().SetTimer(LoadingTimer, this, &UTWLGameInstance::Loading, 0.02f, false);
}

void UTWLGameInstance::Loading_Finish()
{
	GetWorld()->GetWorldSettings()->GlobalGravityZ = GetWorld()->GetDefaultGravityZ();
	GetWorld()->GetWorldSettings()->bGlobalGravitySet = false;
}

void UTWLGameInstance::BeginLoadingScreen(const FString &MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UTWLGameInstance::EndLoadingScreen()
{

}

TArray<FSubLevelStatus> UTWLGameInstance::GetSubLevelsStatus(UWorld* World)
{
	TArray<FSubLevelStatus> Result;
	FWorldContext &Context = GEngine->GetWorldContextFromWorldChecked(World);

	Result.Reserve(World->GetStreamingLevels().Num() + 1);

	// Add persistent level
	{
		FSubLevelStatus LevelStatus = {};
		LevelStatus.PackageName = World->GetOutermost()->GetFName();
		LevelStatus.StreamingStatus = LEVEL_Visible;
		LevelStatus.LODIndex = INDEX_NONE;
		Result.Add(LevelStatus);
	}

	// Iterate over the world info's level streaming objects to find and see whether levels are loaded, visible or neither.
	for (int32 LevelIndex = 0; LevelIndex < World->GetStreamingLevels().Num(); LevelIndex++)
	{
		ULevelStreaming* LevelStreaming = World->GetStreamingLevels()[LevelIndex];

		if (LevelStreaming
			&& !LevelStreaming->GetWorldAsset().IsNull()
			&& LevelStreaming->GetWorldAsset() != World)
		{
			ULevel* Level = LevelStreaming->GetLoadedLevel();
			FSubLevelStatus LevelStatus = {};
			LevelStatus.PackageName = LevelStreaming->GetWorldAssetPackageFName();
			LevelStatus.LODIndex = LevelStreaming->GetLevelLODIndex();

			if (Level != NULL)
			{
				if (World->ContainsLevel(Level) == true)
				{
					if (World->GetCurrentLevelPendingVisibility() == Level)
					{
						LevelStatus.StreamingStatus = LEVEL_MakingVisible;
					}
					else
					{
						LevelStatus.StreamingStatus = LEVEL_Visible;
					}
				}
				else
				{
					LevelStatus.StreamingStatus = LEVEL_Loaded;
				}
			}
			else
			{
				// See whether the level's world object is still around.
				UPackage* LevelPackage = FindObjectFast<UPackage>(NULL, LevelStatus.PackageName);
				UWorld*	  LevelWorld = NULL;
				if (LevelPackage)
				{
					LevelWorld = UWorld::FindWorldInPackage(LevelPackage);
				}

				if (LevelWorld)
				{
					LevelStatus.StreamingStatus = LEVEL_UnloadedButStillAround;
				}
				else if (LevelStreaming->HasLoadRequestPending())
				{
					LevelStatus.StreamingStatus = LEVEL_Loading;
				}
				else
				{
					LevelStatus.StreamingStatus = LEVEL_Unloaded;
				}
			}

			Result.Add(LevelStatus);
		}
	}


	// toss in the levels being loaded by PrepareMapChange
	for (int32 LevelIndex = 0; LevelIndex < Context.LevelsToLoadForPendingMapChange.Num(); LevelIndex++)
	{
		const FName LevelName = Context.LevelsToLoadForPendingMapChange[LevelIndex];

		FSubLevelStatus LevelStatus = {};
		LevelStatus.PackageName = LevelName;
		LevelStatus.StreamingStatus = LEVEL_Preloading;
		LevelStatus.LODIndex = INDEX_NONE;
		Result.Add(LevelStatus);
	}


	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();

		if (PlayerController->GetPawn() != NULL)
		{
			// need to do a trace down here
			//TraceActor = Trace( out_HitLocation, out_HitNormal, TraceDest, TraceStart, false, TraceExtent, HitInfo, true );
			FHitResult Hit(1.f);

			// this will not work for flying around :-(
			PlayerController->GetWorld()->LineTraceSingleByObjectType(Hit, PlayerController->GetPawn()->GetActorLocation(), (PlayerController->GetPawn()->GetActorLocation() - FVector(0.f, 0.f, 256.f)), FCollisionObjectQueryParams(ECC_WorldStatic), FCollisionQueryParams(SCENE_QUERY_STAT(FindLevel), true, PlayerController->GetPawn()));

			ULevel* LevelPlayerIsIn = NULL;

			if (Hit.GetActor() != NULL)
			{
				LevelPlayerIsIn = Hit.GetActor()->GetLevel();
			}
			else if (Hit.Component != NULL)
			{
				LevelPlayerIsIn = Hit.Component->GetComponentLevel();
			}

			if (LevelPlayerIsIn)
			{
				FName LevelName = LevelPlayerIsIn->GetOutermost()->GetFName();
				FSubLevelStatus* LevelStatusPlayerIn = Result.FindByPredicate([LevelName](const FSubLevelStatus& InLevelStatus)
				{
					return InLevelStatus.PackageName == LevelName;
				});

				if (LevelStatusPlayerIn)
				{
					LevelStatusPlayerIn->bPlayerInside = true;
				}
			}
		}
	}

	return Result;
}
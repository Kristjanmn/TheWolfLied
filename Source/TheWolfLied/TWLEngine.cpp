// Made by Kristjanmn

#include "TheWolfLied.h"
#include "GameVariables.h"
#include "TWLEngine.h"

// Engine Init
void UTWLEngine::Init(IEngineLoop* InEngineLoop) {

	UGameEngine::Init(InEngineLoop);

	// Make sure resolution is not dirty (fixes problem with using different aspect ratio)
	if(GameUserSettings->IsScreenResolutionDirty())
	{
		int32 DisplayMode;
		UVideoSettings::GetDisplayMode(DisplayMode);
		UVideoSettings::ChangeDisplayMode(DisplayMode);
	}
	
	bool bConfigured = UGameVariables::GetConfigured();
	if (!bConfigured)
	{
		// Default settings
		bool bShowConfirmMessages = true;
		int iMinutesPerDayNight = 90;
		bConfigured = true;
		int iInventoryColumns = 7;
		int iInventoryIconSize = 80;

		// Create config files with default settings
		UVideoSettings::UseVideoQualityPreset(1);
		UVideoSettings::ChangeDisplayMode(2);
		UGameVariables::SetShowConfirmMessages(bShowConfirmMessages);
		UGameVariables::SetMinutesPerDayNight(iMinutesPerDayNight);
		UGameVariables::SetConfigured(bConfigured);
		UGameVariables::SetInventoryColumns(iInventoryColumns);
		UGameVariables::SetInventoryIconSize(iInventoryIconSize);
	}

	UE_LOG(LogGame, Log, TEXT("Engine Init Success"));
}
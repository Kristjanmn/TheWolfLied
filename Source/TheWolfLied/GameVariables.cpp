// Made by Kristjanmn

#include "TheWolfLied.h"
#include "GameVariables.h"

//ShowConfirmMessages
bool UGameVariables::GetShowConfirmMessages()
{
	bool ShowConfirmMessages;
	GConfig->GetBool(
		TEXT("GameConfig"),
		TEXT("bShowConfirmMessages"),
		ShowConfirmMessages,
		GGameIni
	);

	return ShowConfirmMessages;
}

void UGameVariables::SetShowConfirmMessages(const bool ShowConfirmMessages)
{
	bool bShowConfirmMessages = ShowConfirmMessages;
	GConfig->SetBool(
		TEXT("GameConfig"),
		TEXT("bShowConfirmMessages"),
		bShowConfirmMessages,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
}

//Time of Day
int UGameVariables::GetMinutesPerDayNight()
{
	int MinutesPerDayNight;
	GConfig->GetInt(
		TEXT("GameConfig"),
		TEXT("iMinutesPerDayNight"),
		MinutesPerDayNight,
		GGameIni
	);

	return MinutesPerDayNight;
}

void UGameVariables::SetMinutesPerDayNight(int MinutesPerDayNight)
{
	int iMinutesPerDayNight = MinutesPerDayNight;
	GConfig->SetInt(
		TEXT("GameConfig"),
		TEXT("iMinutesPerDayNight"),
		iMinutesPerDayNight,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
}

//CreateConfig
bool UGameVariables::GetConfigured()
{
	bool Configured;
	GConfig->GetBool(
		TEXT("GameConfig"),
		TEXT("bConfigured"),
		Configured,
		GGameIni
	);

	return Configured;
}

void UGameVariables::SetConfigured(const bool Configured)
{
	bool bConfigured = Configured;
	GConfig->SetBool(
		TEXT("GameConfig"),
		TEXT("bConfigured"),
		bConfigured,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
}

int UGameVariables::GetInventoryColumns()
{
	int Columns;
	GConfig->GetInt(
		TEXT("GameConfig"),
		TEXT("iInventoryColumns"),
		Columns,
		GGameIni
	);

	return Columns;
}

void UGameVariables::SetInventoryColumns(int Columns)
{
	GConfig->SetInt(
		TEXT("GameConfig"),
		TEXT("iInventoryColumns"),
		Columns,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
}

int UGameVariables::GetInventoryIconSize()
{
	int IconSize;
	GConfig->GetInt(
		TEXT("GameConfig"),
		TEXT("iInventoryIconSize"),
		IconSize,
		GGameIni
	);

	return IconSize;
}

void UGameVariables::SetInventoryIconSize(int IconSize)
{
	GConfig->SetInt(
		TEXT("GameConfig"),
		TEXT("iInventoryIconSize"),
		IconSize,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
}

// Get game version
FString UGameVariables::GetProjectVersion() {
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);

	return ProjectVersion;
}
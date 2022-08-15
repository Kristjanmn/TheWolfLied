// Made by Kristjanmn

#include "TheWolfLied.h"
#include "VideoSettings.h"

// Does not work in new version of UE4, returns false
bool UVideoSettings::GetSupportedScreenResolutions(TArray<FString>& Resolutions)
{
	FScreenResolutionArray ResolutionsArray;

	if (RHIGetAvailableResolutions(ResolutionsArray, true))
	{
		for (const FScreenResolutionRHI& Resolution : ResolutionsArray)
		{
			FString StrW = FString::FromInt(Resolution.Width);
			FString StrH = FString::FromInt(Resolution.Height);
			Resolutions.AddUnique(StrW + "x" + StrH);
		}

		return true;
	}

	return false; // failed to obtain screen resolutions
}

// Get current screen resolution
FString UVideoSettings::GetCurrentScreenResolution()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	FIntPoint Resolution = Settings->GetScreenResolution();
	return FString::FromInt(Resolution.X) + "x" + FString::FromInt(Resolution.Y);
}

// Check if in fullscreen mode
void UVideoSettings::GetDisplayMode(int32& DisplayMode)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	bool Borderless;
	GConfig->GetBool(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("bUseBorderlessWindow"),
		Borderless,
		GGameIni
	);

	switch (Settings->GetFullscreenMode()) {
	case EWindowMode::Fullscreen:
		DisplayMode = 0;
		break;
	case EWindowMode::WindowedFullscreen:
		DisplayMode = 1;
		break;
	case EWindowMode::Windowed:
		DisplayMode = 2;
		break;
	}
	if (Borderless) { DisplayMode = 3; }
}

// Check if Vertical Sync is enabled
bool UVideoSettings::IsVSyncEnabled()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	return Settings->IsVSyncEnabled();
}

// Set Vertical Sync
void UVideoSettings::SetVSyncEnabled(const bool VSync)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	Settings->SetVSyncEnabled(VSync);
}

// Set desired resolution (does not change yet) FullscreenMode 0 = full, 1 = boarderless, 2 = windowed
void UVideoSettings::SetScreenResolution(const int32 Width, const int32 Height)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	Settings->SetScreenResolution(FIntPoint(Width, Height));
}

// Change current resolution
void UVideoSettings::ChangeScreenResolution(const int32 Width, const int32 Height, const int32 DisplayMode)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	EWindowMode::Type WindowMode = EWindowMode::Fullscreen;
	bool Borderless = false;

	switch (DisplayMode) {
	case 0:
		WindowMode = EWindowMode::Fullscreen;
		Borderless = false;
		break;
	case 1:
		WindowMode = EWindowMode::WindowedFullscreen;
		Borderless = false;
		break;
	case 2:
		WindowMode = EWindowMode::Windowed;
		Borderless = false;
		break;
	case 3:
		WindowMode = EWindowMode::Windowed;
		Borderless = true;
		break;
	}
	GConfig->SetBool(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("bUseBorderlessWindow"),
		Borderless,
		GGameIni
	);
	GConfig->Flush(false, GGameIni);
	Settings->SetFullscreenMode(WindowMode);
	Settings->RequestResolutionChange(Width, Height, WindowMode, false);
}

// Change display mode only
void UVideoSettings::ChangeDisplayMode(const int32 DisplayMode)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	FIntPoint Resolution = Settings->GetScreenResolution();
	ChangeScreenResolution(Resolution.X, Resolution.Y, DisplayMode);
}

// Get current video quality settings
void UVideoSettings::GetVideoQualitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess, int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance, int32& Foliage)
{
	UGameUserSettings* Settings = GetGameUserSettings();

	AntiAliasing = Settings->ScalabilityQuality.AntiAliasingQuality;
	Effects = Settings->ScalabilityQuality.EffectsQuality;
	PostProcess = Settings->ScalabilityQuality.PostProcessQuality;
	Resolution = Settings->ScalabilityQuality.ResolutionQuality;
	Shadow = Settings->ScalabilityQuality.ShadowQuality;
	Texture = Settings->ScalabilityQuality.TextureQuality;
	ViewDistance = Settings->ScalabilityQuality.ViewDistanceQuality;
	Foliage = Settings->ScalabilityQuality.FoliageQuality;
}

// Save video quality settings
void UVideoSettings::SaveVideoQualitySettings(const int32 AntiAliasing, const int32 Effects, const int32 PostProcess, const int32 Resolution, const int32 Shadow, const int32 Texture, const int32 ViewDistance, const int32 Foliage)
{
	UGameUserSettings* Settings = GetGameUserSettings();

	Settings->ScalabilityQuality.AntiAliasingQuality = AntiAliasing;
	Settings->ScalabilityQuality.EffectsQuality = Effects;
	Settings->ScalabilityQuality.PostProcessQuality = PostProcess;
	Settings->ScalabilityQuality.ResolutionQuality = Resolution;
	Settings->ScalabilityQuality.ShadowQuality = Shadow;
	Settings->ScalabilityQuality.TextureQuality = Texture;
	Settings->ScalabilityQuality.ViewDistanceQuality = ViewDistance;
	Settings->ScalabilityQuality.FoliageQuality = Foliage;
}

// Confirm and save current video mode (resolution and fullscreen/windowed)
void UVideoSettings::SaveVideoModeAndQuality()
{
	UGameUserSettings* Settings = GetGameUserSettings();

	Settings->ConfirmVideoMode();
	Settings->ApplyNonResolutionSettings();
	Settings->SaveSettings();
}

// Revert video settings to original
void UVideoSettings::RevertVideoMode()
{
	UGameUserSettings* Settings = GetGameUserSettings();

	Settings->RevertVideoMode();
}

// Use video settings quality preset
void UVideoSettings::UseVideoQualityPreset(const int32 EVideoQualityPreset)
{
	UGameUserSettings* Settings = GetGameUserSettings();

	Settings->ScalabilityQuality.AntiAliasingQuality = EVideoQualityPreset;
	Settings->ScalabilityQuality.EffectsQuality = EVideoQualityPreset;
	Settings->ScalabilityQuality.PostProcessQuality = EVideoQualityPreset;
	Settings->ScalabilityQuality.ShadowQuality = EVideoQualityPreset;
	Settings->ScalabilityQuality.TextureQuality = EVideoQualityPreset;
	Settings->ScalabilityQuality.ViewDistanceQuality = EVideoQualityPreset;
	Settings->ScalabilityQuality.FoliageQuality = EVideoQualityPreset;
}

//--- PRIVATE

UGameUserSettings* UVideoSettings::GetGameUserSettings()
{
	if (GEngine != nullptr)
	{
		return GEngine->GameUserSettings;
	}

	return nullptr;
}
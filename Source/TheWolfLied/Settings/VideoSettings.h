// Made by Kristjanmn

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VideoSettings.generated.h"

/**
 * Requires RHI dependency
 */

UENUM(BlueprintType)
enum class EVideoQualityPreset : uint8
{
	VQP_Low		UMETA(DisplayName="Low"),
	VQP_Medium	UMETA(DisplayName="Medium"),
	VQP_High	UMETA(DisplayName="High"),
	VQP_Ultra	UMETA(DisplayName="Ultra")
};

UCLASS()
class THEWOLFLIED_API UVideoSettings : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Get list of supported screen resolutions
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static bool GetSupportedScreenResolutions(TArray<FString>& Resolutions);

	// Get current screen resolution
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static FString GetCurrentScreenResolution();

	// Check if in fullscreen mode
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static void GetDisplayMode(int32& DisplayMode);

	// Check if Vertical Sync is enabled
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static bool IsVSyncEnabled();

	// Set Vertical Sync
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void SetVSyncEnabled(const bool VSync);

	// Set screen resolution
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void SetScreenResolution(const int32 Width, const int32 Height);

	// Change screen resolution
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void ChangeScreenResolution(const int32 Width, const int32 Height, const int32 DisplayMode);

	// Change display mode only
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void ChangeDisplayMode(const int32 DisplayMode);

	// Get current video quality settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void GetVideoQualitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess, int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance, int32& Foliage);
	
	// Save video quality settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void SaveVideoQualitySettings(const int32 AntiAliasing, const int32 Effects, const int32 PostProcess, const int32 Resolution, const int32 Shadow, const int32 Texture, const int32 ViewDistance, const int32 Foliage);

	// Confirm and save current video mode (resolution and fullscreen/windowed)
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void SaveVideoModeAndQuality();

	// Revert video settings to original
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void RevertVideoMode();

	// Use video settings quality preset
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static void UseVideoQualityPreset(const int32 EVideoQualityPreset);

private:
	// Try to get GameUserSettings object from the engine
	static UGameUserSettings* GetGameUserSettings();
};
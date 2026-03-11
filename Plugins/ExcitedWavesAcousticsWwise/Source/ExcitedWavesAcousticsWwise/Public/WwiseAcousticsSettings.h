// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AkAuxBus.h"
#include "WwiseAcousticsSettings.generated.h"

USTRUCT()
struct EXCITEDWAVESACOUSTICSWWISE_API FWwiseReverbPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Reverb Preset", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Max absorption for this range. Entries sorted ascending (e.g. 0.2, 0.5, 0.8, 1.0)."))
	float MaxAbsorption = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Reverb Preset", meta = (ToolTip = "Wwise Aux Bus for close-distance reverb. Used when emitter is within Close Distance (from main plugin Detection settings)."))
	TSoftObjectPtr<UAkAuxBus> CloseReverbAuxBus;

	UPROPERTY(EditAnywhere, Category = "Reverb Preset", meta = (ToolTip = "Wwise Aux Bus for mid-distance reverb. Used when emitter is between Close and Mid Distance."))
	TSoftObjectPtr<UAkAuxBus> MidReverbAuxBus;

	UPROPERTY(EditAnywhere, Category = "Reverb Preset", meta = (ToolTip = "Wwise Aux Bus for far-distance reverb. Used when emitter is between Mid and Far Distance."))
	TSoftObjectPtr<UAkAuxBus> FarReverbAuxBus;

	UPROPERTY(EditAnywhere, Category = "Reverb Preset", meta = (ToolTip = "Wwise Aux Bus for outdoor/open space reverb. Used when emitter is beyond Far Distance or in open environment."))
	TSoftObjectPtr<UAkAuxBus> OutdoorReverbAuxBus;

	UPROPERTY(EditAnywhere, Category = "Reverb Preset", meta = (DisplayName = "Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0",
		ToolTip = "Volume offset in dB applied to all send levels for this preset. Use negative values to reduce loud presets (e.g. low-absorption with long tails). Applied independently of Compensate Absorption."))
	float SendVolumeOffsetDb = 0.0f;

	FWwiseReverbPreset() : MaxAbsorption(1.0f) {}
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Excited Waves Acoustics Wwise Integration"))
class EXCITEDWAVESACOUSTICSWWISE_API UWwiseAcousticsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UWwiseAcousticsSettings();

	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Wwise Acoustics Settings")
	static const UWwiseAcousticsSettings* Get();

	const FWwiseReverbPreset* GetReverbPresetForAbsorption(float Absorption) const;

	UPROPERTY(EditAnywhere, Config, Category = "Reverb Presets", meta = (DisplayName = "Reverb Presets", ToolTip = "Each entry: MaxAbsorption + Close/Mid/Far/Outdoor aux buses. Sorted by MaxAbsorption ascending. First entry with absorption <= MaxAbsorption is used."))
	TArray<FWwiseReverbPreset> ReverbPresets;

	UPROPERTY(EditAnywhere, Config, Category = "Reverb Presets", meta = (DisplayName = "Preset Hysteresis Margin", ClampMin = "0.0", ClampMax = "0.2",
		ToolTip = "Absorption must change by this amount beyond the preset boundary before switching. Higher = more stable, less responsive."))
	float PresetHysteresisMargin = 0.04f;

	UPROPERTY(EditAnywhere, Config, Category = "Reverb Presets", meta = (DisplayName = "Preset Crossfade Duration", ClampMin = "0.0", ClampMax = "2.0",
		ToolTip = "Duration in seconds for crossfading between reverb presets when a switch occurs. 0 = instant switch."))
	float PresetCrossfadeDuration = 0.3f;

	UPROPERTY(EditAnywhere, Config, Category = "Reverb Presets", meta = (DisplayName = "Compensate Absorption",
		ToolTip = "When enabled, SDK reduces reverb send levels based on mean absorption (close/mid/far *= exp(-1.8*meanAbsorption)). Disable to hear raw reverb sends unmodified by absorption."))
	bool bEnableApplyAbsorption = true;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Early Reflections Bus", ToolTip = "Aux bus asset for Wwise Reflect early reflections. Assign a Wwise Aux Bus with Reflect effect."))
	TSoftObjectPtr<UAkAuxBus> EarlyReflectionBusAsset;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Enable Reflections", ToolTip = "Enable early reflections via Wwise Reflect when a bus is assigned."))
	bool bEnableReflections = true;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Enable Spatial Radius Management", ToolTip = "Filter and merge image sources by proximity. Reduces number of sources sent to Wwise Reflect when many are close together."))
	bool ReflectEnableSpatialRadius = true;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Image Source Radius (cm)", ClampMin = "100.0", ClampMax = "50000.0", EditCondition = "ReflectEnableSpatialRadius", ToolTip = "Radius for spatial grouping of image sources"))
	float ReflectImageSourceRadius = 2000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Position Match Threshold (cm)", ClampMin = "1.0", ClampMax = "500.0", EditCondition = "ReflectEnableSpatialRadius", ToolTip = "Sources within this distance are merged"))
	float ReflectPositionMatchThreshold = 200.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Max Image Sources", ClampMin = "1", ClampMax = "64", ToolTip = "Maximum number of image sources per emitter sent to Wwise Reflect"))
	int32 MaxReflectImageSources = 6;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Fade In Duration (s)", ClampMin = "0.0", ClampMax = "2.0", ToolTip = "Time in seconds for a new reflection to fade in from silence to full level. 0 = instant."))
	float ReflectFadeInDuration = 0.15f;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Fade Out Duration (s)", ClampMin = "0.0", ClampMax = "2.0", ToolTip = "Time in seconds for a lost reflection to fade out from current level to silence. 0 = instant."))
	float ReflectFadeOutDuration = 0.3f;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Max Reflect Distance (cm)", ClampMin = "0.0", ClampMax = "100000.0", ToolTip = "Emitters beyond this distance from the listener will not send reflections to Wwise. 0 = no distance culling."))
	float ReflectMaxDistance = 5000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Show Reflect Hit Points", ToolTip = "Draw debug spheres at Reflect image source positions"))
	bool ShowReflectHitPoints = false;

	UPROPERTY(EditAnywhere, Config, Category = "Reflect", meta = (DisplayName = "Reflect Hit Point Color", EditCondition = "ShowReflectHitPoints", ToolTip = "Color for Reflect debug spheres"))
	FColor DebugReflectHitPointColor = FColor(0, 240, 255);

};

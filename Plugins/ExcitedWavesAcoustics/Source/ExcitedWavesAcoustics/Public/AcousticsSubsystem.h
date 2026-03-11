// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsDetectorManager.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsDebugTypes.h"
#include "ExcitedWavesAcousticsSettings.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "WorldCollision.h"
#include <memory>
#include "AcousticsSubsystem.generated.h"

class UAcousticsDetectorComponent;

USTRUCT(BlueprintType)
struct FAcousticsDebugOverlayEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FString ActorName;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FString Text;
};

UCLASS()
class EXCITEDWAVESACOUSTICS_API UAcousticsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UAcousticsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	EWAcoustics::SDK::DetectorId RegisterComponent(UAcousticsDetectorComponent* Component);
	void UnregisterComponent(EWAcoustics::SDK::DetectorId DetectorId);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Load Balancing")
	void SetListenerPosition(const FVector& Position);

	void UpdateDetector(EWAcoustics::SDK::DetectorId DetectorId, const EWAcoustics::SDK::Vector3& Position, float DeltaTime);
	void ForceRestartDetector(EWAcoustics::SDK::DetectorId DetectorId);

	const EWAcoustics::SDK::AcousticsResults* GetDetectorResults(EWAcoustics::SDK::DetectorId DetectorId) const;
	const EWAcoustics::SDK::DebugData* GetDetectorDebugData(EWAcoustics::SDK::DetectorId DetectorId) const;
	bool DetectorHasNewDebugData(EWAcoustics::SDK::DetectorId DetectorId) const;
	void MarkDetectorDebugDataConsumed(EWAcoustics::SDK::DetectorId DetectorId);

	const std::vector<EWAcoustics::SDK::ProbeResult>* GetDetectorSideProbeResults(EWAcoustics::SDK::DetectorId DetectorId) const;
	const std::vector<EWAcoustics::SDK::ProbeResult>* GetDetectorUpProbeResults(EWAcoustics::SDK::DetectorId DetectorId) const;
	const std::vector<EWAcoustics::SDK::ImageSource>* GetDetectorImageSources(EWAcoustics::SDK::DetectorId DetectorId) const;
	bool IsDetectorReflectCulled(EWAcoustics::SDK::DetectorId DetectorId) const;

	uint32 GetGlobalRaysScheduledThisFrame() const;
	const EWAcoustics::SDK::AcousticsConfig& GetGlobalConfig() const { return m_globalConfig; }
	const FVector& GetListenerPosition() const { return m_listenerPosition; }

	void SetListenerDetector(EWAcoustics::SDK::DetectorId DetectorId);
	EWAcoustics::SDK::DetectorId GetListenerDetectorId() const { return m_listenerDetectorId; }
	const EWAcoustics::SDK::AcousticsResults* GetListenerDetectorResults() const;

	/** Returns processed send levels with cross-room blend and distance attenuation applied. */
	EWAcoustics::SDK::ProcessedSendLevels GetDetectorProcessedSends(EWAcoustics::SDK::DetectorId DetectorId) const;

	void SyncListenerBlendConfig();
	void LogMaterialAbsorptionSummary(EWAcoustics::SDK::DetectorId DetectorId);

	/** Debug overlay entries for emitters with "Draw Debug Text On Screen" enabled. Bind to UMG widget. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug", meta = (DisplayName = "Get Debug Overlay Texts"))
	TArray<FAcousticsDebugOverlayEntry> GetDebugOverlayTexts() const;

	void SetDebugOverlayText(EWAcoustics::SDK::DetectorId DetectorId, const FString& ActorName, const FString& Text);
	void ClearDebugOverlayText(EWAcoustics::SDK::DetectorId DetectorId);

	/** Disabling clears all sends; enabling triggers re-evaluation. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics")
	void SetSystemEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics")
	bool IsSystemEnabled() const { return m_bSystemEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug")
	void SetDebugTextEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug")
	void SetDebugRaysEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug")
	void SetDebugHitsEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsDebugTextEnabled() const { return m_bDebugTextEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsDebugRaysEnabled() const { return m_bDebugRaysEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsDebugHitsEnabled() const { return m_bDebugHitsEnabled; }

	/** Toggles cross-room blending and forces immediate re-evaluation of all emitters. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Features")
	void SetCrossRoomBlendingEnabled(bool bEnabled);

	/** Toggles distance-based reverb attenuation and forces immediate re-evaluation. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Features")
	void SetDistanceAttenuationEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Features")
	bool IsCrossRoomBlendingEnabled() const { return m_bCrossRoomEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Features")
	bool IsDistanceAttenuationEnabled() const { return m_bDistanceAttenuationEnabled; }

	/** Switch ray scanning method at runtime. Restart is debounced internally. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Parameters")
	void SetRayScanningMethod(ERayScanningMethod NewMethod);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Parameters")
	ERayScanningMethod GetRayScanningMethod() const { return m_currentRayScanningMethod; }

private:
	void LoadGlobalSettings();
	void OnRaycastComplete(const FTraceHandle& TraceHandle, FTraceDatum& TraceData);
	EWAcoustics::SDK::ProbeResult CreateProbeResult(
		EWAcoustics::SDK::DetectorId DetectorId,
		const EWAcoustics::SDK::ProbeRequest& Request,
		const FHitResult& HitResult,
		const FVector& EndPoint
	);
	void ScheduleRaycastsForDetector(EWAcoustics::SDK::DetectorId DetectorId, UAcousticsDetectorComponent* Component);

	/** Runs all rays synchronously in one frame for bImmediateDetection components. */
	void RunImmediateDetection(EWAcoustics::SDK::DetectorId DetectorId, UAcousticsDetectorComponent* Component);

private:
	using FDetectorManagerPtr = std::unique_ptr<EWAcoustics::SDK::AcousticsDetectorManager, EWAcoustics::SDK::AcousticsDetectorManagerDeleter>;
	FDetectorManagerPtr m_detectorManager;
	EWAcoustics::SDK::AcousticsConfig m_globalConfig;
	FVector m_listenerPosition;
	EWAcoustics::SDK::DetectorId m_listenerDetectorId;
	TMap<EWAcoustics::SDK::DetectorId, UAcousticsDetectorComponent*> m_componentMap;

	struct FPendingRaycast
	{
		EWAcoustics::SDK::ProbeRequest Request;
		FVector EndPoint;
		EWAcoustics::SDK::DetectorId DetectorId;
		uint32_t DetectionGeneration;

		FPendingRaycast()
			: EndPoint(FVector::ZeroVector)
			, DetectorId(0)
			, DetectionGeneration(0)
		{
		}

		FPendingRaycast(
			EWAcoustics::SDK::DetectorId InDetectorId,
			const EWAcoustics::SDK::ProbeRequest& InRequest,
			const FVector& InEndPoint,
			uint32_t InGeneration
		)
			: Request(InRequest)
			, EndPoint(InEndPoint)
			, DetectorId(InDetectorId)
			, DetectionGeneration(InGeneration)
		{
		}
	};

	TMap<FTraceHandle, FPendingRaycast> m_pendingRaycasts;

	struct FSurfaceTypeHitInfo
	{
		TMap<FString, int32> SurfaceTypeCounts;
		TMap<FString, float> SurfaceTypeAbsorptions;
		int32 TotalHits = 0;
	};
	TMap<EWAcoustics::SDK::DetectorId, FSurfaceTypeHitInfo> m_surfaceTypeHitInfo;
	TMap<EWAcoustics::SDK::DetectorId, EWAcoustics::SDK::EDetectorState> m_detectorStates;
	TMap<EWAcoustics::SDK::DetectorId, FAcousticsDebugOverlayEntry> m_debugOverlayTexts;

	bool m_bSystemEnabled = true;
	bool m_bDebugTextEnabled = true;
	bool m_bDebugRaysEnabled = true;
	bool m_bDebugHitsEnabled = true;
	bool m_bCrossRoomEnabled = true;
	bool m_bDistanceAttenuationEnabled = true;
	ERayScanningMethod m_currentRayScanningMethod = ERayScanningMethod::Umbrella;
	FTimerHandle m_parameterDebounceTimer;

	void ForceReEvaluateAllEmitters();
	void PropagateConfigAndRestart(bool bRestart);
	void ClearAllEmitterSends();
	void ClearPendingRaycastsForDetector(EWAcoustics::SDK::DetectorId DetectorId);

	/** Debounced restart to avoid spam when sliders are being dragged. */
	void ScheduleDebouncedRestart();
};


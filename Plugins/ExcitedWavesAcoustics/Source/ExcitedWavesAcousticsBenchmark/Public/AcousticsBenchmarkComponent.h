// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "AcousticsBenchmarkComponent.generated.h"

class AActor;
class UAcousticsDetectorComponent;

UENUM(BlueprintType)
enum class EBenchmarkMovementPattern : uint8
{
	Static,
	Circle,
	Linear,
	Random,
	Grid
};

USTRUCT(BlueprintType)
struct FBenchmarkMetrics
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float CurrentFPS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float AverageFPS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MinFPS = 9999.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MaxFPS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float FrameTimeMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float AverageFrameTimeMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float PeakFrameTimeMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 ActiveDetectorCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 RaycastsPerFrame = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float AverageRaycastsPerFrame = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MemoryUsageMB = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float BenchmarkDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 FrameDrops = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 SevereFrameDrops = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXCITEDWAVESACOUSTICSBENCHMARK_API UAcousticsBenchmarkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAcousticsBenchmarkComponent();
	virtual ~UAcousticsBenchmarkComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	void StartBenchmark();

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	void StopBenchmark();

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	void ResetBenchmark();

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	FBenchmarkMetrics GetCurrentMetrics() const { return m_currentMetrics; }

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	bool IsBenchmarkRunning() const { return m_bIsRunning; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ClampMin = "1", ClampMax = "10000", ToolTip = "Number of actors to spawn for the benchmark test"))
	int32 ActorCount = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ToolTip = "Actor template to spawn. Can have AcousticsDetectorComponent (for acoustics test) or not (for baseline test)."))
	TSubclassOf<AActor> ActorTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ToolTip = "Movement pattern for spawned actors: Static, Circle, Linear, Random, or Grid"))
	EBenchmarkMovementPattern MovementPattern = EBenchmarkMovementPattern::Circle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern != EBenchmarkMovementPattern::Static", EditConditionHides, ToolTip = "Speed of actor movement (units per second)"))
	float MovementSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Circle", EditConditionHides, ToolTip = "Radius of circular movement path (cm)"))
	float CircleRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Linear", EditConditionHides, ToolTip = "Distance for linear back-and-forth movement (cm)"))
	float LinearDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Random", EditConditionHides, ToolTip = "Bounds for random movement (cm). Actors move within a cube of this size"))
	float RandomBounds = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "1", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Grid", EditConditionHides, ToolTip = "Grid dimension (NxN). Actors are placed in an NxN grid"))
	int32 GridSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Grid", EditConditionHides, ToolTip = "How far actors oscillate from their grid position (cm)"))
	float GridOscillationAmplitude = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Grid", EditConditionHides, ToolTip = "Speed of oscillation within the grid (Hz)"))
	float GridOscillationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ClampMin = "0", ToolTip = "Distance between spawned actors (cm). Used when spawning in grid layout"))
	float SpawnSpacing = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ToolTip = "When enabled, actors spawn in a grid. When disabled, spawn in a circle"))
	bool bSpawnInGrid = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ToolTip = "Optimize spawned actors by disabling tick, collision, and other overhead. Recommended for performance testing."))
	bool bOptimizeActors = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup")
	bool bAutoStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ClampMin = "0"))
	float AutoStopDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ToolTip = "Show FPS, frame time, and other metrics on screen during benchmark"))
	bool bShowOnScreenMetrics = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ClampMin = "0.01", ToolTip = "How often to update on-screen metrics (seconds)"))
	float MetricsUpdateInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ToolTip = "Log metrics to output log at Log Interval"))
	bool bLogMetrics = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ClampMin = "1.0", ToolTip = "Interval between metric log entries (seconds). Used when Log Metrics is enabled"))
	float LogInterval = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Benchmark|Results")
	FBenchmarkMetrics m_currentMetrics;

private:
	void GenerateActors();
	void DestroyActors();
	void UpdateActorMovement(float DeltaTime);
	void UpdateMetrics(float DeltaTime);
	void DrawOnScreenMetrics();
	FVector CalculateSpawnPosition(int32 ActorIndex) const;
	FVector CalculateTargetPosition(int32 ActorIndex, float DeltaTime);
	float GetMemoryUsageMB() const;

	bool m_bIsRunning = false;
	int32 m_cachedDetectorCount = 0;

	UPROPERTY()
	TArray<AActor*> m_spawnedActors;

	struct FActorMovementData
	{
		FVector StartPosition;
		FVector CurrentTarget;
		float Angle = 0.0f;
		float LinearProgress = 0.0f;
		bool bMovingForward = true;
		FVector RandomDirection;
	};

	TArray<FActorMovementData> m_movementData;

	float m_metricsUpdateTimer = 0.0f;
	float m_logTimer = 0.0f;
	float m_benchmarkStartTime = 0.0f;
	TArray<float> m_fpsHistory;
	TArray<float> m_frameTimeHistory;
	TArray<int32> m_raycastHistory;

	static constexpr float FrameDropThreshold30FPS = 33.33f;
	static constexpr float SevereFrameDropThreshold = 50.0f;
};

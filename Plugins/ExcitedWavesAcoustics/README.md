# Excited Waves Acoustics

**Runtime acoustic environment detection for Unreal Engine**

Excited Waves Acoustics gives every sound source its own acoustic properties at runtime using raycasting – room geometry, material absorption, and indoor/outdoor transitions are detected automatically. The system drives reverb blending (Small/Medium/Large/Outdoor), absorption compensation, cross-room blending, and early reflections with no manual setup, no baking, and no reverb zones. It continuously adapts to any environment, including dynamically destroyed and procedurally generated geometry.

> **Trial Version.** This distribution is fully functional in Unreal Editor for evaluation and prototyping. Packaging a standalone game build requires a full license. For licensing inquiries, contact us at [contact@excitedwaves.com](mailto:contact@excitedwaves.com).

## Requirements

- Unreal Engine 5.6.1
- Windows (Win64)

## Installation

1. Copy the `ExcitedWavesAcoustics` folder into your project's `Plugins/` directory.
2. Restart the Unreal Editor. The precompiled plugin will be loaded automatically.
3. The plugin will appear under **Edit > Plugins > Audio > Excited Waves Acoustics**.

## Quick Start

1. **Add the component.** Attach a `UAcousticsDetectorComponent` to any actor (typically a character or audio listener).
2. **Play.** The detector automatically begins scanning the environment. No further setup is required for basic operation.
3. **Tune.** Adjust detection and audio parameters in **Project Settings > Plugins > Excited Waves Acoustics**.

## How It Works

The detector casts rays outward from its position in configurable patterns (umbrella or uniform distribution). Ray hit distances are binned into **Close**, **Mid**, **Far**, and **Outdoor** categories. Together with ceiling/floor measurements and material absorption coefficients, these bins produce a continuous characterization of the acoustic space. Results update automatically whenever the detector moves beyond a configurable threshold.

An audio middleware adapter receives these results and translates them into reverb sends, early reflections, or any other audio parameter your pipeline requires. See the **Excited Waves Acoustics – Wwise Integration** plugin for a production-ready implementation.

## Project Settings

All global settings are under **Project Settings > Plugins > Excited Waves Acoustics**.

### Detection

| Setting | Default | Description |
|---------|---------|-------------|
| Ray Scanning Method | Umbrella | Ray distribution pattern. **Umbrella** casts paired azimuth rays in steps with elevation iterations. **Uniform** distributes rays evenly on a sphere. |
| Max Azimuth Ray Distance | 10 000 cm | Maximum horizontal ray length. |
| Max Elevation Ray Distance | 5 000 cm | Maximum vertical ray length. |
| Max Probe Height | 1 000 cm | Upper limit for horizontal probe elevation spread in Umbrella mode. |
| Pairs Per Iteration | 2 | Opposite ray pairs per azimuth iteration (Umbrella mode). |
| Azimuth Iterations | 2 | Number of azimuth rotation iterations (Umbrella mode). |
| Elevation Rays | 3 | Vertical rays per direction (Umbrella mode). |
| Angled Ray Iterations | 2 | Extra elevation iterations at increasing angles (Umbrella mode). |
| Uniform Rays Count | 16 | Total rays when using Uniform mode. |
| Close / Mid / Far Distance | 360 / 1 000 / 5 000 cm | Bin boundaries for distance categorization. Hits beyond Far are classified as Outdoor. |
| Verification Ray Offset | 35 % | Height offset for verification rays that validate horizontal hits. |

### Frame Limiting

| Setting | Default | Description |
|---------|---------|-------------|
| Enable Frame Limiting | true | Spread rays across multiple frames to control per-frame cost. |
| Frame Limiting Mode | PerDetector | **Global** – single budget shared by all detectors. **PerDetector** – each detector gets its own budget. |
| Max Rays Per Frame | 8 | Ray budget per frame (or per detector, depending on mode). |

### Distances & LOD

| Setting | Default | Description |
|---------|---------|-------------|
| Position Change Threshold | 100 cm | Movement distance that triggers a new detection cycle. |
| Enable Distance-Based LOD | false | Scale detection frequency by emitter–listener distance. |
| Close / Far Emitter Threshold | 50 / 300 cm | LOD distance bands when LOD is enabled. |
| Detector Circle Radius | 150 cm | Radius of the circle around the detector used for ray origin offsets. |

### Audio

| Setting | Default | Description |
|---------|---------|-------------|
| Use Results Interpolation | true | Smooth transitions between detection results. |
| Interpolation Duration | 1.0 s | Duration to blend from old to new results. |
| Enable Cross-Room Reverb Blending | true | Blend reverb contributions from adjacent spaces. |
| Cross-Room Blend Curve Shape | Exponential | Attenuation curve (Linear, Smooth, Exponential, Inverse, or custom Curve). |
| Cross-Room Blend Min / Max Distance | 0 / 3 000 cm | Distance range for cross-room blending. |
| Max Cross-Room Contribution | 0.5 | Upper limit for blended cross-room reverb. |
| Enable Distance Reverb Attenuation | true | Reduce reverb send levels with distance. |
| Distance Attenuation Curve Shape | Exponential | Curve shape for distance attenuation. |
| Distance Attenuation Min / Max Distance | 0 / 2 000 cm | Attenuation distance range. |
| Distance Attenuation Min Value | 0.3 | Floor for distance attenuation (never fully silent). |
| Enable Enclosure Scaling | false | Scale reverb sends by the enclosure metric. |
| Surface Type Absorption Entries | *(empty)* | Map Physical Surface types to absorption coefficients (0–1). |
| Default Absorption | 0.0 | Absorption used for surfaces without an explicit mapping. |

### Analysis

| Setting | Default | Description |
|---------|---------|-------------|
| Enable Width Modulator | true | When enabled, room width boosts mid/far reverb bins. Wider rooms get more Medium and Large reverb. |
| Width Modulator Strength | 3.0 | Intensity of width-based modulation. |
| Height Boost Strength | 0.5 | Additional weight given to tall spaces. |
| Bin Noise Threshold | 0.05 | Suppress bin values below this threshold. |

### Debug

| Setting | Default | Description |
|---------|---------|-------------|
| Debug Display Mode | Brief | **Full** or **Brief** overlay detail level. |
| Show Debug Rays | false | Draw ray lines in the viewport. |
| Show Hit Points | false | Draw spheres at ray hit locations. |
| Show Results | false | Display detection results as on-screen text. |
| Debug Ray Lifetime | 60 s | How long rays remain visible. |
| Debug Line Thickness | 1.0 | Ray line width. |

Ray colors are fully configurable (Side, Up, Down, Verification, Unverified, Missed, Outlier).

## Per-Detector Settings

Each `UAcousticsDetectorComponent` exposes instance-level overrides:

| Property | Default | Description |
|----------|---------|-------------|
| Enabled | true | Enable/disable this detector. |
| Immediate First Detection | false | Run detection on BeginPlay without waiting for movement. |
| Is Listener | false | Mark this detector as the listener position (affects cross-room calculations). |
| Auto Update Acoustics | true | Automatically push results to the audio adapter each tick. |

Debug text position (distance, offset, scale) is also configurable per detector.

## Blueprint API

The detector component provides these Blueprint-callable functions:

- `SetDetectorEnabled(bool)` / `IsDetectorEnabled()` – enable or disable the detector at runtime.
- `ForceRecalculation()` – trigger an immediate detection cycle.
- `IsAnalysisReady()` – true once the first detection cycle has completed and results are valid.
- `GetCloseReverbPercentage()` / `GetMidReverbPercentage()` / `GetFarReverbPercentage()` / `GetOutdoorReverbPercentage()` – current per-bin reverb amounts.
- `GetAbsorption()` – mean surface absorption (0–1).
- `GetIsIndoor()` – whether the detector considers its position indoors.
- `GetDebugOverlayData()` – all analysis values packed into a struct for UMG widget binding.

The `UAcousticsSubsystem` (world subsystem) offers runtime toggles:

- `SetSystemEnabled(bool)` – enable/disable the entire acoustics system.
- `SetDebugTextEnabled(bool)` / `SetDebugRaysEnabled(bool)` / `SetDebugHitsEnabled(bool)` – toggle debug visualization.
- `SetCrossRoomBlendingEnabled(bool)` / `SetDistanceAttenuationEnabled(bool)` – toggle audio features.
- `SetRayScanningMethod(ERayScanningMethod)` – switch between Umbrella and Uniform at runtime.

## Benchmark Component

The plugin includes an optional `UAcousticsBenchmarkComponent` for stress-testing. Attach it to an actor and configure:

- **Actor Count** – number of detector actors to spawn (1–10 000).
- **Movement Pattern** – Static, Circle, Linear, Random, or Grid.
- **Auto Start / Auto Stop** – run benchmarks unattended.
- **On-Screen Metrics** – live FPS, frame time, raycasts/frame, and memory usage.

## Audio Adapter Interface

Excited Waves Acoustics is middleware-agnostic. It defines an `IAcousticsAudioAdapter` interface that audio integration plugins implement. The adapter receives:

- **Reverb send data** – per-bin (Close/Mid/Far/Outdoor) send levels, absorption, enclosure, and room dimensions.
- **Reflection data** – image source positions and levels for early reflections.

See the **Excited Waves Acoustics – Wwise Integration** plugin for a production-ready adapter implementation.

## Performance

- **Frame limiting** spreads ray computation across frames; a typical configuration of 8 rays/frame has negligible CPU impact.
- **Position threshold** prevents redundant scans when the detector is stationary.
- **Distance-based LOD** (optional) reduces scan frequency for distant emitters.
- The detector reuses internal buffers and avoids per-frame allocations.

## License

Copyright 2025-2026 Excited Waves. All Rights Reserved.

- Website: [excitedwaves.com](https://excitedwaves.com)
- Documentation: [excitedwaves.gitbook.io/excitedwaves-docs](https://excitedwaves.gitbook.io/excitedwaves-docs/)
- Licensing & Support: [contact@excitedwaves.com](mailto:contact@excitedwaves.com)

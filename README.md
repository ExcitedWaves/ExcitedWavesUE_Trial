# Excited Waves Acoustics

**Runtime acoustic environment detection for Unreal Engine 5**

[![Unreal Engine 5.6.1](https://img.shields.io/badge/Unreal%20Engine-5.6.1-blue)](https://www.unrealengine.com/)
[![Wwise 2024.1](https://img.shields.io/badge/Wwise-2024.1.11-blueviolet)](https://www.audiokinetic.com/)
[![Platform](https://img.shields.io/badge/Platform-Win64-lightgrey)]()

---

[![Watch the demo](https://img.youtube.com/vi/ScII63fIz-c/maxresdefault.jpg)](https://www.youtube.com/watch?v=ScII63fIz-c)

Excited Waves Acoustics is a runtime acoustics system for Unreal Engine. Every sound source gets its own acoustic properties automatically – the system uses frame-limited raycasting to evaluate surrounding geometry and drives reverb blending (Small/Medium/Large/Outdoor), absorption compensation, cross-room blending, and early reflections in real time.

No reverb volumes. No trigger boxes. No baking. No re-authoring when level design changes. Works out of the box with any geometry, including dynamically destroyed and procedurally generated environments.

> **Trial Version.** All plugins in this repository are fully functional in Unreal Editor for evaluation and prototyping. Packaging a standalone game build requires a full license.
> For licensing inquiries, contact us at **[contact@excitedwaves.com](mailto:contact@excitedwaves.com)**.

## Current Version

- **Unreal Engine 5.6.1**, Windows (Win64)
- **Audiokinetic Wwise 2024.1.11** – required for audio output in the current release

The core detection engine is middleware-agnostic by design, but the current release only ships with a **Wwise audio adapter**. Wwise integration is effectively required for a working audio pipeline today.

### Upcoming

- **UE 5.7** support
- **Native UE Audio + MetaSounds** adapter

## Plugins

| Plugin | Description |
|--------|-------------|
| [**Excited Waves Acoustics**](Plugins/ExcitedWavesAcoustics/README.md) | Core detection engine. Casts rays, analyzes the environment, produces continuous acoustic metrics (distance bins, absorption, enclosure, room dimensions). Middleware-agnostic. |
| [**Excited Waves Acoustics – Wwise**](Plugins/ExcitedWavesAcousticsWwise/README.md) | Wwise audio adapter. Translates detection results into Wwise aux bus sends (reverb) and Wwise Reflect image sources (early reflections). Supports absorption-based preset selection with crossfading. **Required in the current release.** |
| [**Excited Waves Acoustics – Chaos**](Plugins/ExcitedWavesAcousticsChaos/README.md) | Optional. Listens to Chaos destruction events and automatically triggers acoustic recalculation when geometry breaks. Zero setup required. |

## What This Demo Shows

This project is a playable Unreal Engine scene demonstrating the plugin suite in action:

- **Indoor/outdoor transitions** – walk between enclosed rooms, hallways, and open exteriors; hear the reverb adapt continuously.
- **Material-aware absorption** – different Physical Surface types (concrete, wood, metal) map to absorption coefficients that influence reverb character.
- **Chaos destruction** – destroy walls and hear the acoustic environment update in real time as the room opens up.
- **Cross-room blending** – emitters in adjacent rooms contribute attenuated reverb based on distance and geometry.
- **Early reflections** – Wwise Reflect receives image source positions computed from ray hits, producing spatialized early reflections.
- **Debug visualization** – toggle ray display, hit points, and on-screen metrics to see exactly what the detector computes.

## Getting Started

### Prerequisites

- **Unreal Engine 5.6.1** (installed via Epic Games Launcher)
- **Wwise 2024.1.11** with UE integration (required for audio playback in this demo)

### Wwise Plugin Dependencies

If you want to **regenerate SoundBanks** (e.g., after modifying the Wwise project), the following Wwise plugins are required:

- **Ak Reflect** – used for early reflections
- **Ak Convolution Reverb** – used for reverb

Obtain plugin licenses here: [Audiokinetic – Register Project](https://www.audiokinetic.com/register-project/).

### Running the Demo

1. Clone or download this repository.
2. Open `EWAcousticsTrialDev.uproject` in Unreal Editor 5.6.1.
3. Press **Play** in the editor.

The demo level is pre-configured with acoustics detectors, Wwise integration, and destructible geometry. Walk around to hear the acoustic response change; destroy walls to observe real-time recalculation.

### Using the Plugins in Your Own Project

Install the plugins from the [ExcitedWavesAcousticsUE_PluginTrial](https://github.com/ExcitedWaves/ExcitedWavesAcousticsUE_PluginTrial) repository. See its README for installation and configuration instructions.

## License

Copyright 2025-2026 Excited Waves. All Rights Reserved.

This repository contains a **Trial** distribution of Excited Waves Acoustics plugins. The plugins are distributed as precompiled binaries with public headers (no C++ source code). The trial is fully functional in Unreal Editor for evaluation purposes. Packaging standalone game builds is not available in the trial version.

For a full license (includes source code and game packaging), please contact us.

### Third-Party Software

This project includes third-party software from Audiokinetic (Wwise), Geometric Tools, and others. The Wwise plugins (`Plugins/Wwise`, `Plugins/WwiseSoundEngine`, `Plugins/WwiseNiagara`) are proprietary software by Audiokinetic, Inc. and require a valid Wwise license. Wwise is free for non-commercial projects with certain limitations – see [Audiokinetic Pricing](https://www.audiokinetic.com/pricing/).

See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for full license texts and attributions.

## Contact

- **Website:** [excitedwaves.com](https://excitedwaves.com)
- **Documentation:** [excitedwaves.gitbook.io/excitedwaves-docs](https://excitedwaves.gitbook.io/excitedwaves-docs/)
- **Licensing & Support:** [contact@excitedwaves.com](mailto:contact@excitedwaves.com)

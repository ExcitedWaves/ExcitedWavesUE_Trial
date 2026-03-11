# Excited Waves Acoustics – Wwise Integration

**Reverb sends and early reflections driven by real-time acoustic detection**

This plugin connects Excited Waves Acoustics to Audiokinetic Wwise. Detection results are translated into Wwise aux bus sends for reverb and into image source data for the Wwise Reflect early reflections plugin – all in real time, with no manual reverb zone authoring.

> **Trial Version.** This distribution is fully functional in Unreal Editor for evaluation and prototyping. Packaging a standalone game build requires a full license. For licensing inquiries, contact us at [contact@excitedwaves.com](mailto:contact@excitedwaves.com).

## Requirements

- Unreal Engine 5.6.1
- **Excited Waves Acoustics** plugin (core)
- **Wwise** UE integration plugin (Audiokinetic), version 2024.1.11 or compatible
- **Wwise Reflect** plugin by Audiokinetic (required for early reflections; free evaluation available)

## Installation

1. Copy the `ExcitedWavesAcousticsWwise` folder into your project's `Plugins/` directory (alongside `ExcitedWavesAcoustics`).
2. Restart the Unreal Editor. The precompiled plugin will be loaded automatically.
3. The plugin will appear under **Edit > Plugins > Audio > Excited Waves Acoustics - Wwise Integration**.

## Wwise Reflect – License Setup

Early reflections require the **Wwise Reflect** plugin, which is licensed separately by Audiokinetic. A free evaluation license is available for non-commercial use:

1. Open the **Audiokinetic Launcher**.
2. Go to the **Plug-ins** tab, select your Wwise version.
3. Click **Add Plug-ins**, select **Wwise Reflect**, and click **Install**.
4. Register your project at [audiokinetic.com/register-project](https://www.audiokinetic.com/register-project/) and request a free Reflect evaluation license.
5. In Wwise Authoring, go to **Project > License Manager**, paste the license key, and click **Close**.
6. Save the project and restart Wwise.

Without a Reflect license, reverb sends will still work normally – only early reflections will be inactive.

For details, see [Audiokinetic – Try Plug-ins for Free](https://www.audiokinetic.com/en/wwise/plugins/try-for-free/).

## Wwise Project Setup

### Reverb Aux Buses

Create aux buses in your Wwise project for each distance bin and absorption range. The demo project ships with three presets:

| Absorption Range | Close (Small) Bus | Mid Bus | Far (Large) Bus |
|-----------------|-------------------|---------|-----------------|
| Reflective (0–0.33) | reflective_small | reflective_mid | reflective_large |
| Neutral (0.33–0.66) | neutral_small | neutral_mid | neutral_large |
| Absorptive (0.66–1.0) | absorptive_small | absorptive_mid | absorptive_large |

Each bus should contain a reverb effect appropriate for its distance and absorption characteristics (e.g., longer tails for reflective/large buses, shorter tails for absorptive/small buses). The Outdoor bus slot is available but not used in the demo project.

### Early Reflections (Wwise Reflect)

If using early reflections, create an aux bus with the **Wwise Reflect** effect inserted. Assign it in the plugin settings (see below).

## Project Settings

All settings are under **Project Settings > Plugins > Excited Waves Acoustics Wwise Integration**.

### Reverb Presets

The plugin uses an absorption-based preset system. Each preset maps to a set of aux buses (Close, Mid, Far, and optionally Outdoor).

| Setting | Default | Description |
|---------|---------|-------------|
| Reverb Presets | *(empty)* | Array of presets. Each entry defines `MaxAbsorption` and four aux bus references. Sort by MaxAbsorption ascending. |
| Preset Hysteresis Margin | 0.04 | Absorption must change by this amount beyond a preset boundary before switching. Prevents flicker. |
| Preset Crossfade Duration | 0.3 s | Crossfade time when transitioning between presets. 0 = instant switch. |
| Compensate Absorption | true | Reduce reverb send levels based on mean absorption (higher absorption = less reverb energy). |

#### Per-Preset Settings

| Setting | Default | Description |
|---------|---------|-------------|
| Max Absorption | 1.0 | Upper absorption boundary for this preset. |
| Close / Mid / Far / Outdoor Reverb Aux Bus | *(none)* | Wwise aux bus assets for each distance bin. |
| Send Volume Offset (dB) | 0.0 dB | Volume offset applied to all sends in this preset (-24 to +24 dB). Useful for balancing presets with different reverb tail lengths. |

### Reflect (Early Reflections)

| Setting | Default | Description |
|---------|---------|-------------|
| Early Reflections Bus | *(none)* | Aux bus with Wwise Reflect effect. Assign to enable early reflections. |
| Enable Reflections | true | Master toggle for early reflections. |
| Enable Spatial Radius Management | true | Merge nearby image sources to reduce processing. |
| Image Source Radius | 2 000 cm | Grouping radius for spatial management. |
| Position Match Threshold | 200 cm | Sources within this distance are merged into one. |
| Max Image Sources | 6 | Maximum image sources sent to Reflect per emitter. |
| Fade In Duration | 0.15 s | Fade-in time for new reflections. |
| Fade Out Duration | 0.3 s | Fade-out time for removed reflections. |
| Max Reflect Distance | 5 000 cm | Emitters beyond this distance from the listener skip reflections. 0 = no culling. |

### Reflect Debug

| Setting | Default | Description |
|---------|---------|-------------|
| Show Reflect Hit Points | false | Draw debug spheres at image source positions. |
| Reflect Hit Point Color | Cyan | Color for debug spheres. |

## Per-Detector Settings

Each `UAcousticsDetectorComponent` has Wwise-specific overrides:

| Property | Default | Description |
|----------|---------|-------------|
| Enable Reflections | true | Per-detector toggle for early reflections. |
| Reflect Distance Scaling Factor | 2.0 | Per-detector distance scaling override. |
| Reflect Level | 1.0 | Per-detector reflections volume. |
| Show Reflect Debug | false | Per-detector debug visualization. |

## How Reverb Sends Work

1. The core plugin produces per-bin amounts (Close, Mid, Far, Outdoor) and absorption values (weighted mean for compensation, RMS for preset selection).
2. This plugin selects the appropriate reverb preset based on RMS absorption (with hysteresis).
3. Bin amounts are mapped to send levels on the corresponding aux buses.
4. If absorption compensation is enabled, send levels are reduced proportionally to absorption.
5. Cross-room blending and distance attenuation (configured in the core plugin) further modulate the sends.
6. Sends are applied to all `UAkComponent` instances on the emitter actor via `SetAuxSends`.

When preset boundaries are crossed, the old and new presets are crossfaded over the configured duration.

## How Early Reflections Work

1. The core plugin calculates image source positions from ray hit data.
2. This plugin sends the image sources to Wwise Reflect as custom game data.
3. Reflect synthesizes early reflections based on the source positions, levels, and distance scaling.
4. Spatial radius management merges nearby sources and limits the total count.
5. New sources fade in and removed sources fade out over the configured durations.

## License

Copyright 2025-2026 Excited Waves. All Rights Reserved.

- Website: [excitedwaves.com](https://excitedwaves.com)
- Documentation: [excitedwaves.gitbook.io/excitedwaves-docs](https://excitedwaves.gitbook.io/excitedwaves-docs/)
- Licensing & Support: [contact@excitedwaves.com](mailto:contact@excitedwaves.com)

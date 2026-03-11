# Excited Waves Acoustics – Chaos Destruction

**Automatic acoustic recalculation when geometry is destroyed**

This plugin integrates Excited Waves Acoustics with Unreal Engine's Chaos destruction system. When a Geometry Collection breaks or pieces are removed, nearby acoustics detectors are automatically notified and re-scan the changed environment. No manual setup is required – the plugin discovers destructible objects and detectors on its own.

> **Trial Version.** This distribution is fully functional in Unreal Editor for evaluation and prototyping. Packaging a standalone game build requires a full license. For licensing inquiries, contact us at [contact@excitedwaves.com](mailto:contact@excitedwaves.com).

## Requirements

- Unreal Engine 5.6.1
- **Excited Waves Acoustics** plugin (core)

## Installation

1. Copy the `ExcitedWavesAcousticsChaos` folder into your project's `Plugins/` directory (alongside `ExcitedWavesAcoustics`).
2. Restart the Unreal Editor. The precompiled plugin will be loaded automatically.
3. The plugin will appear under **Edit > Plugins > Audio > Excited Waves Acoustics - Chaos Destruction**.

## How It Works

The plugin registers a `UDestructionAcousticsSubsystem` (world subsystem) that:

1. **Discovers** all Geometry Collection components in the level at world start.
2. **Subscribes** to Chaos break and removal events on each collection.
3. **Detects** destruction events, filters by magnitude threshold and spatial radius.
4. **Notifies** nearby `UAcousticsDetectorComponent` instances to re-run their detection cycle.

Everything happens automatically. Place Geometry Collections and Acoustics Detectors anywhere in the level – the subsystem handles the rest.

### Multi-Iteration Detection

After a destruction event, the acoustic environment may continue to change as debris settles. The plugin can run multiple detection iterations with a configurable interval, producing progressively more accurate results as the geometry stabilizes.

## Project Settings

All settings are under **Project Settings > Plugins > Excited Waves Acoustics Chaos**.

### Destruction Events

| Setting | Default | Description |
|---------|---------|-------------|
| Notification Radius | 5 000 cm | Radius around the destruction event in which detectors are notified. |
| Min Break Magnitude | 100 | Minimum break force (mass × velocity) required to trigger a notification. Filters out minor debris. |
| Per-Detector Cooldown | 0.5 s | Minimum time between notifications for the same detector. Prevents rapid-fire recalculations. |
| Recalculation Delay | 0.0 s | Delay before recalculation starts, giving geometry time to settle. |

### Scanning

| Setting | Default | Description |
|---------|---------|-------------|
| Auto-Scan Interval | 5.0 s | How often to scan for newly spawned Geometry Collections and detectors. Set to 0 to scan only at level start. |

### Multi-Iteration

| Setting | Default | Description |
|---------|---------|-------------|
| Multi-Iteration Detection | true | Run multiple detection passes after a destruction event. |
| Max Iterations | 3 | Number of detection passes per event. |
| Iteration Interval | 0.5 s | Delay between successive passes. |

### Removal Events

| Setting | Default | Description |
|---------|---------|-------------|
| Enable Removal Events | true | Also trigger detection when destroyed pieces are removed from the world. |
| Removal Event Cooldown | 0.2 s | Cooldown for removal-triggered notifications. |

## Blueprint API

The subsystem is accessible from Blueprints and C++:

```cpp
UDestructionAcousticsSubsystem* Sub =
    GetWorld()->GetSubsystem<UDestructionAcousticsSubsystem>();
```

| Function | Description |
|----------|-------------|
| `RegisterDetector(Detector)` | Manually register a detector (optional – auto-discovery works). |
| `UnregisterDetector(Detector)` | Remove a detector from tracking. |
| `ScanForGeometryCollections()` | Force an immediate scan for destructible objects. |
| `ScanForAcousticsDetectors()` | Force an immediate scan for detector components. |
| `GetTrackedGeometryCollectionCount()` | Number of tracked Geometry Collections. |
| `GetRegisteredDetectorCount()` | Number of registered detectors. |

## License

Copyright 2025-2026 Excited Waves. All Rights Reserved.

- Website: [excitedwaves.com](https://excitedwaves.com)
- Documentation: [excitedwaves.gitbook.io/excitedwaves-docs](https://excitedwaves.gitbook.io/excitedwaves-docs/)
- Licensing & Support: [contact@excitedwaves.com](mailto:contact@excitedwaves.com)

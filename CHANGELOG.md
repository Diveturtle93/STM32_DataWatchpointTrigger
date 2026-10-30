# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden hier dokumentiert.
Format nach [Keep a Changelog](https://keepachangelog.com/de/1.1.0/),
Versionierung nach [Semantic Versioning](https://semver.org/lang/de/).

## [Unveröffentlicht]

## [1.0.0] - 2026-10-30

### Hinzugefügt
- `DWT_Init()` schaltet die DWT-Einheit frei, setzt den Zykluszähler auf 0 und startet ihn. Auf dem Cortex-M7 entsperrt es zusätzlich die DWT-Register, damit der Zähler auch ohne Debugger läuft.
- `DWT_Enable()` und `DWT_Disable()` schalten die Trace-Einheiten frei bzw. sperren sie.
- `DWT_CycCounterEn()` und `DWT_CycCounterDis()` starten bzw. halten den Zykluszähler an.
- `DWT_CycCounterRead()` liest den Zählerstand aus, `DWT_CycCounterClear()` setzt ihn auf 0.
- Versionsnummer als Defines `DWT_MAJOR`, `DWT_MINOR`, `DWT_PATCH` und `DWT_DEV` in `dwt.h`.
- README mit API, Verwendung und unterstützten Cortex-M Kernen.

[Unveröffentlicht]: https://github.com/Diveturtle93/STM32_DataWatchpointTrigger/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/Diveturtle93/STM32_DataWatchpointTrigger/releases/tag/v1.0.0

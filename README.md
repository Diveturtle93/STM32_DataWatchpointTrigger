# STM32 Data Watchpoint Trigger (DWT)

Eine schlanke C-Bibliothek für STM32-Mikrocontroller, die den Zykluszähler der Data Watchpoint
and Trace Einheit (DWT) des ARM Cortex-M Kerns nutzt. Damit lassen sich Code-Abschnitte auf den
CPU-Takt genau messen, um Ausführungszeiten zu analysieren und zeitkritische Stellen zu finden.
Der Zugriff erfolgt direkt über die CMSIS-Register, ohne Peripherie oder HAL-Treiber.

## Beschreibung

Die DWT ist eine Debug- und Trace-Einheit, die in den Cortex-M3, -M4, -M7 und -M33 Kernen
enthalten ist. Ihr 32-Bit-Zykluszähler (`CYCCNT`) läuft mit dem CPU-Takt und zählt jeden
Taktzyklus. Wird der Zähler vor und nach einem Code-Abschnitt ausgelesen, ergibt die Differenz
die Ausführungszeit in Taktzyklen.

Damit der Zähler läuft, sind zwei Schritte nötig:

- Das Bit `TRCENA` im Register `CoreDebug->DEMCR` schaltet die Trace-Einheiten frei.
- Das Bit `CYCCNTENA` im Register `DWT->CTRL` startet den Zykluszähler.

`DWT_Init()` erledigt beides und setzt den Zähler vorher auf 0.

Der Name des Repositorys „Data Watchpoint Trigger“ meint dieselbe Einheit. ARM bezeichnet sie
offiziell als „Data Watchpoint and Trace“.

## Dateien

| Datei   | Beschreibung                                 |
|---------|----------------------------------------------|
| `dwt.h` | Versionsnummer, Funktionsdeklarationen       |
| `dwt.c` | Implementierung aller DWT-Steuerfunktionen   |

## Fehlerbilder

| Symptom | Ursache | Abhilfe |
|---------|---------|---------|
| `DWT_CycCounterRead()` liefert immer 0 | Kern ohne `CYCCNT` (Cortex-M0/M0+) | Kern im Abschnitt [Unterstützte Cortex-M Kerne](#unterstützte-cortex-m-kerne) prüfen |
| Zähler läuft nur mit angeschlossenem Debugger (Cortex-M7) | DWT-Register gesperrt, Lock Access Register nicht entsperrt | `DWT_Init()` statt der Einzelfunktionen aufrufen |
| Compilerfehler, `DWT` oder `CoreDebug` unbekannt | `main.h` bindet die CMSIS-Header des Mikrocontrollers nicht ein | Projekt mit CubeMX erzeugen oder den Geräteheader (z. B. `stm32f7xx.h`) einbinden |
| Gemessene Zeiten schwanken stark | Interrupts während der Messung werden mitgezählt | Mehrfach messen oder Interrupts während der Messung sperren |
| Umrechnung in µs stimmt nicht | Falscher CPU-Takt angenommen | Mit `SystemCoreClock` rechnen |

## API

```c
void DWT_Init (void);                   // DWT freischalten, Zähler nullsetzen und starten
void DWT_Enable (void);                 // Trace-Einheiten freischalten (TRCENA-Bit)
void DWT_Disable (void);                // Trace-Einheiten sperren
void DWT_CycCounterEn (void);           // Zykluszähler starten (CYCCNTENA-Bit)
void DWT_CycCounterDis (void);          // Zykluszähler anhalten
uint32_t DWT_CycCounterRead (void);     // Aktuellen Zählerstand auslesen
void DWT_CycCounterClear (void);        // Zähler auf 0 setzen
```

## Verwendung

### 1. Dateien einbinden

`dwt.h` und `dwt.c` in das STM32-Projekt kopieren und den Header einbinden:

```c
#include "dwt.h"
```

### 2. DWT initialisieren

Einmalig vor der Hauptschleife aufrufen:

```c
DWT_Init();
```

### 3. Ausführungszeit messen

```c
uint32_t start, stop, cycles;

// Zähler vor dem Abschnitt auslesen
start = DWT_CycCounterRead();

// Zu messender Code-Abschnitt
myFunction();

// Zähler nach dem Abschnitt auslesen
stop = DWT_CycCounterRead();

// Differenz ergibt die verbrauchten Taktzyklen
cycles = stop - start;
```

Die Zeit in Mikrosekunden ergibt sich aus:

```text
Zeit [µs] = cycles / CPU-Takt [MHz]
```

Beispiel bei 216 MHz: `1000 Zyklen / 216 ≈ 4,6 µs`. Der aktuelle CPU-Takt steht in der
CMSIS-Variable `SystemCoreClock` (in Hz).

### 4. Zähler zurücksetzen und anhalten

```c
DWT_CycCounterClear();                  // Zähler auf 0 setzen
DWT_CycCounterDis();                    // Zähler anhalten (optional)
```

## Hinweise

- Der Zähler läuft nach `0xFFFFFFFF` über und beginnt wieder bei 0, bei 216 MHz nach etwa
  20 Sekunden. Weil `stop - start` mit `uint32_t` gerechnet wird, stimmt die Differenz auch
  über einen Überlauf hinweg, solange die Messung kürzer als eine volle Umrundung ist.
- Auf dem Cortex-M7 sind die DWT-Register nach dem Reset gegen Schreibzugriffe gesperrt, solange
  kein Debugger angeschlossen ist. `DWT_Init()` schreibt dort den Schlüssel `0xC5ACCE55` in das
  Lock Access Register `DWT->LAR`. Die anderen Kerne haben dieses Register nicht.
- Der Zähler läuft in Hardware und belastet die CPU nicht. Er funktioniert auch ohne
  angeschlossenen Debugger und im Release-Build.
- Unterbrechungen (Interrupts) während der Messung werden mitgezählt.

## Unterstützte Cortex-M Kerne

Nicht jeder STM32 hat einen Zykluszähler. Vor der Verwendung im Referenzhandbuch des
Mikrocontrollers prüfen, ob `DWT->CYCCNT` vorhanden ist.

| Kern         | `CYCCNT` vorhanden | Beispiele        |
|--------------|--------------------|------------------|
| Cortex-M0    | Nein               | STM32F0          |
| Cortex-M0+   | Nein               | STM32G0, STM32L0 |
| Cortex-M3    | Ja                 | STM32F1, STM32F2 |
| Cortex-M4(F) | Ja                 | STM32F3, STM32F4 |
| Cortex-M7    | Ja                 | STM32F7, STM32H7 |
| Cortex-M33   | Ja                 | STM32L5, STM32U5 |

## Abhängigkeiten

- `main.h` – STM32 HAL, enthält die CMSIS-Definitionen für `CoreDebug` und `DWT`
- `stdint.h` – Datentyp `uint32_t`

## Lizenz

Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).

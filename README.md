# STM32 Data Watchpoint Trigger (DWT)
 
Eine schlanke C-Bibliothek für STM32-Mikrocontroller, die den integrierten Data Watchpoint
and Trace (DWT) Zähler des ARM Cortex-M Kerns nutzt. Mit dem DWT-Zykluszähler lassen sich
Code-Abschnitte präzise mit CPU-Taktzyklen messen, um Ausführungszeiten zu analysieren
und Timing-kritische Stellen im Programm zu identifizieren.
 
## Beschreibung
 
Der DWT ist eine Debug- und Trace-Einheit, die in den meisten ARM Cortex-M3, -M4, -M7 und
-M33 Kernen integriert ist. Der 32-Bit-Zykluszähler (`CYCCNT`) läuft mit der
CPU-Taktfrequenz und zählt jeden einzelnen Taktzyklus. Durch Auslesen des Zählers vor und
nach einem Code-Abschnitt lässt sich die Ausführungszeit in Taktzyklen exakt bestimmen.
 
**Wichtiger Hinweis:** Nicht jeder STM32 verfügt über einen DWT. Vor der Verwendung sollte
im Datenblatt des jeweiligen Mikrocontrollers geprüft werden, ob der DWT vorhanden ist.
STM32-Serien ohne Cortex-M3/M4/M7/M33-Kern (z. B. einige STM32G0-Varianten mit Cortex-M0+)
unterstützen den DWT nicht.
 
## Dateien
 
| Datei   | Beschreibung                                              |
|---------|-----------------------------------------------------------|
| `dwt.h` | Header mit Funktionsdeklarationen                         |
| `dwt.c` | Implementierung aller DWT-Steuerfunktionen                |
 
## API
 
```c
void DWT_Init(void);                  // DWT initialisieren, Zähler nullsetzen und einschalten
void DWT_Enable(void);                // DWT-Einheit im CoreDebug aktivieren (TRCENA-Bit)
void DWT_CycCounterEn(void);          // Zykluszähler starten (CYCCNTENA-Bit)
void DWT_CycCounterDis(void);         // Zykluszähler anhalten
uint32_t DWT_CycCounterRead(void);    // Aktuellen Zählerstand auslesen
void DWT_CycCounterClear(void);       // Zähler auf 0 zurücksetzen
```
 
## Verwendung
 
### 1. Dateien einbinden
 
`dwt.h` und `dwt.c` in das STM32-Projekt kopieren und den Header einbinden:
 
```c
#include "dwt.h"
```
 
### 2. DWT initialisieren
 
Einmalig vor der Hauptschleife aufrufen. `DWT_Init()` aktiviert die DWT-Einheit, setzt den
Zähler auf 0 und startet den Zählvorgang:
 
```c
DWT_Init();
```
 
### 3. Code-Ausführungszeit messen
 
```c
uint32_t start, stop, cycles;
 
// Zähler vor dem zu messenden Abschnitt auslesen
start = DWT_CycCounterRead();
 
// Zu messender Code-Abschnitt
myFunction();
 
// Zähler nach dem Abschnitt auslesen
stop = DWT_CycCounterRead();
 
// Differenz ergibt die Anzahl der verbrauchten Taktzyklen
cycles = stop - start;
```
 
Die benötigte Zeit in Mikrosekunden ergibt sich aus:
 
```
Zeit [µs] = cycles / (CPU-Takt [MHz])
```
 
Beispiel bei 216 MHz: `1000 Zyklen / 216 = ~4,6 µs`
 
### 4. Zähler zurücksetzen
 
```c
DWT_CycCounterClear();   // Zähler auf 0 setzen
DWT_CycCounterDis();     // Zähler anhalten (optional)
```
 
## Hinweise
 
- Der Zählerwert läuft nach `0xFFFFFFFF` (ca. 4,3 Milliarden Takte) über und beginnt wieder bei 0.
Bei einem 216 MHz Takt entspricht das einem Überlauf nach ca. 20 Sekunden. Für längere Messungen
muss der Überlauf berücksichtigt werden.
- `DWT_Init()` schreibt den ITM-Lock-Access-Register-Schlüssel (`0xC5ACCE55`), um die
Debug-Einheit zu entsperren. Dies ist auf manchen STM32-Serien notwendig, bevor auf DWT-Register
zugegriffen werden darf.
- Der DWT ist nur im Debug-Build sinnvoll aktiv. Im Release-Build kann `DEBUG_DWT` in `dwt.h`
auskommentiert bleiben, da kein Overhead entsteht – der Zähler läuft hardwareseitig und erfordert
lediglich einen einmaligen Lesevorgang.

## Unterstützte Cortex-M Kerne
 
| Kern         | DWT vorhanden |
|--------------|---------------|
| Cortex-M0    | Nein          |
| Cortex-M0+   | Nein          |
| Cortex-M3    | Ja            |
| Cortex-M4(F) | Ja            |
| Cortex-M7    | Ja            |
| Cortex-M33   | Ja            |
 
## Abhängigkeiten
 
- `main.h` – STM32 HAL (enthält die CMSIS-Definitionen für `CoreDebug`, `DWT`, `ITM`)

## Lizenz
 
Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).

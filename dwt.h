//----------------------------------------------------------------------
// Titel	:	dwt.h
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	12.06.2022
// Autor	:	Diveturtle93
// Projekt	:	Data Watchpoint Trigger
// Quelle	:
//----------------------------------------------------------------------

// Sicherheitssymbol
//----------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------

// Dateiheader definieren
//----------------------------------------------------------------------
#ifndef INC_DWT_H_
#define INC_DWT_H_
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------
#include <stdint.h>
//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der eigenen Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Definiere Debug-Symbole
//----------------------------------------------------------------------
#ifdef DEBUG
//	#define DEBUG_DWT
#endif
//----------------------------------------------------------------------

// Version definieren
//----------------------------------------------------------------------
#define DWT_MAJOR								1
#define DWT_MINOR								0
#define DWT_PATCH								0
#define DWT_DEV									0
//----------------------------------------------------------------------

// Funktionen definieren
//----------------------------------------------------------------------
void DWT_Init (void);														// DWT Zaehler initialisieren
void DWT_Enable (void);														// DWT Zaehler aktivieren
void DWT_Disable (void);													// DWT Zaehler deaktivieren
void DWT_CycCounterEn (void);												// DWT Zaehler einschalten
void DWT_CycCounterDis (void);												// DWT Zaehler ausschalten
uint32_t DWT_CycCounterRead (void);											// DWT Zaehler auslesen
void DWT_CycCounterClear (void);											// DWT Zaehler loeschen
//----------------------------------------------------------------------

#endif /* INC_DWT_H_ */
//----------------------------------------------------------------------

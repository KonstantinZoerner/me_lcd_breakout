
#include "Arduino.h"

#include "konstanten.h"

extern void neues_spiel_beginnen();
extern void neuer_level();
extern void ball_verlieren();

extern int anzahl_punkte;
extern int anzahl_baelle;
extern int aktueller_level;
extern int anzahl_steine;

extern byte stein_liste[ANZAHL_STEIN_REIHEN][STEINE_PRO_REIHE];

#include <Arduino.h>

#include "anzeige.h"

extern bool berechneSchlaegerPosition();

// Sachen aus spielablauf.cpp
//
extern void neues_spiel_beginnen();
extern void neuer_level();
extern int anzahl_punkte;
extern int anzahl_baelle;
extern int aktueller_level;
extern int anzahl_steine;

// Sachen aus me_lcd_...
extern void neuen_ball_starten();
extern void maleBallNeu();
extern void maleSchlaegerNeu();
extern void steineMalen();
extern void berechneBallPosition();
extern void pruefeBallGegenMauer();

void setup() {

neuen_ball_starten();

  anzeige_initialisieren();

  neues_spiel_beginnen();

  male_rand();

  male_punkte_leiste();
  male_punktstand(0);
  male_level_nr(aktueller_level);
  male_anzahl_baelle(3);

  maleSchlaegerNeu();
  steineMalen();

  delay(1000);
}

void loop() {
  berechneBallPosition();

  pruefeBallGegenMauer();
  maleBallNeu();

  if (anzahl_steine == 0) {
      neuer_level();
      steineMalen();
      male_level_nr(aktueller_level);

      neuen_ball_starten();
  }


  if (berechneSchlaegerPosition()) {;
        maleSchlaegerNeu();
  }
  delay(50);
}

#include <Arduino.h>

#include "anzeige.h"
#include "spielablauf.h"


extern bool berechneSchlaegerPosition();

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
  if (anzahl_steine == 0) {
      neuen_ball_starten();
      neuer_level();
      steineMalen();
      male_level_nr(aktueller_level);


  }
  berechneBallPosition();

  pruefeBallGegenMauer();
  maleBallNeu();




  if (berechneSchlaegerPosition()) {;
        maleSchlaegerNeu();
  }
  delay(50);
}

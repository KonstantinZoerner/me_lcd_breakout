#include <Arduino.h>

#include "anzeige.h"
#include "spielablauf.h"


extern bool berechneSchlaegerPosition();

#include "ball.h"

/**
Ardunino-Initialisierungsfunktion. Wird beim Start 1x aufgerufen.
*/
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

/**
Ardunino-Schleife. Wird immer und immer wieder aufgerufen.
*/
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
  if (ball_raus_ueberpruefen()){
    ball_platzen();
    ball_verlieren();
    neuen_ball_starten();
  }




  if (berechneSchlaegerPosition()) {;
        maleSchlaegerNeu();
  }
  delay(50);
}

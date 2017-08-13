#include <Arduino.h>

#include "anzeige.h"
#include "spielablauf.h"
#include "ball.h"
#include "schlaeger.h"

/** Arduino-Initialisierungsfunktion. Wird beim Start 1x aufgerufen.
*/
void setup() {

  anzeige_initialisieren();
  male_punkte_leiste();
  male_rand();

  neues_spiel_beginnen();
  male_level_nr(aktueller_level);
  male_anzahl_baelle(anzahl_baelle);

  maleSchlaegerNeu();
  steineMalen();
  neuen_ball_starten();

  delay(1000);
}

/** Arduino-Schleife. Wird immer und immer wieder aufgerufen.
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

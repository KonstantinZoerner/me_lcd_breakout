#include <Arduino.h>

#include "konstanten.h"


int anzahl_punkte   = 0;
int anzahl_baelle   = 0;
int aktueller_level = 0;
int anzahl_steine   = 0;


byte stein_liste[ANZAHL_STEIN_REIHEN][STEINE_PRO_REIHE];


void steine_initialisieren(int level) {
  anzahl_steine = 0;

  switch (level) {

    case 1:
      for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
        for (int j = 0; j < STEINE_PRO_REIHE; ++j) {
          stein_liste[i][j] = 1 + i;
          anzahl_steine++;
        }
      }
      break;

    case 2:
      for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
        for (int j = 0; j < STEINE_PRO_REIHE; ++j) {
          stein_liste[i][j] = 1 + (j%4);
          anzahl_steine++;
        }
      }
      break;

    default:
      for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
        for (int j = 0; j < STEINE_PRO_REIHE; ++j) {
          stein_liste[i][j] = 1;
          anzahl_steine++;
        }
      }
  }

}


void neues_spiel_beginnen() {
  anzahl_baelle   = 3;
  aktueller_level = 1;
  anzahl_punkte   = 0;

  steine_initialisieren(1);
}


void ball_verlieren() {
  anzahl_baelle--;
}


void neuer_level() {
  aktueller_level++;
  steine_initialisieren(aktueller_level);
}


bool ist_spiel_zuende() {
  return anzahl_baelle == 0;
}

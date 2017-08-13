#include <Arduino.h>

#include "konstanten.h"

int ball_pos_x;
int ball_pos_y;

int ball_pos_x_alt;
int ball_pos_y_alt;

int ball_rx;
int ball_ry;

// Sachen aus anzeige.cpp
//
extern void anzeige_initialisieren();
extern void buffer_schreiben();
extern void male_rechteck (int x, int y, int breite, int hoehe, int farbe);
extern void male_punkte_leiste();
extern void male_punktstand(int punkte);
extern void male_level_nr(int lvl);
extern void male_anzahl_baelle(int anzahl);
extern void male_rand();
extern char buffer[];

// Sachen aus schlaeger.cpp
//
extern int schlaeger_x;
extern int schlaeger_x_alt;
extern bool berechneSchlaegerPosition();

// Sachen aus spielablauf.cpp
//
extern void neues_spiel_beginnen();
extern void neuer_level();
extern byte stein_liste[ANZAHL_STEIN_REIHEN][STEINE_PRO_REIHE];
extern int anzahl_punkte;
extern int anzahl_baelle;
extern int aktueller_level;
extern int anzahl_steine;


void maleBallNeu() {
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIRF(%d,%d,%d,%d);", ball_pos_x_alt, ball_pos_y_alt, BALL_RADIUS, ball_pos_x, ball_pos_y, BALL_RADIUS, BALL_FARBE);
    buffer_schreiben();
}


void maleSchlaegerNeu() {
  sprintf(buffer, "BOXF(%d,%d,%d,%d,%d);BOXF(%d,%d,%d,%d,%d);",schlaeger_x_alt - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x_alt - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, 0, schlaeger_x - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, SCHLAEGER_FARBE);
  buffer_schreiben();
}


void punkte_addieren(int neue_punkte){
  anzahl_punkte += neue_punkte;
  male_punktstand(anzahl_punkte);
}


void steineMalen() {

  for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
    for (int j=0; j<STEINE_PRO_REIHE; ++j) {

        if (stein_liste[i][j] != 0) {
        male_rechteck(STEINE_START_X + j * (STEIN_BREITE + STEIN_ABSTAND), STEINE_START_Y + i * (STEIN_HOEHE + STEIN_ABSTAND), STEIN_BREITE, STEIN_HOEHE, stein_liste[i][j]);
        }
    }
  }
}

int schlaeger_ermitteln(){
  int linke_ecke = schlaeger_x - SCHLAEGER_BREITE / 2;
  int bereich_breite = SCHLAEGER_BREITE / 5;
  if(linke_ecke <= ball_pos_x && ball_pos_x < linke_ecke + bereich_breite){
    return 1;
  } else if(linke_ecke + bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 2 * bereich_breite){
    return 2;
  } else if(linke_ecke + 2*bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 3 * bereich_breite){
    return 3;
  }else if(linke_ecke + 3*bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 4 * bereich_breite){
    return 4;
  }else if(linke_ecke + 4*bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 5 * bereich_breite){
    return 5;
  }else{
    return 3;
  }
}

void ball_richtung_aendern(int bereich){
  if(bereich == 1){
    if (-6 < ball_rx and ball_rx < 9){
    ball_rx -= 2;
    ball_ry = -sqrt(GESCHWINDIGKEIT - ball_rx * ball_rx);
  }
}else if(bereich == 2){
  if (-6 < ball_rx and ball_rx < 9){
  ball_rx-- ;
  ball_ry = -sqrt(GESCHWINDIGKEIT - ball_rx * ball_rx);
}
}else if(bereich == 3){

  ball_rx = ball_rx;
  ball_ry = -ball_ry;

}else if(bereich == 4){
  if (-9 < ball_rx and ball_rx < 6){
  ball_rx++;
  ball_ry = -sqrt(GESCHWINDIGKEIT - ball_rx * ball_rx);
}
}else if(bereich == 5){
  if (-9 < ball_rx and ball_rx < 6){
  ball_rx += 2;
  ball_ry = -sqrt(GESCHWINDIGKEIT - ball_rx * ball_rx);
}
}else{
  ball_ry = -ball_ry;
}

}



void berechneBallPosition() {

  // Alte Position merken
  //
  ball_pos_x_alt = ball_pos_x;
  ball_pos_y_alt = ball_pos_y;

  // Abprallen am Schlaeger
  if (ball_ry > 0) {
    if (ball_pos_x + ball_rx + BALL_RADIUS > schlaeger_x - SCHLAEGER_BREITE/2 // Ball rechts vom linken Rand des Schlaegers
      && ball_pos_x + ball_rx - BALL_RADIUS < schlaeger_x + SCHLAEGER_BREITE/2 // Ball ist links vom rechten Rand
        && ball_pos_y + ball_ry + BALL_RADIUS > SCHLAEGER_POS_Y - 1// Ball ist unterhalb der oberen Schlaegergrenze
          && ball_pos_y + ball_ry - BALL_RADIUS < SCHLAEGER_POS_Y + SCHLAEGER_HOEHE - 1
      ) {
        int bereich = schlaeger_ermitteln();
      ball_richtung_aendern(bereich);

    }
  }

  // Bewegung X-Richtung
  //
  if (ball_rx > 0) {
     if (ball_pos_x + ball_rx < ANZEIGE_BREITE - (BALL_RADIUS + RAND_BREITE +1)) {
          ball_pos_x += ball_rx;
     } else {
          ball_rx = -ball_rx;
     }
  } else {
     if (ball_pos_x + ball_rx > (BALL_RADIUS + RAND_BREITE)) {
          ball_pos_x += ball_rx;
     } else {
          ball_rx = -ball_rx;
     }
  }

  // Bewegung in Y-Richtung
  //
  if (ball_ry > 0) {
     // Abprallen unten (TODO: verloren)
     if (ball_pos_y + ball_ry < ANZEIGE_HOEHE - (BALL_RADIUS + RAND_BREITE)) {
          ball_pos_y += ball_ry;
     } else {
          ball_ry = -ball_ry;
     }
  } else {
     // Abprallen an der Decke (TODO: verloren)
     if (ball_pos_y + ball_ry > (BALL_RADIUS + RAND_BREITE)) {
          ball_pos_y += ball_ry;
     } else {
          ball_ry = -ball_ry;
     }
  }
}



void steinLoeschen(int x, int y) {
  if (stein_liste[y][x] != 0) {
      stein_liste[y][x] = 0;
      male_rechteck(STEINE_START_X + x * (STEIN_BREITE + STEIN_ABSTAND),  STEINE_START_Y + y * (STEIN_HOEHE + STEIN_ABSTAND), STEIN_BREITE, STEIN_HOEHE, 0);
   }
}

int ermittelSteinFuerPixel (int x, int y) {
  if (y < STEINE_START_Y
        || y > STEINE_START_Y + (STEIN_HOEHE + STEIN_ABSTAND) * ANZAHL_STEIN_REIHEN -1) {
    return -1;
  }

  if (x < STEINE_START_X
        || x > STEINE_START_X + (STEIN_BREITE + STEIN_ABSTAND) * STEINE_PRO_REIHE -1) {
    return -1;
  }

  int spalte = (x - STEINE_START_X) / (STEIN_BREITE + STEIN_ABSTAND);
  int reihe = (y - STEINE_START_Y) / (STEIN_HOEHE + STEIN_ABSTAND);

  if (stein_liste[reihe][spalte] != 0) {
    return reihe * STEINE_PRO_REIHE + spalte;
  } else {
    return -1; // kein Stein an der der Stelle.
  }
}

void pruefeBallGegenMauer() {

  int getroffenerStein = -1;
  int trefferMuster = 0;

  // Teste links oben
  int stein = ermittelSteinFuerPixel(ball_pos_x + ball_rx - BALL_RADIUS, ball_pos_y + ball_ry - BALL_RADIUS);
  if (stein != -1) {
    getroffenerStein = stein;
    trefferMuster += 1;
  }

  // Teste rechts oben
  stein = ermittelSteinFuerPixel(ball_pos_x + ball_rx + BALL_RADIUS, ball_pos_y + ball_ry - BALL_RADIUS);
  if (stein != -1) {
    if (getroffenerStein == -1) {
      getroffenerStein = stein;
      trefferMuster += 2;
    } else {
      trefferMuster += 2;
    }
  }

  // Teste links unten
  stein = ermittelSteinFuerPixel(ball_pos_x + ball_rx - BALL_RADIUS, ball_pos_y + ball_ry + BALL_RADIUS);
  if (stein != -1) {
    if (getroffenerStein == -1) {
      getroffenerStein = stein;
      trefferMuster += 4;
    } else{
      trefferMuster += 4;
    }
  }

  // Teste rechts unten
  stein = ermittelSteinFuerPixel(ball_pos_x + ball_rx + BALL_RADIUS, ball_pos_y + ball_ry + BALL_RADIUS);
  if (stein != -1) {
    if (getroffenerStein == -1) {
      getroffenerStein = stein;
      trefferMuster += 8;
    } else {
      trefferMuster += 8;
    }
  }

  if (getroffenerStein != -1) {
    steinLoeschen(getroffenerStein % STEINE_PRO_REIHE, getroffenerStein / STEINE_PRO_REIHE);
    punkte_addieren(1);
    anzahl_steine--;


    switch(trefferMuster) {
      case 1: // links oben
        if(ball_ry < 0){
          if(ball_rx < 0){
            ball_rx = -ball_rx;
            ball_ry = -ball_ry;
          }else{
            ball_ry = -ball_ry;

          }
        } else {
          ball_rx = -ball_rx;

        }
        break;
      case 2: // rechts oben
        if(ball_ry < 0){
          if(ball_rx < 0){
            ball_ry = -ball_ry;
          }else{
            ball_ry = -ball_ry;
            ball_rx = -ball_rx;
          }
        } else {
          ball_rx = -ball_rx;
        }
        break;
      case 8: // links unten
        if(ball_ry > 0){
            if(ball_rx < 0){
              ball_ry = -ball_ry;
            }else{
              ball_ry = -ball_ry;
              ball_rx = -ball_rx;
            }
          } else {
            ball_rx = -ball_rx;

          }
          break;

      case 4: // rechts unten
        if(ball_ry > 0){
          if(ball_rx < 0){
            ball_rx = -ball_rx;
            ball_ry = -ball_ry;
          }else{
            ball_ry = -ball_ry;

          }
        } else {
          ball_rx = -ball_rx;

        }
        break;
      case 7:
      case 11:
      case 13:
      case 14:
          ball_rx = -ball_rx;
          ball_ry = -ball_ry;
          break;
      case 3:
      case 12:
          ball_ry = -ball_ry;
          break;
      case 5:
      case 10:
          ball_rx = -ball_rx;
          break;
      default:
          ball_rx = -ball_rx;
          ball_ry = -ball_ry;
          break;
    }
  }
}



void setup() {



  ball_pos_x = 20;
  ball_pos_y = 170;
  ball_rx = 6;
  ball_ry = 6;

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

      ball_pos_x = 20;
      ball_pos_y = 170;
      ball_pos_x_alt = 20;
      ball_pos_y_alt = 170;

      ball_rx = 6;
      ball_ry = 6;
  }


  if (berechneSchlaegerPosition()) {;
        maleSchlaegerNeu();
  }
  delay(50);
}

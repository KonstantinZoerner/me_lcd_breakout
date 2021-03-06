#include <Arduino.h>

#include "konstanten.h"
#include "anzeige.h"
#include "spielablauf.h"
#include "schlaeger.h"

int ball_pos_x;
int ball_pos_y;

int ball_pos_x_alt;
int ball_pos_y_alt;

int ball_rx;
int ball_ry;





void male_ball_neu() {
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIRF(%d,%d,%d,%d);", ball_pos_x_alt, ball_pos_y_alt, BALL_RADIUS, ball_pos_x, ball_pos_y, BALL_RADIUS, BALL_FARBE);
    buffer_schreiben();
}


void punkte_addieren(int neue_punkte){
  anzahl_punkte += neue_punkte;
  male_punktstand(anzahl_punkte);
}


void steine_malen() {

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
  int bereich_breite = SCHLAEGER_BREITE / 9;
  if (linke_ecke > ball_pos_x){
    return 1;
  } else if(linke_ecke <=ball_pos_x && ball_pos_x < linke_ecke + bereich_breite){
    return 1;
  } else if(linke_ecke + bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 2 * bereich_breite){
    return 2;
  } else if(linke_ecke + 2 * bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 3 * bereich_breite){
    return 3;
  } else if(linke_ecke + 3 * bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 4 * bereich_breite){
    return 4;
  } else if(linke_ecke + 4 * bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 5 * bereich_breite){
    return 5;
  } else if(linke_ecke + 5 * bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 6 * bereich_breite){
    return 6;
  } else if(linke_ecke + 6 * bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 7 * bereich_breite){
    return 7;
  } else if(linke_ecke + 7 * bereich_breite <= ball_pos_x && ball_pos_x < linke_ecke + 8 * bereich_breite){
    return 8;
  } else{
    return 9;
  }
}

void ball_richtung_aendern(int bereich){
  if(bereich == 1){
    ball_rx -= 4;
  }else if(bereich == 2){
    ball_rx-= 3 ;
  }else if(bereich == 3){
    ball_rx-= 2;
  }else if(bereich == 4){
    ball_rx-= 1;
  }else if(bereich == 5){
    ball_ry = -ball_ry;
  }else if(bereich == 6){
    ball_rx+= 1;
  }else if(bereich == 7){
    ball_rx+= 2;
  }else if(bereich == 8){
    ball_rx+= 3;
  }else if(bereich == 9){
    ball_rx+= 4;
  }
  if (ball_rx > MAX_BALL_RX){
    ball_rx = MAX_BALL_RX;
  }else if (ball_rx < -MAX_BALL_RX) {
    ball_rx = -MAX_BALL_RX;
    }

  ball_ry = -sqrt(GESCHWINDIGKEIT - ball_rx * ball_rx);

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

void neuen_ball_starten() {
  ball_pos_x_alt = ball_pos_x;
  ball_pos_y_alt = ball_pos_y;
  ball_pos_x = 20;
  ball_pos_y = 170;
  male_ball_neu();
  ball_rx = 6;
  ball_ry = 6;
  ball_pos_x_alt = 20;
  ball_pos_y_alt = 170;
}

bool ball_raus_ueberpruefen(){
    return (ball_pos_y - BALL_RADIUS > SCHLAEGER_POS_Y + SCHLAEGER_HOEHE);
  }

void ball_platzen(){
  for(int i = 1; i < 6; i++) {
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIR(%d,%d,%d,%d);", ball_pos_x, ball_pos_y, BALL_RADIUS + i - 1, ball_pos_x, ball_pos_y, BALL_RADIUS + i, BALL_FARBE);
    buffer_schreiben();
    delay(50);
  }
  sprintf(buffer, "CIRF(%d,%d,%d,0);", ball_pos_x, ball_pos_y, BALL_RADIUS + 5);
  buffer_schreiben();

}

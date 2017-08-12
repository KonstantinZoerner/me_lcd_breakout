#include <Arduino.h>

#include <SoftwareSerial.h>
#include <MeSerial.h>

#include "konstanten.h"

MeSerial serial(PORT_5);

int ball_pos_x;
int ball_pos_y;

int ball_pos_x_alt;
int ball_pos_y_alt;

int ball_rx;
int ball_ry;

// Sachen aus schlaeger.c
//
extern int schlaeger_x;
extern int schlaeger_x_alt;
extern bool berechneSchlaegerPosition();

int abgeprallt = 0;

char buffer[80];

byte stein_liste[ANZAHL_STEIN_REIHEN][STEINE_PRO_REIHE];


// Zustand des Spieles
int anzahl_punkte = 0;
int anzahl_baelle = 3;
int aktueller_level = 3;




void steineInitailisieren() {
  for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
    for (int j = 0; j < STEINE_PRO_REIHE; ++j) {
      stein_liste[i][j] = 1;
    }
  }
}

void maleBallNeu() {
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIRF(%d,%d,%d,%d);", ball_pos_x_alt, ball_pos_y_alt, BALL_RADIUS, ball_pos_x, ball_pos_y, BALL_RADIUS, BALL_FARBE);
    serial.println(buffer);
}

void maleSchlaegerNeu() {
  sprintf(buffer, "BOXF(%d,%d,%d,%d,%d);BOXF(%d,%d,%d,%d,%d);",schlaeger_x_alt - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x_alt - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, 0, schlaeger_x - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, SCHLAEGER_FARBE);
  serial.println(buffer);
}

void maleAnzeige() {

	// Anzahl Punkte und Level
	sprintf(buffer, "DS16(%d,%d,'Pts:%5d  Lvl:%2d  Bls: ',%d);", ANZEIGE_START_X, ANZEIGE_START_Y, anzahl_punkte, aktueller_level, SCHLAEGER_FARBE);
	serial.println(buffer);

    // Baelle
    for (int i = 0; i < MAX_BAELLE; ++i)
    {
    	sprintf(buffer, "CIRF(%d,%d,%d,%d);", ANZEIGE_START_X + 190 + i * (BALL_GROESSE+3), ANZEIGE_START_Y + BALL_RADIUS + 2, BALL_RADIUS, SCHLAEGER_FARBE);
    	serial.println(buffer);
    }
}
void punkte_addieren(int neue_punkte){
  anzahl_punkte += neue_punkte;
  maleAnzeige();
}

void berechneBallPosition() {

  // Alte Position merken
  //
  ball_pos_x_alt = ball_pos_x;
  ball_pos_y_alt = ball_pos_y;

  if (abgeprallt == 1 && ball_ry < 0){
    abgeprallt = 0;
  }

  // Abprallen am Schlaeger
  if (ball_ry > 0) {
    if (ball_pos_x + ball_rx + BALL_RADIUS > schlaeger_x - SCHLAEGER_BREITE/2 // Ball rechts vom linken Rand des Schlaegers
      && ball_pos_x + ball_rx - BALL_RADIUS < schlaeger_x + SCHLAEGER_BREITE/2 // Ball ist links vom rechten Rand
        && ball_pos_y + ball_ry + BALL_RADIUS > SCHLAEGER_POS_Y - 1 // Ball ist unterhalb der oberen Schlaegergrenze
          && ball_pos_y + ball_ry - BALL_RADIUS < SCHLAEGER_POS_Y - 1
      ) {
      ball_ry = -ball_ry;
      abgeprallt = 1;

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

void male_rechteck (int x, int y, int breite, int hoehe, int farbe) {
  char s[100];
  sprintf(s, "BOXF(%d,%d,%d,%d,%d);", x, y, x + breite, y + hoehe, farbe);

  serial.println(s);

}

void rand_malen(){
  male_rechteck(0, 0, RAND_BREITE, ANZEIGE_HOEHE, 6);
  male_rechteck(0, 0, ANZEIGE_BREITE, RAND_BREITE, 6);
  male_rechteck(ANZEIGE_BREITE - RAND_BREITE - 1, 0, RAND_BREITE, ANZEIGE_HOEHE, 6);

}

void steineMalen() {

  for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
    for (int j=0; j<STEINE_PRO_REIHE; ++j) {

        if (stein_liste[i][j] == 1) {
        male_rechteck(STEINE_START_X + j * (STEIN_BREITE + STEIN_ABSTAND), STEINE_START_Y + i * (STEIN_HOEHE + STEIN_ABSTAND), STEIN_BREITE, STEIN_HOEHE, 1);
        }
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
  if (y < STEINE_START_Y || y > STEINE_START_Y + (STEIN_HOEHE + STEIN_ABSTAND) * ANZAHL_STEIN_REIHEN) {
    return -1;
  }

  if (x < STEINE_START_X || x > STEINE_START_X + (STEIN_BREITE + STEIN_ABSTAND) * STEINE_PRO_REIHE) {
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
    punkte_addieren(20);


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
  ball_rx = 4;
  ball_ry = 4;

  serial.begin(9600);
  serial.print("DR1;");    // the screen displays in upright way
  serial.println("CLS(0);");
  serial.println("");
  serial.println("DS24(64,104,' ',5);");

  steineInitailisieren();

  maleAnzeige();

  rand_malen();
  maleSchlaegerNeu();
  steineMalen();


  delay(1000);



}


void loop() {


  berechneBallPosition();
  pruefeBallGegenMauer();
  maleBallNeu();
  /*
  if (abgeprallt == 1){
      male_rechteck(0,0,10,10,4);
  }else{
      male_rechteck(0,0,10,10,0);
  }
  */

  if (berechneSchlaegerPosition()) {;
        maleSchlaegerNeu();
  }

  delay(50);
}

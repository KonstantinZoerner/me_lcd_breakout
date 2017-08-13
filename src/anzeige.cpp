#include <Arduino.h>

#include <SoftwareSerial.h>
#include <MeSerial.h>

#include "konstanten.h"

MeSerial serial(PORT_5); // LCD-Anzeige an Port 5 angeschlossen
char buffer[80];

void anzeige_initialisieren() {
  serial.begin(9600);
  serial.print("DR1;");    // Anzeige hochkant
  serial.println("CLS(0);");
  serial.println("");
  serial.println("DS24(64,104,' ',5);");
}


/** Male ein gefuelltes Rechteck auf den Bildschirm.
*/
void male_rechteck (int x, int y, int breite, int hoehe, int farbe) {
  sprintf(buffer, "BOXF(%d,%d,%d,%d,%d);", x, y, x + breite, y + hoehe, farbe);
  serial.println(buffer);
}


void buffer_schreiben() {
  serial.println(buffer);
}


void male_punktstand(int punkte) {
  sprintf(buffer, "DS16(%d,%d,'%5d',%d);",
      ANZEIGE_START_X + 32, ANZEIGE_START_Y, punkte, SCHLAEGER_FARBE);
  serial.println(buffer);
}


void male_level_nr(int lvl) {
  sprintf(buffer, "DS16(%d,%d,'%2d',%d);",
      ANZEIGE_START_X + 120, ANZEIGE_START_Y, lvl, SCHLAEGER_FARBE);
  serial.println(buffer);
}


void male_anzahl_baelle(int anzahl) {
  for (int i = 0; i < MAX_BAELLE; ++i) {
    // Vorhandener Ball ist ausgefuellt
    if (anzahl > i) {
      sprintf(buffer, "CIRF(%d,%d,%d,%d);",
        ANZEIGE_START_X + 190 + i * (BALL_GROESSE+3), ANZEIGE_START_Y + BALL_RADIUS + 2, BALL_RADIUS, SCHLAEGER_FARBE);
    } else {
      sprintf(buffer, "CIR(%d,%d,%d,%d);",
        ANZEIGE_START_X + 190 + i * (BALL_GROESSE+3), ANZEIGE_START_Y + BALL_RADIUS + 2, BALL_RADIUS, SCHLAEGER_FARBE);
    }
    serial.println(buffer);
  }
}

void male_punkte_leiste() {
  sprintf(buffer, "DS16(%d,%d,'Pts:%5d  Lvl:%2d  Bls: ',%d);",
      ANZEIGE_START_X, ANZEIGE_START_Y, 0, 0, SCHLAEGER_FARBE);
  serial.println(buffer);

  male_punktstand(0);
  male_level_nr(0);
  male_anzahl_baelle(0);
}

void male_rand() {
  male_rechteck(0, 0, RAND_BREITE, ANZEIGE_HOEHE, 6);
  male_rechteck(0, 0, ANZEIGE_BREITE, RAND_BREITE, 6);
  male_rechteck(ANZEIGE_BREITE - RAND_BREITE - 1, 0, RAND_BREITE, ANZEIGE_HOEHE, 6);
}

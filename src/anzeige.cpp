#include <Arduino.h>

#include <SoftwareSerial.h>
#include <MeSerial.h>

#include "konstanten.h"

MeSerial serial(PORT_5);
char buffer[80];

void anzeige_initialisieren() {
  serial.begin(9600);
  serial.print("DR1;");    // Anzeige hochkant
  serial.println("CLS(0);");
  serial.println("");
  serial.println("DS24(64,104,' ',5);");
}

void male_rechteck (int x, int y, int breite, int hoehe, int farbe) {
  sprintf(buffer, "BOXF(%d,%d,%d,%d,%d);", x, y, x + breite, y + hoehe, farbe);
  serial.println(buffer);
}

void buffer_schreiben() {
  serial.println(buffer);
}


void male_punkte_leiste() {

  // Anzahl Punkte und Level
  sprintf(buffer, "DS16(%d,%d,'Pts:%5d  Lvl:%2d  Bls: ',%d);", ANZEIGE_START_X, ANZEIGE_START_Y, 0, 0, SCHLAEGER_FARBE);
  buffer_schreiben();

    // Baelle
    for (int i = 0; i < MAX_BAELLE; ++i)
    {
      sprintf(buffer, "CIRF(%d,%d,%d,%d);", ANZEIGE_START_X + 190 + i * (BALL_GROESSE+3), ANZEIGE_START_Y + BALL_RADIUS + 2, BALL_RADIUS, SCHLAEGER_FARBE);
      buffer_schreiben();
    }
}


void male_punktstand(int punkte) {
  sprintf(buffer, "DS16(%d,%d,'%5d',%d);", ANZEIGE_START_X + 32, ANZEIGE_START_Y, punkte, SCHLAEGER_FARBE);
  serial.println(buffer);
}


void male_level_nr(int level) {
  sprintf(buffer, "DS16(%d,%d,'%2d',%d);", ANZEIGE_START_X + 64, ANZEIGE_START_Y, level, SCHLAEGER_FARBE);
  serial.println(buffer);
}


void male_rand() {
  male_rechteck(0, 0, RAND_BREITE, ANZEIGE_HOEHE, 6);
  male_rechteck(0, 0, ANZEIGE_BREITE, RAND_BREITE, 6);
  male_rechteck(ANZEIGE_BREITE - RAND_BREITE - 1, 0, RAND_BREITE, ANZEIGE_HOEHE, 6);
}

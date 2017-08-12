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

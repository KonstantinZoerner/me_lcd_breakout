#include <MeAuriga.h>

#include "konstanten.h"
#include "anzeige.h"

int schlaeger_x = 100;
int schlaeger_x_alt = 100;

MeJoystick joystick(6);

bool berechneSchlaegerPosition() {

  int joy_x = joystick.readX();

  // Alte Position merken
  schlaeger_x_alt = schlaeger_x;
  schlaeger_x = (joy_x + 500) / 4;

  if (schlaeger_x - SCHLAEGER_BREITE/2 < RAND_BREITE + 1) {
    schlaeger_x = RAND_BREITE + 1 + SCHLAEGER_BREITE/2;
  } else if (schlaeger_x - SCHLAEGER_BREITE/2 > ANZEIGE_BREITE - RAND_BREITE - 2 - SCHLAEGER_BREITE) {
    schlaeger_x = ANZEIGE_BREITE - RAND_BREITE - 2 - SCHLAEGER_BREITE + SCHLAEGER_BREITE/2;
  }

  return schlaeger_x != schlaeger_x_alt;
}

void maleSchlaegerNeu() {
  sprintf(buffer, "BOXF(%d,%d,%d,%d,%d);BOXF(%d,%d,%d,%d,%d);",schlaeger_x_alt - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x_alt - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, 0, schlaeger_x - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, SCHLAEGER_FARBE);
  buffer_schreiben();
}

bool joystick_gezogen() {
  return joystick.readY() > 100;
}

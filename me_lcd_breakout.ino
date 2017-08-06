#include <MeAuriga.h>
#include <SoftwareSerial.h>

MeSerial serial(PORT_5);
MeJoystick joystick(6);

#define ANZEIGE_BREITE 240
#define ANZEIGE_HOEHE  320
#define RAND_BREITE 2

#define BALL_GROESSE 10
#define BALL_RADIUS  5
#define BALL_FARBE   2

#define SCHLAEGER_HOEHE 7
#define SCHLAEGER_BREITE 50
#define SCHLAEGER_POS_Y 270
#define SCHLAEGER_FARBE 4

#define STEIN_BREITE 25
#define STEIN_HOEHE 10
#define STEIN_ABSTAND 3


#define STEINE_PRO_REIHE    8
#define ANZAHL_STEIN_REIHEN 6
#define STEINE_START_Y 40


int ball_pos_x;
int ball_pos_y;

int ball_pos_x_alt;
int ball_pos_y_alt;

int ball_rx;
int ball_ry;

int schlaeger_x = 100;
int schlaeger_x_alt = 100;

int joy_x;

char buffer[80];

byte stein_liste[ANZAHL_STEIN_REIHEN][STEINE_PRO_REIHE];

void steineInitailisieren() {
  for (int i=0; i<ANZAHL_STEIN_REIHEN; ++i) {
    for (int j = 0; j < STEINE_PRO_REIHE; ++j) {
      stein_liste[i][j] = 1;
    }
  }
}



void leseJoystickAus() {
	joy_x = joystick.read(1);
}

void maleJoystickPosition() {
  sprintf(buffer, "DS24(64,104,'joy_x=        ',5)");
  serial.println(buffer); 
  
	sprintf(buffer, "DS24(64,104,'joy_x=%d',5)", joy_x);
	serial.println(buffer); 
}

void maleBallNeu() {
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIRF(%d,%d,%d,%d);", ball_pos_x_alt, ball_pos_y_alt, BALL_RADIUS, ball_pos_x, ball_pos_y, BALL_RADIUS, BALL_FARBE);
    serial.println(buffer); 
}

void maleSchlaegerNeu() {
    
  sprintf(buffer, "BOXF(%d,%d,%d,%d,%d);BOXF(%d,%d,%d,%d,%d);",schlaeger_x_alt - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x_alt - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, 0, schlaeger_x - SCHLAEGER_BREITE/2, SCHLAEGER_POS_Y, schlaeger_x - SCHLAEGER_BREITE/2 + SCHLAEGER_BREITE, SCHLAEGER_POS_Y + SCHLAEGER_HOEHE, SCHLAEGER_FARBE);
  
  serial.println(buffer);  
}

void berechneBallPosition() {
  
  // Alte Position merken
  //
  ball_pos_x_alt = ball_pos_x;
  ball_pos_y_alt = ball_pos_y;

  // Abprallen am Schlaeger
  if (ball_ry > 0) {
    if (ball_pos_x + BALL_RADIUS > schlaeger_x - SCHLAEGER_BREITE/2 // Ball rechts vom linken Rand des Schlaegers
      && ball_pos_x - BALL_RADIUS < schlaeger_x + SCHLAEGER_BREITE/2 // Ball ist links vom rechten Rand
        && ball_pos_y + BALL_RADIUS > SCHLAEGER_POS_Y - 1 // Ball ist unterhalb der oberen Schlaegergrenze
          && ball_pos_y - BALL_RADIUS < SCHLAEGER_POS_Y - 1
      ) {
      ball_ry = -ball_ry;
    }
  }  


  // Bewegung X-Richtung
  //
  if (ball_rx > 0) {
     if (ball_pos_x + ball_rx < ANZEIGE_BREITE - (BALL_RADIUS + RAND_BREITE)) {
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

void berechneSchlaegerPosition() {
  
  // Alte Position merken
  schlaeger_x_alt = schlaeger_x;
  schlaeger_x = (joy_x + 500) / 4;

  if (schlaeger_x - SCHLAEGER_BREITE/2 < RAND_BREITE + 1) {
    schlaeger_x = RAND_BREITE + 1 + SCHLAEGER_BREITE/2;
  } else if (schlaeger_x - SCHLAEGER_BREITE/2 > ANZEIGE_BREITE - RAND_BREITE - 2 - SCHLAEGER_BREITE) {
    schlaeger_x = ANZEIGE_BREITE - RAND_BREITE - 2 - SCHLAEGER_BREITE + SCHLAEGER_BREITE/2;
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
        male_rechteck(9 + j * (STEIN_BREITE + STEIN_ABSTAND), STEINE_START_Y + i * (STEIN_HOEHE + STEIN_ABSTAND), STEIN_BREITE, STEIN_HOEHE, 1);              
        }


    }      
  }


}

void steinLoeschen(int x, int y) {
  if (stein_liste[x][y] != 0) {
      stein_liste[x][y] = 0;
      male_rechteck(9 + x * (STEIN_BREITE + STEIN_ABSTAND),  STEINE_START_Y + y * (STEIN_HOEHE + STEIN_ABSTAND), STEIN_BREITE, STEIN_HOEHE, 0);
  }
}

int ermittelSteinFuerPixel (int x, int y) {
  if (y < STEINE_START_Y || y > STEINE_START_Y + (STEIN_HOEHE + STEIN_ABSTAND) * ANZAHL_STEIN_REIHEN) {
    return -1;
  }

  int spalte = x / (STEIN_BREITE + STEIN_ABSTAND);
  int reihe = (y - STEINE_START_Y) / (STEIN_HOEHE + STEIN_ABSTAND);

  return reihe * STEINE_PRO_REIHE + spalte;
}


void setup() {

  ball_pos_x = 150;
  ball_pos_y = 150;
  ball_rx = 4;
  ball_ry = 4;
  
  serial.begin(9600);
  serial.print("DR1;");    // the screen displays in upright way
  serial.println("CLS(0);");
  serial.println("");
  serial.println("DS24(64,104,' ',5);"); 

  steineInitailisieren();

  rand_malen();
  maleSchlaegerNeu();
  steineMalen();


  delay(1000); 



}


void loop() {
  
  berechneBallPosition();
  maleBallNeu();

  leseJoystickAus(); 
  berechneSchlaegerPosition();
  if (schlaeger_x_alt != schlaeger_x) {
        maleSchlaegerNeu();    
  }

  int stein = ermittelSteinFuerPixel(ball_pos_x - BALL_RADIUS, ball_pos_y - BALL_RADIUS);
  if (stein != -1) {
    steinLoeschen(stein % STEINE_PRO_REIHE, stein / STEINE_PRO_REIHE);
  }
  stein = ermittelSteinFuerPixel(ball_pos_x + BALL_RADIUS, ball_pos_y - BALL_RADIUS);
  if (stein != -1) {
    steinLoeschen(stein % STEINE_PRO_REIHE, stein / STEINE_PRO_REIHE);
  }
  stein = ermittelSteinFuerPixel(ball_pos_x - BALL_RADIUS, ball_pos_y + BALL_RADIUS);
  if (stein != -1) {
    steinLoeschen(stein % STEINE_PRO_REIHE, stein / STEINE_PRO_REIHE);
  }
  stein = ermittelSteinFuerPixel(ball_pos_x + BALL_RADIUS, ball_pos_y + BALL_RADIUS);
  if (stein != -1) {
    steinLoeschen(stein % STEINE_PRO_REIHE, stein / STEINE_PRO_REIHE);
  }


  
  delay(50);
}

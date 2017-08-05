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
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIRF(%d,%d,%d,%d);", ball_pos_x_alt, ball_pos_y_alt, BALL_GROESSE / 2, ball_pos_x, ball_pos_y, BALL_RADIUS, BALL_FARBE);
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

  rand_malen();
  maleSchlaegerNeu();

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
  
  delay(50);
}

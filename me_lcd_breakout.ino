#include <MeAuriga.h>
#include <SoftwareSerial.h>

MeSerial serial(PORT_5);

#define ANZEIGE_BREITE 240
#define ANZEIGE_HOEHE  320
#define RAND_BREITE 2

#define BALL_GROESSE 10
#define BALL_RADIUS  5
#define BALL_FARBE   2

int ball_pos_x;
int ball_pos_y;

int ball_pos_x_alt;
int ball_pos_y_alt;

int ball_rx;
int ball_ry;

char buffer[80];

void maleBallNeu() {
    sprintf(buffer, "CIRF(%d,%d,%d,0);CIRF(%d,%d,%d,%d);", ball_pos_x_alt, ball_pos_y_alt, BALL_GROESSE / 2, ball_pos_x, ball_pos_y, BALL_RADIUS, BALL_FARBE);
    serial.println(buffer); 
}

void berechneBallPosition() {

  // Alte Position merken
  //
  ball_pos_x_alt = ball_pos_x;
  ball_pos_y_alt = ball_pos_y;

  // X-Richtung
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
  
  // Y-Richtung
  //
  if (ball_ry > 0) {
     if (ball_pos_y + ball_ry < ANZEIGE_HOEHE - (BALL_RADIUS + RAND_BREITE)) {
          ball_pos_y += ball_ry;
     } else {
          ball_ry = -ball_ry;
     }
  } else {
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

void setup() {

  ball_pos_x = 150;
  ball_pos_y = 150;
  ball_rx = 4;
  ball_ry = 4;
  
  serial.begin(9600);
  serial.print("DR1;");    // the screen displays in upright way
  serial.println("CLS(0);");
  serial.println("");
  serial.println("DS24(64,104,'Hallo!',5);"); 
  rand_malen();



  delay(1000); 
}


void loop() {
  
  berechneBallPosition();
  maleBallNeu();
  
  delay(50);
}

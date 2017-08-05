#include <MeAuriga.h>
#include <SoftwareSerial.h>

MeSerial mySerial(PORT_5);

#define ANZEIGE_BREITE 240
#define ANZEIGE_HOEHE  320
#define RAND_BREITE 3

#define BALL_GROESSE 10
#define BALL_FARBE   2

int ball_pos_x;
int ball_pos_y;

int ball_pos_x_alt;
int ball_pos_y_alt;

char buffer[80];

void maleBallNeu() {
    sprintf(buffer, "CIRF(%d,%d,%d,0);", ball_pos_x_alt, ball_pos_y_alt, BALL_GROESSE / 2);
    mySerial.println(buffer);
    sprintf(buffer, "CIRF(%d,%d,%d,%d);", ball_pos_x, ball_pos_y, BALL_GROESSE / 2, BALL_FARBE);
    mySerial.println(buffer); 
}

void berechneBallPosition() {

  // Alte Position merken
  //
  ball_pos_x_alt = ball_pos_x;
  ball_pos_y_alt = ball_pos_y;


  
}


void male_rechteck (int x, int y, int breite, int hoehe, int farbe) {
  char s[100];
  sprintf(s, "BOXF(%d,%d,%d,%d,%d);", x, y, x + breite, y + hoehe, farbe);
  
  mySerial.println(s);  

}

void rand_malen(){
  male_rechteck(0, 0, RAND_BREITE, ANZEIGE_HOEHE, 7);
  male_rechteck(0, 0, ANZEIGE_BREITE, RAND_BREITE, 2);
  male_rechteck(ANZEIGE_BREITE - RAND_BREITE - 1, 0, RAND_BREITE, ANZEIGE_HOEHE, 4);
  
}

void setup() {

  ball_pos_x = 150;
  ball_pos_y = 150;
  
  mySerial.begin(9600);
  mySerial.print("DR1;");    // the screen displays in upright way
  mySerial.println("CLS(0);");
  mySerial.println("");
  mySerial.println("DS24(64,104,'Hallo!',5);"); 


  delay(1000); 
}


void loop() {
  rand_malen();
  maleBallNeu();
  delay(100);
}

#include <MeAuriga.h>
#include <SoftwareSerial.h>

MeSerial mySerial(PORT_5);

#define ANZEIGE_BREITE = 240
#define ANZEIGE_HOEHE  = 320

void male_rechteck (int x, int y, int breite, int hoehe, int farbe) {
      mySerial.println("BOXF(x,y,y + breite,y + hoehe, farbe);");  

}

void rand_malen(){
  male_rechteck(10, 10, 20, 30, 2);
}

void setup() {
  mySerial.begin(9600);
  mySerial.print("DR1;");    // the screen displays in upright way
  mySerial.println("CLS(0);");
  mySerial.println("");
  mySerial.println("DS24(64,104,'Hallo!',5);"); 


  delay(1000); 
}


void loop() {
  rand_malen();
}

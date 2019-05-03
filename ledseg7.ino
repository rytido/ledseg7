//sudo chmod -R 777 /dev/ttyUSB0
#include "LedControl.h"
#include "ESP8266WiFi.h"
#include "config.h"

/*
12/D7 - DIN
13/D5 - CLK/SCLK
10/D8 - CS/LOAD
We have 1 MAX72XX
*/
LedControl lc=LedControl(D7,D5,D8,1);

void setup() {
  Serial.begin(9600);
  /* MAX72XX wakeup call*/
  lc.shutdown(0,false);
  /* Set the brightness*/
  lc.setIntensity(0,0);
  /* and clear the display */
  lc.clearDisplay(0);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, wifipw);
  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 7; i > 0; i--) {
      lc.setChar(0, i, ' ', true);
      delay(99);
      lc.setChar(0, i, ' ', false);
    }
  }
}

void setvalue(long i){
  lc.setDigit(0,0,i%10,false);
  lc.setDigit(0,1,i<10 ? ' ' : (i/10)%10,false);
  lc.setDigit(0,2,i<100 ? ' ' : (i/100)%10,false);
  lc.setDigit(0,3,i<1000 ? ' ' : (i/1000)%10,false);
  lc.setDigit(0,4,i<10000 ? ' ' : (i/10000)%10,false);
  lc.setDigit(0,5,i<100000 ? ' ' : (i/100000)%10,false);
  lc.setDigit(0,6,i<1000000 ? ' ' : (i/1000000)%10,false);
  lc.setDigit(0,7,i<10000000 ? ' ' : (i/10000000)%10,false);
  delay(500);
}

long intval=1;
void loop() {
  setvalue(intval);
  intval = intval + 1;
}

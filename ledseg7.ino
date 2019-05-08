//ESP8266 ESP-12E MAX7219
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
WiFiClient client;

const char bytes[] = {
  0xf9,0xbe,0xb4,0xd9,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x00,0x7b,0x0d,
  0xf4,0xa3,0x7f,0x11,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5c,0x80,0xcd,0x5c,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x32,0x37,0x2e,0x30,0x2e,0x30,0x2e,0x31,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x20,0x8d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x32,0x37,0x2e,0x30,0x2e,0x30,0x2e,0x31,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x8d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xa1,0x07,0x00
};

void setup() {
  Serial.begin(9600);
  lc.shutdown(0,false); // MAX72XX wakeup call
  lc.setIntensity(0,0); // Set the brightness
  lc.clearDisplay(0); // Clear the display

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, wifipw);
  while (WiFi.status() != WL_CONNECTED) {
    loading();
  }
  //WiFi.localIP()
}

void loading(){
  for (int i = 7; i >= 0; i--) {
    lc.setChar(0, i, ' ', true);
    delay(99);
    lc.setChar(0, i, ' ', false);
  }
}

void failure(){
  lc.clearDisplay(0);
  lc.setChar(0,4,'L',false);
  lc.setDigit(0,5,1,false);
  lc.setChar(0,6,'A',false);
  lc.setChar(0,7,'F',false);
  delay(30000);
}

void blocks(){
  //'A','b','c','d','E','F','H','L','P'
  //r lc.setRow(0,0,0x05);
  //u lc.setRow(0,0,0x1c);
  //0 decimal top topright topleft bottom bottomleft topleft middle
  //middle topleft bottomleft bottom bottomright topright top decimal
  lc.clearDisplay(0);
  lc.setRow(0,2,B01011011); //S
  lc.setRow(0,3,B00001000); //bottom (underscore)
  lc.setChar(0,4,'c',false);
  lc.setRow(0,5,0x1D); //o
  lc.setChar(0,6,'L',false);
  lc.setChar(0,7,'B',false);
}

void halving(){
  lc.clearDisplay(0);
  lc.setRow(0,1,B01111011);
  lc.setRow(0,2,0x15); //n
  lc.setRow(0,3,B00010000); //i
  lc.setChar(0,4,'F',false);
  lc.setChar(0,5,'L',false);
  lc.setChar(0,6,'A',false);
  lc.setChar(0,7,'H',false);
}

void setvalue(long i){
  lc.setDigit(0,0,i%10,false);
  if (i<10) {lc.setChar(0,1,' ',false);} else {lc.setDigit(0,1,(i/10)%10,false);}
  if (i<100) {lc.setChar(0,2,' ',false);} else {lc.setDigit(0,2,(i/100)%10,false);}
  if (i<1000) {lc.setChar(0,3,' ',false);} else {lc.setDigit(0,3,(i/1000)%10,false);}
  if (i<10000) {lc.setChar(0,4,' ',false);} else {lc.setDigit(0,4,(i/10000)%10,false);}
  if (i<100000) {lc.setChar(0,5,' ',false);} else {lc.setDigit(0,5,(i/100000)%10,false);}
  if (i<1000000) {lc.setChar(0,6,' ',false);} else {lc.setDigit(0,6,(i/1000000)%10,false);}
  if (i<10000000) {lc.setChar(0,7,' ',false);} else {lc.setDigit(0,7,(i/10000000)%10,false);}
  delay(200);
}

void loop() {

  client.connect(peer_ip, 8333);

  if (client.connected()) {
    client.write(bytes, sizeof(bytes));
    loading();
    loading();
    
    int height = 0;
    if (client.available()){
      for (int i = 0; i < 125; i += 1) {
        int j = (unsigned char) client.read();
        if (i > 120) {
          height = height + j * pow(256, i - 121);
        }
        delay(5);
      }
    }
    client.stop();
    if (height > 0){
      for (int i = 0; i < 3; i += 1) {
        blocks();
        delay(2000);
        setvalue(height);
        delay(8000);
        halving();
        delay(2000);
        setvalue(210000 - height % 210000);
        delay(8000);
      }
    } else {
      failure();
    }
  } else {
    failure();
  }
}

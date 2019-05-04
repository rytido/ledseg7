//ESP8266 ESP-12E MAX7219
//sudo chmod -R 777 /dev/ttyUSB0
#include "LedControl.h"
#include "ESP8266WiFi.h"
#include "config.h"
#include "WebSocketsClient.h"

/*
12/D7 - DIN
13/D5 - CLK/SCLK
10/D8 - CS/LOAD
We have 1 MAX72XX
*/
LedControl lc=LedControl(D7,D5,D8,1);
WebSocketsClient sock;
char version_hex[] = "f9beb4d976657273696f6e0000000000550000007b0df4a37f11010000000000000000005c80cd5c0000000000000000000000003132372e302e302e3100000000000000208d00000000000000003132372e302e302e3100000000000000208d00000000000000000020a10700";

void webSocketEvent(WStype_t type, uint8_t * payload, size_t len) {

    switch(type) {
        case WStype_DISCONNECTED:
            setvalue(10);
            break;
        case WStype_CONNECTED:
            {
                setvalue(11);
            }
            break;
        case WStype_TEXT:
           setvalue(12);

            // send data to back to Server
            //sock.sendTXT(payload, len);
            break;
        case WStype_BIN:
            setvalue(13);
            hexdump(payload, len);

            // echo data back to Server
            //sock.sendBIN(payload, lenght);
            break;

        case WStype_ERROR:
            setvalue(99);
            break;

        default:
            break;
    }
}

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
  //WiFi.localIP()
  sock.begin(peer_ip, 8333);
  sock.onEvent(webSocketEvent);
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
  // sock.sendBIN(payload, length);
}

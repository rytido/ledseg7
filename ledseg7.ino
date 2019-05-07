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
char const *hex = "f9beb4d976657273696f6e0000000000550000007b0df4a37f11010000000000000000005c80cd5c0000000000000000000000003132372e302e302e3100000000000000208d00000000000000003132372e302e302e3100000000000000208d00000000000000000020a10700";

uint8_t* hex_str_to_uint8(const char* string) {

    if (string == NULL)
        return NULL;

    size_t slength = strlen(string);
    if ((slength % 2) != 0) // must be even
        return NULL;

    size_t dlength = slength / 2;

    uint8_t* data = (uint8_t*)malloc(dlength);

    memset(data, 0, dlength);

    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if (c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F')
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
            value = (10 + (c - 'a'));
        else
            return NULL;

        data[(index / 2)] += value << (((index + 1) % 2) * 4);

        index++;
    }

    return data;
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

  client.connect(peer_ip, 8333);

  if (client.connected()) {
    setvalue(1);
  }

  char bytes[109];
  for (int i = 0; i < 109; i += 1) {
    char test[2] = {hex[i*2], hex[i*2+1]};
    bytes[i] = (char) strtol(test, NULL, 16);
  }

  client.print(bytes);

  while (client.connected() && !client.available()) {
    delay(100);
  }
  //uint8_t* bytes = hex_str_to_uint8(hex);

  char buf[4];
  for (int i = 0; i < 125; i += 1) {
    int j = (unsigned char) client.read();
    setvalue(i*10000 + j);
    /*if (i > 120) {
      buf[i - 121] = client.read();
      setvalue(i);
    } else {
      client.read();
    }*/
  }
/*
  int a = int((unsigned char)(buf[0]) << 24 |
              (unsigned char)(buf[1]) << 16 |
              (unsigned char)(buf[2]) << 8 |
              (unsigned char)(buf[3]));
  */
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
  //setvalue(intval);
  //intval = intval + 1;
  delay(1000);
}

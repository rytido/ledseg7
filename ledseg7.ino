//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 13 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,13,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=200;

void setup() {
  Serial.begin(9600);
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,0);
  /* and clear the display */
  lc.clearDisplay(0);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  lc.setRow(0,0,0x05);
  delay(delaytime);
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  lc.setRow(0,0,0x1c);
  delay(delaytime);
  lc.setRow(0,0,B00010000);
  delay(delaytime);
  lc.setRow(0,0,0x15);
  delay(delaytime);
  lc.setRow(0,0,0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
for(int i=0;i<13;i++) {
lc.setDigit(0,7,i,false);
lc.setDigit(0,6,i+1,false);
lc.setDigit(0,5,i+2,false);
lc.setDigit(0,4,i+3,false);
lc.setDigit(0,3,i+4,false);
lc.setDigit(0,2,i+5,false);
lc.setDigit(0,1,i+6,false);
lc.setDigit(0,0,i+7,false);
delay(delaytime);
}
lc.clearDisplay(0);
delay(delaytime);
}

void increment(){
  for(int i=0;i<10000;i++){
    lc.setDigit(0,0,i%10,false);
    lc.setDigit(0,1,(i/10)%10,false);
    lc.setDigit(0,2,(i/100)%10,false);
    delay(delaytime);
  }
}
/*
 for(int d=0;d<8;d++){
    lc.setDigit(0,d,((int)(i/pow(10,d)))%10,false);  
  }
 */

void setvalue(long i){
  lc.setDigit(0,0,i%10,false);
  lc.setDigit(0,1,(i/10)%10,false);
  lc.setDigit(0,2,(i/100)%10,false);
  lc.setDigit(0,3,(i/1000)%10,false);
  lc.setDigit(0,4,(i/10000)%10,false);
  lc.setDigit(0,5,(i/100000)%10,false);
  lc.setDigit(0,6,(i/1000000)%10,false);
  lc.setDigit(0,7,(i/10000000)%10,false);
  delay(1000);
}

long integerValue=0;  // Max value is 65535
char incomingByte;

void loop() {
  if (Serial.available() > 0) {   // something came across serial
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      integerValue *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);
    }
    Serial.println(integerValue);   // Do something with the value
  }
  setvalue(integerValue);
}

#include <BasicTerm.h>
#include <Wire.h>
#include <RDA1846.h>

RDA1846 rda;

#define LOW_FRQ 0x1000
#define HIGH_FRQ 0x5800

byte getVal(char c)
{
 if(c >= '0' && c <= '9')
 return (byte)(c - '0');
 else
 return (byte)(c-'A'+10);
}

void setup() {
  uint16_t tmp;
  
  Serial.begin(38400);
  Serial.print("Started.\r\n");
  rda.begin();
  Serial.print("RDA Init\n");
  rda.rdaInit();
  delay(200);
  rda.setFrequency(433.9);
  rda.setTxChannel(INNERTONE2);
  rda.setTone(1.0);
  rda.setEmphasis(false);
  rda.setChannelMode(MODE125KHz);

  while(true) {
    while(!Serial.available());
    char c = Serial.read();

    while(Serial.available()) {
      Serial.read();
    }

    if(c == 'o') {
        rda.txOn();
        Serial.print("TX on\n");
    } else if(c == 'p') {
        rda.txOff();
        Serial.print("TX off\n");
    } else if(c == 'f') {
        Serial.print("FREQ: ");
        while(Serial.available() < 3) ;
        double flt = Serial.parseFloat();
        Serial.print("Setting freq ");
        Serial.print(flt);
        rda.setFrequency(flt);
        Serial.print("...\n");
    } else if(c == 'r') {
        rda.rxOn();
        Serial.print("RX on\n");
    } else if(c == 'i') {
        rda.rxOff();
        Serial.print("RX off\n");
    } else if(c == 't') {
      Serial.print("TONE: ");
      while(Serial.available() < 2);
      
        String data = "";
        data += Serial.read();
        data += Serial.read();
        data += Serial.read();
        data += Serial.read();
        
        unsigned char dataU = getVal(data[0]) + (getVal(data[1]) << 4) + (getVal(data[2]) << 8) + (getVal(data[3]) << 12); 
        rda.setTone(dataU);
    } else if(c == 'h') {
      digitalWrite(2, HIGH);
      delay(2);
      rda.setTone(2.0);
      digitalWrite(2, LOW);
    } else if(c == 'l') {
      digitalWrite(2, HIGH);
      delay(2);
      rda.setTone(1.0);
      digitalWrite(2, LOW);
    } else if(c == 'q') {
      uint16_t i = 0;

      for(i = 0; i < 4000; i++) {
        //digitalWrite(2, HIGH);
      //delay(4);
      rda.setTone(LOW_FRQ);
      //digitalWrite(2, LOW);
      delay(1);
       //digitalWrite(2, HIGH);
      //delay(4);
      rda.setTone(HIGH_FRQ);
      //digitalWrite(2, LOW);
      delay(1);
      }
    } else if(c == 'n') {
      uint16_t i = 0;

      for(i = 0; i < 4000; i++) {
        rda.setTone(((double)i)/1000.0);
        delay(1);
      }
    }
  }

}

void loop() {

}

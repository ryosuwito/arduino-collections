#include <TM1637Display.h>
#include <Arduino.h>
#define CLK 2
#define DIO1 3
#define DIO2 4
#define DIO3 5
#define DIO4 6
// RESERVED PIN

// SISA ANALOG PIN DIUBAH JADI GPIO A0,A1,A2,A3,A4,A5
// PIN A0,UNTUK SHIFT REGISTER BLOCK B1, B2 TOTAL 8 SENSOR
// PIN A1,UNTUK SHIFT REGISTER BLOCK B3, B4 TOTAL 8 SENSOR
// UNTUK INPUT SENSOR KE PIN 10

const uint8_t SEG_DONE[] = {SEG_G,SEG_G,SEG_G,SEG_G,};
const uint8_t SEG_CLEAR[] = {0,0,0,0};
int latchPin = 12;
int clockPin = 13;
int dataPin = 11;
int TransitionYellow = 3;
int GreenStep1 = 15;  //Variable to interate
int GreenStep2 = 15;  //Variable to interate
int GreenStep3 = 15;  //Variable to interate
int GreenStep4 = 15;  //Variable to interate
int RedStep1 = 0;
int RedStep2 = 0;
int RedStep3 = 0;
int RedStep4 = 0;
int NumbStep1 = 0;
int NumbStep2 = 0;
int NumbStep3 = 0;
int NumbStep4 = 0;                                                       

boolean isChanged = false;

byte data;
byte dataArray[4];

unsigned long interval = 1000;
unsigned long interval2 = 100;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();
  
TM1637Display display1(CLK, DIO1);

TM1637Display display2(CLK, DIO2);

TM1637Display display3(CLK, DIO3);

TM1637Display display4(CLK, DIO4);

int pinRed = 7;
int pinYellow = 8;
int pinGreen = 9;

int Red1 = HIGH;
int Yellow1 = HIGH;
int Green1 = HIGH;
int LightState1 = 1; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

int Red2 = HIGH;
int Yellow2 = HIGH;
int Green2 = HIGH;
int LightState2 = 3; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

int Red3 = HIGH;
int Yellow3 = HIGH;
int Green3 = HIGH;
int LightState3 = 3; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

int Red4 = HIGH;
int Yellow4 = HIGH;
int Green4 = HIGH;
int LightState4 = 3; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

void setup()
{
  pinMode(latchPin, OUTPUT);
  init_count();
  Serial.begin(9600);
  dataArray[0] = 0b11111110;
  dataArray[1] = 0b11111101;
  dataArray[2] = 0b11111011;
  dataArray[3] = 0b11110111;
  update_count();
  display1.setBrightness(0x00);  //set the diplay to minimum brightness
  display2.setBrightness(0x00);  //set the diplay to minimum brightness
  display3.setBrightness(0x00);  //set the diplay to minimum brightness
  display4.setBrightness(0x00);  //set the diplay to minimum brightness
  pinMode(pinRed, OUTPUT);  
  pinMode(pinYellow, OUTPUT);  
  pinMode(pinGreen, OUTPUT);
  update_pin_lamp1();
  update_pin_lamp2();
  update_pin_lamp3();
  update_pin_lamp4();
}


void loop()
{ 
  while (currentMillis - previousMillis <= interval){ 
    currentMillis = millis();
    currentMillis2 = millis();
    if (currentMillis2 - previousMillis2 >= interval2){
      int j = 0;
      for(j; j<5; j++){    
        update_lamp(j);
        delay(interval2);
      }
      if(isChanged == true){
        isChanged = false;
      }
      previousMillis2 = currentMillis2;
    }
  }
  check_state();
  isChanged = true;
  previousMillis = currentMillis;
}

void update_count(){
  NumbStep1 = GreenStep1;
  NumbStep2 = GreenStep1;
  NumbStep3 = NumbStep2 + GreenStep2 + TransitionYellow;
  NumbStep4 = NumbStep3 + GreenStep3 + TransitionYellow;
}

void init_count(){
  RedStep1 = GreenStep2 + GreenStep3 + GreenStep4 + (2*TransitionYellow);
  RedStep2 = GreenStep1 + GreenStep3 + GreenStep4 + (2*TransitionYellow);
  RedStep3 = GreenStep1 + GreenStep2 + GreenStep4 + (2*TransitionYellow);
  RedStep4 = GreenStep1 + GreenStep2 + GreenStep3 + (2*TransitionYellow);
}

void update_time(int pos1){
  switch(pos1){
    case 1:
      display1.setBrightness(0, true);
      display1.showNumberDec(NumbStep1);
      break;
    case 2:  
      display2.setBrightness(0, true);
      display2.showNumberDec(NumbStep2);
      break;
    case 3:
      display3.setBrightness(0, true);
      display3.showNumberDec(NumbStep3);
      break;
    case 4:
      display4.setBrightness(0, true);
      display4.showNumberDec(NumbStep4);
      break;
  }
}
void clear_time(int pos2){
  switch(pos2){
    case 1:
      display1.setSegments(SEG_DONE);
      //display1.setBrightness(0, false);
      break;
    case 2:
      display2.setSegments(SEG_DONE);
      //display2.setBrightness(0, false);
      break;
    case 3:
      display3.setSegments(SEG_DONE);
      //display3.setBrightness(0, false);
      break;
    case 4:
      display4.setSegments(SEG_DONE );
      //display4.setBrightness(0, false);
      break;
  }
}
void check_state(){
  if(NumbStep1 < 1){
    update_pin_lamp1();
  }  
  if(NumbStep2 < 1){
    update_pin_lamp2();
  }  
  if(NumbStep3 < 1){
    update_pin_lamp3();
  }  
  if(NumbStep4 < 1){
    update_pin_lamp4();
  }  
}

void update_pin_lamp1(){
  switch(LightState1){
    case 0:
      LightState1 = 1;
      NumbStep1 = TransitionYellow;
      Red1 = LOW;
      Yellow1 = LOW;
      Green1 = HIGH;
      break;
      
    case 1:
      LightState1 = 2;
      NumbStep1 = GreenStep1;
      Red1 = HIGH;
      Yellow1 = HIGH;
      Green1 = LOW;
      break;
      
    case 2:
      LightState1 = 3;
      NumbStep1 = TransitionYellow;
      Red1 = HIGH;
      Yellow1 = LOW;
      Green1 = HIGH;
      break;
      
    case 3:
      LightState1 = 0;
      NumbStep1 = RedStep1;
      Red1 = LOW;
      Yellow1 = HIGH;
      Green1 = HIGH;
      break;
  }
}

void update_pin_lamp2(){
  switch(LightState2){
    case 0:
      LightState2 = 1;
      NumbStep2 = TransitionYellow;
      Red2 = LOW;
      Yellow2 = LOW;
      Green2 = HIGH;
      break;
      
    case 1:
      LightState2 = 2;
      NumbStep2 = GreenStep2;
      Red2 = HIGH;
      Yellow2 = HIGH;
      Green2 = LOW;
      break;
      
    case 2:
      LightState2 = 3;
      NumbStep2 = TransitionYellow;
      Red2 = HIGH;
      Yellow2 = LOW;
      Green2 = HIGH;
      break;
      
    case 3:
      LightState2 = 0;
      NumbStep2 = RedStep2;
      Red2 = LOW;
      Yellow2 = HIGH;
      Green2 = HIGH;
      break;
  }
}

void update_pin_lamp3(){
  switch(LightState3){
    case 0:
      LightState3 = 1;
      NumbStep3 = TransitionYellow;
      Red3 = LOW;
      Yellow3 = LOW;
      Green3 = HIGH;
      break;
      
    case 1:
      LightState3 = 2;
      NumbStep3 = GreenStep3;
      Red3 = HIGH;
      Yellow3 = HIGH;
      Green3 = LOW;
      break;
      
    case 2:
      LightState3 = 3;
      NumbStep3 = TransitionYellow;
      Red3 = HIGH;
      Yellow3 = LOW;
      Green3 = HIGH;
      break;
      
    case 3:
      LightState3 = 0;
      NumbStep3 = RedStep3;
      Red3 = LOW;
      Yellow3 = HIGH;
      Green3 = HIGH;
      break;
  }
}

void update_pin_lamp4(){
  switch(LightState4){
    case 0:
      LightState4 = 1;
      NumbStep4 = TransitionYellow;
      Red4 = LOW;
      Yellow4 = LOW;
      Green4 = HIGH;
      break;
      
    case 1:
      LightState4 = 2;
      NumbStep4 = GreenStep4;
      Red4 = HIGH;
      Yellow4 = HIGH;
      Green4 = LOW;
      break;
      
    case 2:
      LightState4 = 3;
      NumbStep4 = TransitionYellow;
      Red4 = HIGH;
      Yellow4 = LOW;
      Green4 = HIGH;
      break;
      
    case 3:
      LightState4 = 0;
      NumbStep4 = RedStep4;
      Red4 = LOW;
      Yellow4 = HIGH;
      Green4 = HIGH;
      break;
  }
}
void update_lamp(int i){
  switch(i){
    case 1:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red1);
      digitalWrite(pinYellow, Yellow1);
      digitalWrite(pinGreen, Green1);
      if(isChanged == true){
        if(LightState1 == 2){
          update_time(1);
        } 
        else {
          clear_time(1);
        }
        NumbStep1--;
      }
      break;
      
    case 2:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red2);
      digitalWrite(pinYellow, Yellow2);
      digitalWrite(pinGreen, Green2);
      if(isChanged == true){
        if(LightState2 == 2){
          update_time(2);
        } 
        else {
          clear_time(2);
        }
        NumbStep2--;
      }
      break;
      
    case 3:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red3);
      digitalWrite(pinYellow, Yellow3);
      digitalWrite(pinGreen, Green3);
      if(isChanged == true){
        if(LightState3 == 2){
          update_time(3);
        } 
        else {
          clear_time(3);
        }
        NumbStep3--;
      }
      break;
      
    case 4:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red4);
      digitalWrite(pinYellow, Yellow4);
      digitalWrite(pinGreen, Green4);
      if(isChanged == true){
        if(LightState4 == 2){
          update_time(4);
        } 
        else {
          clear_time(4);
        }
        NumbStep4--;
      }
      break;
  }
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut){
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for(int k=7; k>=0; k--){
    digitalWrite(myClockPin, 0);

    if (myDataOut & (1<<k)){
      pinState = 1;
    }else{
      pinState = 0;
    }

    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}


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
int GreenStep1 = 13;  //Variable to interate
int GreenStep2 = 17;  //Variable to interate
int GreenStep3 = 19;  //Variable to interate
int GreenStep4 = 14;  //Variable to interate
int RedStep1 = GreenStep2 + GreenStep3 + GreenStep4 + (2*TransitionYellow);
int RedStep2 = GreenStep1 + GreenStep3 + GreenStep4 + (2*TransitionYellow);
int RedStep3 = GreenStep1 + GreenStep2 + GreenStep4 + (2*TransitionYellow);
int RedStep4 = GreenStep1 + GreenStep2 + GreenStep3 + (2*TransitionYellow);
int NumbStep1 = 0;
int NumbStep2 = 0;
int NumbStep3 = 0;
int NumbStep4 = 0;

int isObstacle = HIGH;
boolean isChanged = false;

int i = 1,j = 1;
byte data;
byte dataArray[4];
byte sensingArray[8];

unsigned long interval = 1000;
unsigned long interval2 = 250;
unsigned long interval3 = 5000;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

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
  
  Serial.begin(9600);
  dataArray[0] = 0b00000001;
  dataArray[1] = 0b00000010;
  dataArray[2] = 0b00000100;
  dataArray[3] = 0b00001000;

  sensingArray[0] = 0b11111110;
  sensingArray[1] = 0b11111101;
  sensingArray[2] = 0b11111011;
  sensingArray[3] = 0b11110111;  
  sensingArray[4] = 0b11101111;
  sensingArray[5] = 0b11011111;
  sensingArray[6] = 0b10111111;
  sensingArray[7] = 0b01111111; 
  
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
  NumbStep1 = GreenStep1;
  NumbStep2 = GreenStep1;
  NumbStep3 = NumbStep2 + GreenStep2 + TransitionYellow;
  NumbStep4 = NumbStep3 + GreenStep3 + TransitionYellow;
}


void loop()
{

 
  unsigned long currentMillis = millis();
  unsigned long currentMillis2 = millis();
  unsigned long currentMillis3 = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval){
    update_time();
    previousMillis = currentMillis;
  }
  if ((unsigned long)(currentMillis2 - previousMillis2) >= interval2){
    update_lamp();
    i++;
    if(i>4)i=1;
    previousMillis2 = currentMillis2;
  }
  if ((unsigned long)(currentMillis3 - previousMillis3) >= interval3){
    j++;
    if(j>8)j=1;
    previousMillis3 = currentMillis3;
  }

}

void update_time(){

  check_state();
  //display(CLK, DIO1);
  if(LightState1 == 1 | LightState1 == 3){
    display1.setSegments(SEG_DONE);
  }else{
    display1.showNumberDec(NumbStep1); //Display the Variable value;
  }
  
  //display(CLK, DIO2);
  if(LightState2 == 1 | LightState2 == 3){
    display2.setSegments(SEG_DONE);
  }else{
    display2.showNumberDec(NumbStep2); //Display the Variable value;
  }
  
  //display(CLK, DIO3);
  if(LightState3 == 1 | LightState3 == 3){
    display3.setSegments(SEG_DONE);
  }else{
    display3.showNumberDec(NumbStep3); //Display the Variable value;
  }
  
  //display(CLK, DIO4);
  if(LightState4 == 1 | LightState4 == 3){
    display4.setSegments(SEG_DONE);
  }else{
    display4.showNumberDec(NumbStep4); //Display the Variable value;
  }
  
  
  NumbStep1--;
  NumbStep2--;
  NumbStep3--;
  NumbStep4--;
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
void update_lamp(){
  switch(i){
    case 1:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red1);
      digitalWrite(pinYellow, Yellow1);
      digitalWrite(pinGreen, Green1);
      break;
      
    case 2:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red2);
      digitalWrite(pinYellow, Yellow2);
      digitalWrite(pinGreen, Green2);
      break;
      
    case 3:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red3);
      digitalWrite(pinYellow, Yellow3);
      digitalWrite(pinGreen, Green3);
      break;
      
    case 4:
      data = dataArray[i-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red4);
      digitalWrite(pinYellow, Yellow4);
      digitalWrite(pinGreen, Green4);
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


#include <TM1637Display.h>
#include <Arduino.h>
#define CLK 2
#define DIO1 3
#define DIO2 4
#define DIO3 5
#define DIO4 6

const uint8_t SEG_DONE[] = {SEG_G,SEG_G,SEG_G,SEG_G,};
const uint8_t SEG_CLEAR[] = {0,0,0,0};
int TransitionYellow = 3;
int GreenStep1 = 53;  //Variable to interate
int GreenStep2 = 67;  //Variable to interate
int GreenStep3 = 59;  //Variable to interate
int GreenStep4 = 64;  //Variable to interate
int RedStep1 = GreenStep2 + GreenStep3 + GreenStep4 + (2*TransitionYellow);
int RedStep2 = GreenStep1 + GreenStep3 + GreenStep4 + (2*TransitionYellow);
int RedStep3 = GreenStep1 + GreenStep2 + GreenStep4 + (2*TransitionYellow);
int RedStep4 = GreenStep1 + GreenStep2 + GreenStep3 + (2*TransitionYellow);
int NumbStep1 = 0;
int NumbStep2 = 0;
int NumbStep3 = 0;
int NumbStep4 = 0;
int i = 1;

unsigned long interval = 1000;
unsigned long interval2 = 3;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

TM1637Display display1(CLK, DIO1);

TM1637Display display2(CLK, DIO2);

TM1637Display display3(CLK, DIO3);

TM1637Display display4(CLK, DIO4);

int pinRed = 7;
int pinYellow = 8;
int pinGreen = 9;
int pinLight1 = 10;
int pinLight2 = 11;
int pinLight3 = 12;
int pinLight4 = 13;

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
  display1.setBrightness(0x00);  //set the diplay to minimum brightness
  display2.setBrightness(0x00);  //set the diplay to minimum brightness
  display3.setBrightness(0x00);  //set the diplay to minimum brightness
  display4.setBrightness(0x00);  //set the diplay to minimum brightness
  pinMode(pinRed, OUTPUT);  
  pinMode(pinYellow, OUTPUT);  
  pinMode(pinGreen, OUTPUT);
  pinMode(pinLight1, OUTPUT);
  pinMode(pinLight2, OUTPUT);
  pinMode(pinLight3, OUTPUT);
  pinMode(pinLight4, OUTPUT);
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
      Green1 = LOW;
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
      Green2 = LOW;
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
      Green3 = LOW;
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
      Green4 = LOW;
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
      digitalWrite(pinLight1, HIGH);
      digitalWrite(pinLight2, LOW);
      digitalWrite(pinLight3, LOW);
      digitalWrite(pinLight4, LOW);
      digitalWrite(pinRed, Red1);
      digitalWrite(pinYellow, Yellow1);
      digitalWrite(pinGreen, Green1);
      break;
      
    case 2:
      digitalWrite(pinLight1, LOW);
      digitalWrite(pinLight2, HIGH);
      digitalWrite(pinLight3, LOW);
      digitalWrite(pinLight4, LOW);
      digitalWrite(pinRed, Red2);
      digitalWrite(pinYellow, Yellow2);
      digitalWrite(pinGreen, Green2);
      break;
      
    case 3:
      digitalWrite(pinLight1, LOW);
      digitalWrite(pinLight2, LOW);
      digitalWrite(pinLight3, HIGH);
      digitalWrite(pinLight4, LOW);
      digitalWrite(pinRed, Red3);
      digitalWrite(pinYellow, Yellow3);
      digitalWrite(pinGreen, Green3);
      break;
      
    case 4:
      digitalWrite(pinLight1, LOW);
      digitalWrite(pinLight2, LOW);
      digitalWrite(pinLight3, LOW);
      digitalWrite(pinLight4, HIGH);
      digitalWrite(pinRed, Red4);
      digitalWrite(pinYellow, Yellow4);
      digitalWrite(pinGreen, Green4);
      break;
  }
}


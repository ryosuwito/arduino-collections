#include <TM1637Display.h>
#include <Arduino.h>
#define CLK 2
#define DIO1 3
#define DIO2 4
#define DIO3 5
#define DIO4 6
#define INDIKATOR A0
#define SENSING 10
// RESERVED PIN

// SISA ANALOG PIN DIUBAH JADI GPIO A0,A1,A2,A3,A4,A5
// PIN A0,UNTUK SHIFT REGISTER BLOCK B1, B2 TOTAL 8 SENSOR
// PIN A1,UNTUK SHIFT REGISTER BLOCK B3, B4 TOTAL 8 SENSOR
// UNTUK INPUT SENSOR KE PIN 10
 
char charData = ' ';
const uint8_t SEG_DONE[] = {SEG_G,SEG_G,SEG_G,SEG_G,};
const uint8_t SEG_CLEAR[] = {0,0,0,0};

int clockPin = 13;
int latchPin = 12;
int dataPin = 11;
int isObstacle = HIGH;

int sensingClockPin = A3;
int sensingLatchPin = A2;
int sensingDataPin = A1;

int TransitionYellow = 3;
int Base = 10;
int GreenStep1 = Base;  //Variable to interate
int GreenStep2 = Base;  //Variable to interate
int GreenStep3 = Base;  //Variable to interate
int GreenStep4 = Base;  //Variable to interate
int RedStep1 = 0;
int RedStep2 = 0;
int RedStep3 = 0;
int RedStep4 = 0;
int NumbStep1 = 0;
int NumbStep2 = 0;
int NumbStep3 = 0;
int NumbStep4 = 0;                                                       

boolean isChanged = false;
boolean isChecked1 = true;
boolean isChecked2 = true;
boolean isChecked3 = true;
boolean isChecked4 = true;

byte data;
byte dataArray[4];
byte clearBlock = 0b00000000;
byte sensingData;
byte sensingBlock1[3];
byte sensingBlock2[3];
byte sensingBlock3[3];
byte sensingBlock4[3];

unsigned long interval = 1000;
unsigned long interval2 = 5;
unsigned long interval3 = 1000;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();
unsigned long currentMillis3 = millis();
  
TM1637Display display1(CLK, DIO1);

TM1637Display display2(CLK, DIO2);

TM1637Display display3(CLK, DIO3);

TM1637Display display4(CLK, DIO4);

int idx = 1;
int pinRed = 7;
int pinYellow = 8;
int pinGreen = 9;

int Red1 = LOW;
int Yellow1 = LOW;
int Green1 = LOW;
int LightState1 = 1; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

int Red2 = LOW;
int Yellow2 = LOW;
int Green2 = LOW;
int LightState2 = 3; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

int Red3 = LOW;
int Yellow3 = LOW;
int Green3 = LOW;
int LightState3 = 3; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

int Red4 = LOW;
int Yellow4 = LOW;
int Green4 = LOW;
int LightState4 = 3; //0 = Red; 1 = Yellow (Red To Green); 2 = Green; 3 = Yellow (Green To Red)

void setup(){
  pinMode(22, OUTPUT); 
  pinMode(latchPin, OUTPUT);
  pinMode(sensingLatchPin, OUTPUT);
  pinMode(SENSING, INPUT);
  pinMode(pinRed, OUTPUT);  
  pinMode(pinYellow, OUTPUT);  
  pinMode(pinGreen, OUTPUT);
  pinMode(INDIKATOR, OUTPUT);
  digitalWrite(INDIKATOR, HIGH);
  
  init_count();
  
  
  Serial.begin(9600);
  
  Serial.println("Arduino is ready");
  
  Serial.println("Remember to select Both NL & CR in the serial monitor");
  
  // HC-05 default serial speed for AT mode is 38400
  
  Serial3.begin(9600);
  
  dataArray[0] = 0b11111110;
  dataArray[1] = 0b11111101;
  dataArray[2] = 0b11111011;
  dataArray[3] = 0b11110111;
  
  sensingBlock2[0] = 0b10111110;
  sensingBlock2[1] = 0b10111101;
  sensingBlock2[2] = 0b10111011;
   
  sensingBlock3[0] = 0b10110111;
  sensingBlock3[1] = 0b10101111;
  sensingBlock3[2] = 0b10011111;
    
  sensingBlock1[0] = 0b01111110;
  sensingBlock1[1] = 0b01111101;
  sensingBlock1[2] = 0b01111011;
  
  sensingBlock4[0] = 0b01110111;
  sensingBlock4[1] = 0b01101111;
  sensingBlock4[2] = 0b01011111;
  
  display1.setBrightness(0x00);  //set the diplay to minimum brightness
  display2.setBrightness(0x00);  //set the diplay to minimum brightness
  display3.setBrightness(0x00);  //set the diplay to minimum brightness
  display4.setBrightness(0x00);  //set the diplay to minimum brightness
  
  update_pin_lamp1();
  update_pin_lamp2();
  update_pin_lamp3();
  update_pin_lamp4();

  update_count();
}


void loop(){ 
  if (Serial3.available()) {
    
    charData = Serial3.read();
    
    Serial.write(charData);
    
    if(charData < '3')            //Checks whether value of data is equal to 1 
      digitalWrite(22, HIGH);  //If value is 1 then LED turns ON
    else if(charData > '2')       //Checks whether value of data is equal to 0
      digitalWrite(22, LOW); 
    
  
  }
  
  // Keep reading from Arduino Serial Monitor and send to HC-05
  
  if (Serial.available()) {
    
    charData = Serial.read();
    
    Serial3.write(charData); 
  }
  
  currentMillis = millis();
  currentMillis2 = millis();
  currentMillis3 = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval){
    
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
    
    isChanged = true;
    previousMillis = currentMillis;
    
    if(LightState1 == 2 || LightState1 == 0){
      update_time(1);
      isChecked1 = false;
    } 
    else {   
      if(LightState1 == 1 && isChecked1 == false){
        checkBlock1();
      }
      
      clear_time(1);
    }
    NumbStep1--;  
      
    if(LightState2 == 2 || LightState2 == 0){
      update_time(2);
      isChecked2 = false;
    } 
    else { 
      if(LightState2 == 1 && isChecked2 == false){
        checkBlock2();
      }
      clear_time(2);
    }
    NumbStep2--;   
     
    if(LightState3 == 2 || LightState3 == 0){
      update_time(3);
      isChecked3 = false;
    } 
    else { 
      if(LightState3 == 1 && isChecked3 == false){
        checkBlock3();
      }
      clear_time(3);
    }
    NumbStep3--; 
       
    if(LightState4 == 2 || LightState4 == 0){
      update_time(4);
      isChecked4 = false;
    } 
    else { 
      if(LightState4 == 1 && isChecked4 == false){
        checkBlock4();
      }
      clear_time(4);
    }
    NumbStep4--;  
  }

  if ((unsigned long)(currentMillis2 - previousMillis2) >= interval2){
    update_lamp(idx);
    idx++;
    previousMillis2 = currentMillis2;
  }
  if(idx > 4){
    isChanged = false;
    idx = 1;
  }
}

void checkBlock1(){
    sensingData = sensingBlock1[0];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    
    isObstacle = digitalRead(SENSING);
    
    GreenStep1 = Base;
    
    if(isObstacle == LOW){
      digitalWrite(INDIKATOR, LOW);
      GreenStep1 = Base + 15;
      NumbStep2 += 15;
      NumbStep3 += 15;
    }else{
      digitalWrite(INDIKATOR, HIGH);
    }
    delay(500);
    sensingData = sensingBlock1[1];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(500);
    sensingData = sensingBlock1[2];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(500);
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, clearBlock);
    digitalWrite(sensingLatchPin, 1);
    isChecked1 = true; 
    
    RedStep2 = GreenStep1;
    RedStep3 = NumbStep2 + GreenStep2 + TransitionYellow;
    RedStep4 = NumbStep3 + GreenStep3 + TransitionYellow;
}

void checkBlock2(){
    sensingData = sensingBlock2[0];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    
    isObstacle = digitalRead(SENSING);
    
    GreenStep2 = Base;
    if(isObstacle == LOW){
      digitalWrite(INDIKATOR, LOW);
      GreenStep2 = Base + 15;
      NumbStep4 += 15;
      NumbStep3 += 15;
    }else{
      digitalWrite(INDIKATOR, HIGH);
    }
    delay(500);
    sensingData = sensingBlock2[1];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(100);
    sensingData = sensingBlock2[2];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(100);
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, clearBlock);
    digitalWrite(sensingLatchPin, 1);    
    isChecked2 = true;    
    RedStep3 = GreenStep2;
    RedStep4 = NumbStep3 + GreenStep3 + TransitionYellow;
    RedStep1 = NumbStep4 + GreenStep4 + TransitionYellow;
}
void checkBlock3(){
    sensingData = sensingBlock3[0];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    
    isObstacle = digitalRead(SENSING);
    
    GreenStep3 = Base;
    if(isObstacle == LOW){
      digitalWrite(INDIKATOR, LOW);
      GreenStep3 = Base + 15;
      NumbStep1 += 15;
      NumbStep4 += 15;
    }else{
      digitalWrite(INDIKATOR, HIGH);
    }
    delay(500);
    sensingData = sensingBlock3[1];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(100);
    sensingData = sensingBlock3[2];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(100);
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, clearBlock);
    digitalWrite(sensingLatchPin, 1);   
    isChecked3 = true;    
    RedStep4 = GreenStep3;
    RedStep1 = NumbStep4 + GreenStep4 + TransitionYellow;
    RedStep2 = NumbStep1 + GreenStep1 + TransitionYellow;
}
void checkBlock4(){
    sensingData = sensingBlock4[0];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    
    isObstacle = digitalRead(SENSING);
    
    GreenStep4 = Base;
    if(isObstacle == LOW){
      digitalWrite(INDIKATOR, LOW);
      GreenStep4 = Base + 15;
      NumbStep1 += 15;
      NumbStep2 += 15;
    }else{
      digitalWrite(INDIKATOR, HIGH);
    }
    delay(500);
    sensingData = sensingBlock4[1];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(100);
    sensingData = sensingBlock4[2];
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, sensingData);
    digitalWrite(sensingLatchPin, 1);
    delay(100);
    digitalWrite(sensingLatchPin, 0);
    shiftOut(sensingDataPin, sensingClockPin, clearBlock);
    digitalWrite(sensingLatchPin, 1);  
    isChecked4 = true;    
    RedStep1 = GreenStep4;
    RedStep2 = NumbStep1 + GreenStep1 + TransitionYellow;
    RedStep3 = NumbStep2 + GreenStep2 + TransitionYellow;
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
      display1.showNumberDec(NumbStep1);
      break;
    case 2:  
      display2.showNumberDec(NumbStep2);
      break;
    case 3:
      display3.showNumberDec(NumbStep3);
      break;
    case 4:
      display4.showNumberDec(NumbStep4);
      break;
  }
}
void clear_time(int pos2){
  switch(pos2){
    case 1:
      display1.setSegments(SEG_DONE);
      break;
    case 2:
      display2.setSegments(SEG_DONE);
      break;
    case 3:
      display3.setSegments(SEG_DONE);
      break;
    case 4:
      display4.setSegments(SEG_DONE);
      break;
  }
}
void check_state(){
    update_pin_lamp1();
    update_pin_lamp2();
    update_pin_lamp3();
    update_pin_lamp4();
}

void update_pin_lamp1(){
  switch(LightState1){
    case 0:
      LightState1 = 1;
      NumbStep1 = TransitionYellow;
      Red1 = LOW;
      Yellow1 = HIGH;
      Green1 = LOW;
      break;
      
    case 1:
      LightState1 = 2;
      NumbStep1 = GreenStep1;
      Red1 = LOW;
      Yellow1 = LOW;
      Green1 = HIGH;
      break;
      
    case 2:
      LightState1 = 3;
      NumbStep1 = TransitionYellow;
      Red1 = LOW;
      Yellow1 = HIGH;
      Green1 = LOW;
      break;
      
    case 3:
      LightState1 = 0;
      NumbStep1 = RedStep1;
      Red1 = HIGH;
      Yellow1 = LOW;
      Green1 = LOW;
      break;
  }
}

void update_pin_lamp2(){
  switch(LightState2){
    case 0:
      LightState2 = 1;
      NumbStep2 = TransitionYellow;
      Red2 = LOW;
      Yellow2 = HIGH;
      Green2 = LOW;
      break;
      
    case 1:
      LightState2 = 2;
      NumbStep2 = GreenStep2;
      Red2 = LOW;
      Yellow2 = LOW;
      Green2 = HIGH;
      break;
      
    case 2:
      LightState2 = 3;
      NumbStep2 = TransitionYellow;
      Red2 = LOW;
      Yellow2 = HIGH;
      Green2 = LOW;
      break;
      
    case 3:
      LightState2 = 0;
      NumbStep2 = RedStep2;
      Red2 = HIGH;
      Yellow2 = LOW;
      Green2 = LOW;
      break;
  }
}

void update_pin_lamp3(){
  switch(LightState3){
    case 0:
      LightState3 = 1;
      NumbStep3 = TransitionYellow;
      Red3 = LOW;
      Yellow3 = HIGH;
      Green3 = LOW;
      break;
      
    case 1:
      LightState3 = 2;
      NumbStep3 = GreenStep3;
      Red3 = LOW;
      Yellow3 = LOW;
      Green3 = HIGH;
      break;
      
    case 2:
      LightState3 = 3;
      NumbStep3 = TransitionYellow;
      Red3 = LOW;
      Yellow3 = HIGH;
      Green3 = LOW;
      break;
      
    case 3:
      LightState3 = 0;
      NumbStep3 = RedStep3;
      Red3 = HIGH;
      Yellow3 = LOW;
      Green3 = LOW;
      break;
  }
}

void update_pin_lamp4(){
  switch(LightState4){
    case 0:
      LightState4 = 1;
      NumbStep4 = TransitionYellow;
      Red4 = LOW;
      Yellow4 = HIGH;
      Green4 = LOW;
      break;
      
    case 1:
      LightState4 = 2;
      NumbStep4 = GreenStep4;
      Red4 = LOW;
      Yellow4 = LOW;
      Green4 = HIGH;
      break;
      
    case 2:
      LightState4 = 3;
      NumbStep4 = TransitionYellow;
      Red4 = LOW;
      Yellow4 = HIGH;
      Green4 = LOW;
      break;
      
    case 3:
      LightState4 = 0;
      NumbStep4 = RedStep4;
      Red4 = HIGH;
      Yellow4 = LOW;
      Green4 = LOW;
      break;
  }
}
void update_lamp(int index){
  switch(index){
    case 1:
      data = dataArray[index-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red1);
      digitalWrite(pinYellow, Yellow1);
      digitalWrite(pinGreen, Green1);
      break;
      
    case 2:
      data = dataArray[index-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red2);
      digitalWrite(pinYellow, Yellow2);
      digitalWrite(pinGreen, Green2);
      break;
      
    case 3:
      data = dataArray[index-1];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data);
      digitalWrite(latchPin, 1);
      digitalWrite(pinRed, Red3);
      digitalWrite(pinYellow, Yellow3);
      digitalWrite(pinGreen, Green3);
      break;
      
    case 4:
      data = dataArray[index-1];
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


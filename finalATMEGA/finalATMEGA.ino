#include <TM1637Display.h>
#include <Arduino.h>
#define CLK 2
#define DIO1 3
#define DIO2 4
#define DIO3 5
#define DIO4 6

#define PIN_RED1 22
#define PIN_YELLOW1 23
#define PIN_GREEN1 24
#define GND_BLOCK1 25

#define PIN_RED2 26
#define PIN_YELLOW2 27
#define PIN_GREEN2 28
#define GND_BLOCK2 29

#define PIN_RED3 30
#define PIN_YELLOW3 31
#define PIN_GREEN3 32
#define GND_BLOCK3 33

#define PIN_RED4 34
#define PIN_YELLOW4 35
#define PIN_GREEN4 36
#define GND_BLOCK4 37

#define SENSING1A 38
#define SENSING1B 39
#define SENSING1C 40

#define SENSING2A 41
#define SENSING2B 42
#define SENSING2C 43

#define SENSING3A 44
#define SENSING3B 45
#define SENSING3C 46

#define SENSING4A 47
#define SENSING4B 48
#define SENSING4C 49

#define PIN_ALERT 50

char charData = ' ';
const uint8_t SEG_DONE[] = {SEG_G,SEG_G,SEG_G,SEG_G,};
const uint8_t SEG_CLEAR[] = {0,0,0,0};
const int SENSOR_VALUE = 15;

int isObstacleA = HIGH;
int isObstacleB = HIGH;
int isObstacleC = HIGH;

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

unsigned long intervalSecond = 500;
unsigned long intervalLampu = 5;
unsigned long intervalInterupt = 15*intervalSecond;
unsigned long lastMillisSecond = 0;
unsigned long lastMillisLampu = 0;
unsigned long lastMillisInterupt = 0;
unsigned long currentMillisSecond = millis();
unsigned long currentMillisLampu = millis();
unsigned long currentMillisInterupt = millis();
  
TM1637Display display1(CLK, DIO1);

TM1637Display display2(CLK, DIO2);

TM1637Display display3(CLK, DIO3);

TM1637Display display4(CLK, DIO4);

int idx = 1;

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
  pinMode(PIN_RED1, OUTPUT);
  pinMode(PIN_YELLOW1, OUTPUT);
  pinMode(PIN_GREEN1, OUTPUT);
  pinMode(GND_BLOCK1, OUTPUT);
  digitalWrite(GND_BLOCK1, LOW);

  pinMode(PIN_RED2, OUTPUT);
  pinMode(PIN_YELLOW2, OUTPUT);
  pinMode(PIN_GREEN2, OUTPUT);
  pinMode(GND_BLOCK2, OUTPUT);
  digitalWrite(GND_BLOCK2, LOW);

  pinMode(PIN_RED3, OUTPUT);
  pinMode(PIN_YELLOW3, OUTPUT);
  pinMode(PIN_GREEN3, OUTPUT);
  pinMode(GND_BLOCK3, OUTPUT);
  digitalWrite(GND_BLOCK3, LOW);

  pinMode(PIN_RED4, OUTPUT);
  pinMode(PIN_YELLOW4, OUTPUT);
  pinMode(PIN_GREEN4, OUTPUT);
  pinMode(GND_BLOCK4, OUTPUT);
  digitalWrite(GND_BLOCK4, LOW);
  
  pinMode(SENSING1A, INPUT);
  pinMode(SENSING1B, INPUT);
  pinMode(SENSING1C, INPUT);

  pinMode(SENSING2A, INPUT);
  pinMode(SENSING2B, INPUT);
  pinMode(SENSING2C, INPUT);

  pinMode(SENSING3A, INPUT);
  pinMode(SENSING3B, INPUT);
  pinMode(SENSING3C, INPUT);

  pinMode(SENSING4A, INPUT);
  pinMode(SENSING4B, INPUT);
  pinMode(SENSING4C, INPUT);

  pinMode(PIN_ALERT, OUTPUT);
  digitalWrite(PIN_ALERT, HIGH);
  init_count();
  
  
  Serial.begin(9600);
  
  Serial.println("Arduino is ready");
  
  Serial.println("Remember to select Both NL & CR in the serial monitor");
  
  // HC-05 default serial speed for AT mode is 38400
  
  Serial3.begin(9600);
    
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
  
  }
  
  // Keep reading from Arduino Serial Monitor and send to HC-05
  
  if (Serial.available()) {
    
    charData = Serial.read();
    
    Serial3.write(charData); 
  }
      
  if(charData == '1'){
    interupt(1);
  } 
  else if(charData == '2'){      
    interupt(2);    
  } 
  else if(charData == '3'){      
    interupt(3);    
  } 
  else if(charData == '4'){      
    interupt(4);    
  }
    
  currentMillisSecond = millis();
  currentMillisLampu = millis();
  if ((unsigned long)(currentMillisSecond  - lastMillisSecond ) >= intervalSecond){

    isChanged = true;
    lastMillisSecond = currentMillisSecond;
    
    if(LightState1 == 2 || LightState1 == 0){
      update_time(1);
      isChecked1 = false;
    } 
    else {  
      if(LightState1 == 2 && isChecked1 == false){
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
      if(LightState2 == 2 && isChecked2 == false){
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
      if(LightState3 == 2 && isChecked3 == false){
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
      if(LightState4 == 2 && isChecked4 == false){
        checkBlock4();
      }
      clear_time(4);
    }
    NumbStep4--;  

        
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

  if ((unsigned long)(currentMillisLampu - lastMillisLampu) >= intervalLampu){
    update_lamp(idx);
    idx++;
    lastMillisLampu = currentMillisLampu;
  }
  if(idx > 4){
    isChanged = false;
    idx = 1;
  }
}

void interupt(int index){
    currentMillisInterupt = millis();
    lastMillisInterupt = currentMillisInterupt;
    
    currentMillisSecond = millis();
    lastMillisSecond = currentMillisSecond;
    boolean done = false;
    int idxBlink = 0;

    digitalWrite(PIN_RED1, LOW);
    digitalWrite(PIN_YELLOW1, LOW);
    digitalWrite(PIN_GREEN1, LOW);
    digitalWrite(PIN_RED2, LOW);
    digitalWrite(PIN_YELLOW2, LOW);
    digitalWrite(PIN_GREEN2, LOW);
    digitalWrite(PIN_RED3, LOW);
    digitalWrite(PIN_YELLOW3, LOW);
    digitalWrite(PIN_GREEN3, LOW);
    digitalWrite(PIN_RED4, LOW);
    digitalWrite(PIN_YELLOW4, LOW);
    digitalWrite(PIN_GREEN4, LOW);

    currentMillisSecond = millis();
    lastMillisSecond = currentMillisSecond;
    idxBlink = 5;
    while(idxBlink > 0 ){
      currentMillisSecond = millis();
      if ((unsigned long)(currentMillisSecond  - lastMillisSecond ) >= intervalSecond){ 
        display1.showNumberDec(idxBlink);
        display2.showNumberDec(idxBlink);
        display3.showNumberDec(idxBlink);
        display4.showNumberDec(idxBlink);
        idxBlink--;
        lastMillisSecond = currentMillisSecond;
      }
      digitalWrite(PIN_YELLOW1, HIGH);
      digitalWrite(PIN_YELLOW2, HIGH);
      digitalWrite(PIN_YELLOW3, HIGH);
      digitalWrite(PIN_YELLOW4, HIGH);
    }
    while ((unsigned long)(currentMillisInterupt  - lastMillisInterupt ) < intervalInterupt){
      currentMillisInterupt = millis();
      currentMillisSecond = millis();
      if ((unsigned long)(currentMillisSecond  - lastMillisSecond ) >= intervalSecond/3){ 
        if(idxBlink%2 == 0){    
          digitalWrite(PIN_ALERT, LOW);
          display1.setSegments(SEG_DONE);
          display2.setSegments(SEG_DONE);
          display3.setSegments(SEG_DONE);
          display4.setSegments(SEG_DONE);
        }else{
          digitalWrite(PIN_ALERT, HIGH);
          display1.setSegments(SEG_CLEAR);
          display2.setSegments(SEG_CLEAR);
          display3.setSegments(SEG_CLEAR);
          display4.setSegments(SEG_CLEAR);
        }
        lastMillisSecond = currentMillisSecond;
        idxBlink++;
      }
      if(done == false){
        switch(index){
          case 1: 
            digitalWrite(PIN_RED1, LOW);
            digitalWrite(PIN_YELLOW1, LOW);
            digitalWrite(PIN_GREEN1, HIGH);
            digitalWrite(PIN_RED2, HIGH);
            digitalWrite(PIN_YELLOW2, LOW);
            digitalWrite(PIN_GREEN2, LOW);
            digitalWrite(PIN_RED3, HIGH);
            digitalWrite(PIN_YELLOW3, LOW);
            digitalWrite(PIN_GREEN3, LOW);
            digitalWrite(PIN_RED4, HIGH);
            digitalWrite(PIN_YELLOW4, LOW);
            digitalWrite(PIN_GREEN4, LOW);
            break;
            
          case 2: 
            digitalWrite(PIN_RED1, HIGH);
            digitalWrite(PIN_YELLOW1, LOW);
            digitalWrite(PIN_GREEN1, LOW);
            digitalWrite(PIN_RED2, LOW);
            digitalWrite(PIN_YELLOW2, LOW);
            digitalWrite(PIN_GREEN2, HIGH);
            digitalWrite(PIN_RED3, HIGH);
            digitalWrite(PIN_YELLOW3, LOW);
            digitalWrite(PIN_GREEN3, LOW);
            digitalWrite(PIN_RED4, HIGH);
            digitalWrite(PIN_YELLOW4, LOW);
            digitalWrite(PIN_GREEN4, LOW);
            break;
                      
          case 3: 
            digitalWrite(PIN_RED1, HIGH);
            digitalWrite(PIN_YELLOW1, LOW);
            digitalWrite(PIN_GREEN1, LOW);
            digitalWrite(PIN_RED2, HIGH);
            digitalWrite(PIN_YELLOW2, LOW);
            digitalWrite(PIN_GREEN2, LOW);
            digitalWrite(PIN_RED3, LOW);
            digitalWrite(PIN_YELLOW3, LOW);
            digitalWrite(PIN_GREEN3, HIGH);
            digitalWrite(PIN_RED4, HIGH);
            digitalWrite(PIN_YELLOW4, LOW);
            digitalWrite(PIN_GREEN4, LOW);
            break;          
            
          case 4:
            digitalWrite(PIN_RED1, HIGH);
            digitalWrite(PIN_YELLOW1, LOW);
            digitalWrite(PIN_GREEN1, LOW);
            digitalWrite(PIN_RED2, HIGH);
            digitalWrite(PIN_YELLOW2, LOW);
            digitalWrite(PIN_GREEN2, LOW);
            digitalWrite(PIN_RED3, HIGH);
            digitalWrite(PIN_YELLOW3, LOW);
            digitalWrite(PIN_GREEN3, LOW);
            digitalWrite(PIN_RED4, LOW);
            digitalWrite(PIN_YELLOW4, LOW);
            digitalWrite(PIN_GREEN4, HIGH);
            break;
        }
        for(int j=1; j<5; j++){
          if(j==4) done = true;
          charData = ' ';
        }
      }
    }
    idxBlink = 0;

    digitalWrite(PIN_RED1, LOW);
    digitalWrite(PIN_YELLOW1, LOW);
    digitalWrite(PIN_GREEN1, LOW);
    digitalWrite(PIN_RED2, LOW);
    digitalWrite(PIN_YELLOW2, LOW);
    digitalWrite(PIN_GREEN2, LOW);
    digitalWrite(PIN_RED3, LOW);
    digitalWrite(PIN_YELLOW3, LOW);
    digitalWrite(PIN_GREEN3, LOW);
    digitalWrite(PIN_RED4, LOW);
    digitalWrite(PIN_YELLOW4, LOW);
    digitalWrite(PIN_GREEN4, LOW);

    currentMillisSecond = millis();
    lastMillisSecond = currentMillisSecond;
    idxBlink = 5;
    while(idxBlink > 0){
      currentMillisSecond = millis();
      if ((unsigned long)(currentMillisSecond  - lastMillisSecond ) >= intervalSecond){ 
        display1.showNumberDec(idxBlink);
        display2.showNumberDec(idxBlink);
        display3.showNumberDec(idxBlink);
        display4.showNumberDec(idxBlink);
        idxBlink--;
        lastMillisSecond = currentMillisSecond;
      }
      digitalWrite(PIN_YELLOW1, HIGH);
      digitalWrite(PIN_YELLOW2, HIGH);
      digitalWrite(PIN_YELLOW3, HIGH);
      digitalWrite(PIN_YELLOW4, HIGH);
    }
}

void checkBlock1(){
    isObstacleA = digitalRead(SENSING1A);
    isObstacleB = digitalRead(SENSING1B);
    isObstacleC = digitalRead(SENSING1C);   
    
    GreenStep1 = Base;
    int margin = 0;
    if(isObstacleA == LOW) {
      margin = SENSOR_VALUE;
    }
    if(isObstacleB == LOW) {
      margin += SENSOR_VALUE;
    }
    if(isObstacleC == LOW){
      margin += SENSOR_VALUE;
    }
    isChecked1 = true; 

    if(margin>0){    
      GreenStep1 = margin;
      NumbStep2 += margin-Base;
      NumbStep3 += margin-Base;
    }
    RedStep2 = GreenStep1;
    RedStep3 = NumbStep2 + GreenStep2 + TransitionYellow;
    RedStep4 = NumbStep3 + GreenStep3 + TransitionYellow;
    String l = String(RedStep4);
    Serial.println(l);
}

void checkBlock2(){
    isObstacleA = digitalRead(SENSING2A);
    isObstacleB = digitalRead(SENSING2B);
    isObstacleC = digitalRead(SENSING2C);
        
    GreenStep2 = Base;
    int margin = 0;
    if(isObstacleA == LOW) {
      margin = SENSOR_VALUE;
    }
    if(isObstacleB == LOW) {
      margin += SENSOR_VALUE;
    }
    if(isObstacleC == LOW){
      margin += SENSOR_VALUE;
    }
    isChecked2 = true; 

    if(margin>0){    
      GreenStep2 = margin;
      NumbStep4 += margin-Base;
      NumbStep3 += margin-Base;
    }
    
    RedStep3 = GreenStep2;
    RedStep4 = NumbStep3 + GreenStep3 + TransitionYellow;
    RedStep1 = NumbStep4 + GreenStep4 + TransitionYellow;
}
void checkBlock3(){
    isObstacleA = digitalRead(SENSING3A);
    isObstacleB = digitalRead(SENSING3B);
    isObstacleC = digitalRead(SENSING3C);
        
    GreenStep3 = Base;
    int margin = 0;
    if(isObstacleA == LOW) {
      margin = SENSOR_VALUE;
    }
    if(isObstacleB == LOW) {
      margin += SENSOR_VALUE;
    }
    if(isObstacleC == LOW){
      margin += SENSOR_VALUE;
    }
    isChecked3 = true; 

    if(margin>0){    
      GreenStep3 = margin;
      NumbStep1 += margin-Base;
      NumbStep4 += margin-Base;
    }
    
    RedStep4 = GreenStep3;
    RedStep1 = NumbStep4 + GreenStep4 + TransitionYellow;
    RedStep2 = NumbStep1 + GreenStep1 + TransitionYellow;
}
void checkBlock4(){
    isObstacleA = digitalRead(SENSING4A);
    isObstacleB = digitalRead(SENSING4B);
    isObstacleC = digitalRead(SENSING4C);
        
    GreenStep4 = Base;
    int margin = 0;
    if(isObstacleA == LOW) {
      margin = SENSOR_VALUE;
    }
    if(isObstacleB == LOW) {
      margin += SENSOR_VALUE;
    }
    if(isObstacleC == LOW){
      margin += SENSOR_VALUE;
    }
    isChecked4 = true; 

    if(margin>0){    
      GreenStep4 = margin;
      NumbStep1 += margin-Base;
      NumbStep2 += margin-Base;
    }

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
      digitalWrite(PIN_RED1, Red1);
      digitalWrite(PIN_YELLOW1, Yellow1);
      digitalWrite(PIN_GREEN1, Green1);
      break;
      
    case 2:
      digitalWrite(PIN_RED2, Red2);
      digitalWrite(PIN_YELLOW2, Yellow2);
      digitalWrite(PIN_GREEN2, Green2);
      break;
      
    case 3:
      digitalWrite(PIN_RED3, Red3);
      digitalWrite(PIN_YELLOW3, Yellow3);
      digitalWrite(PIN_GREEN3, Green3);
      break;
      
    case 4:
      digitalWrite(PIN_RED4, Red4);
      digitalWrite(PIN_YELLOW4, Yellow4);
      digitalWrite(PIN_GREEN4, Green4);
  }
}



#include <TM1637Display.h>
#include <Arduino.h>
#define CLK 2
#define DIO1 3
#define DIO2 4
#define DIO3 5
#define DIO4 6
#define SENSING 10

const uint8_t SEG_DONE[] = {SEG_G,SEG_G,SEG_G,SEG_G,};
const uint8_t SEG_CLEAR[] = {0,0,0,0};

unsigned long interval = 1000;
unsigned long previousMillis = 0;
TM1637Display display1(CLK, DIO1);
TM1637Display display2(CLK, DIO2);
TM1637Display display3(CLK, DIO3);
TM1637Display display4(CLK, DIO4);
int NumbStep = 150;
int NumbStep2 = 150;
int NumbStep3 = 150;
int NumbStep4 = 150;
int LightState1 = 1;

void setup() {
  // put your setup code here, to run once:
  display1.setBrightness(0x00);  
  display2.setBrightness(0x00);
  display3.setBrightness(0x00);
  display4.setBrightness(0x00);//set the diplay to minimum brightness

}

void loop() {
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval){
    update_time();
    previousMillis = currentMillis;
  }
  // put your main code here, to run repeatedly:

}

void update_time(){
  //display(CLK, DIO1);76uuyyyjj65tx 
  if(LightState1 == 4 | LightState1 == 3){
    display1.setSegments(SEG_DONE);
  }else{
    display1.showNumberDec(NumbStep); 
    display2.showNumberDec(NumbStep);
    display3.showNumberDec(NumbStep);
    display4.showNumberDec(NumbStep);//Display the Variable value;
  }
  
  NumbStep--;
  NumbStep2--;
  NumbStep3--;
  NumbStep4--;
}

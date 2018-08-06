#define SENSING 10
#define DIO1 3
#include <Arduino.h>


int isObstacle = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIO1, OUTPUT); 
  pinMode(SENSING, INPUT);
  digitalWrite(DIO1, HIGH);
}

void loop() {
  
  if(isObstacle != digitalRead(SENSING)){
    isObstacle = digitalRead(SENSING);
  }
  
  if(isObstacle == LOW){
    digitalWrite(DIO1, HIGH);
  }else{
    digitalWrite(DIO1, LOW);
  }

}

#define SENSING 10
#define DIO1 13
#define RED 2
#define YELLOW 3
#define GREEN 4
#include <Arduino.h>


int isObstacle = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIO1, OUTPUT); 
  pinMode(RED, OUTPUT); 
  pinMode(YELLOW, OUTPUT); 
  pinMode(GREEN, OUTPUT); 
  pinMode(SENSING, INPUT);
  digitalWrite(DIO1, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, HIGH);
}

void loop() {
  
  if(isObstacle != digitalRead(SENSING)){
    isObstacle = digitalRead(SENSING);
  }
  
  if(isObstacle == HIGH){
    digitalWrite(DIO1, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
  }else{
    digitalWrite(DIO1, HIGH);
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, HIGH);
  }

}


unsigned long interval = 150000;
unsigned long interval2 = 500;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();
boolean isInDanger = false;
boolean isInDanger2 = false;
boolean isInDanger3 = false;

int pinBuzzer = A3;

int pinWarning1 = A0;
int pinWarning2 = A1;
int pinWarning3 = A2;

int pinEcho1 = 5;
int pinTrigger1 = 4;

int pinEcho2 = 7;
int pinTrigger2 = 6;

int pinEcho3 = 3;
int pinTrigger3 = 2;

float duration, distance;
float duration2, distance2;
float duration3, distance3;

int buzzerLoop = 5;

void setup() {
  Serial.begin(115200);
  Serial.print("BEGIN");
  pinMode(pinTrigger1, OUTPUT);
  pinMode(pinEcho1, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, HIGH);
  pinMode(pinWarning1, OUTPUT);
  digitalWrite(pinWarning1, LOW);
  pinMode(pinWarning2, OUTPUT);
  digitalWrite(pinWarning2, LOW);
  pinMode(pinWarning3, OUTPUT);
  digitalWrite(pinWarning3, LOW);

}

void loop() {
 if(isInDanger == true && isInDanger2 == true && isInDanger3 == true){
  sendWarning();
 }
 for(int i =0; i<3; i++){
   generatePulse(i);
   checkSensor(i);
   delay(500);
 }
}
void sendWarning(){
 for(int i =0; i<3; i++){
   turnBuzzerOn();
   delay(500);
 }
}

void checkSensor(int i){

  Serial.println("Sensor#"+String(i+1));
  Serial.print("Distance = ");

  switch(i){
    case 0:
      distance = (duration / 2) * 0.0333;
      if (distance >= 50 || distance <= 2) {
         Serial.print("Out of range ");
         Serial.print(distance);
         Serial.println(" cm");
         isInDanger = true;
      
         digitalWrite(pinWarning1, HIGH);
         delay(300);
      }
      else {
        Serial.print(distance);
        Serial.println(" cm");
        isInDanger = false;
      
        digitalWrite(pinWarning1, LOW);
        delay(300);
      }
      break;
    case 1:
      distance2 = (duration2 / 2) * 0.0333;
      if (distance2 >= 50 || distance2 <= 2) {
         Serial.print("Out of range ");
         Serial.print(distance2);
         Serial.println(" cm");
         isInDanger2 = true;
      
         digitalWrite(pinWarning2, HIGH);
        delay(300);
      }
      else {
        Serial.print(distance2);
        Serial.println(" cm");
        isInDanger2 = false;
        
        digitalWrite(pinWarning2, LOW);
        delay(300);
      }
      break;
    case 2:
      distance3 = (duration3 / 2) * 0.0333;
      if (distance3 >= 50 || distance3 <= 2) {
         Serial.print("Out of range ");
         Serial.print(distance3);
         Serial.println(" cm");
         isInDanger3 = true;
         
         digitalWrite(pinWarning3, HIGH);
        delay(300);
      }
      else {
        Serial.print(distance3);
        Serial.println(" cm");
        isInDanger3 = false;
        
        digitalWrite(pinWarning3, LOW);
        delay(300);
      }
      break;
  }
}
void generatePulse(int i){
  switch(i){
    case 0:
      digitalWrite(pinTrigger1, LOW);
      delayMicroseconds(2);
      digitalWrite(pinTrigger1, HIGH);
      delayMicroseconds(10);
      digitalWrite(pinTrigger1, LOW);
      duration = pulseIn(pinEcho1, HIGH);
      break;
    case 1:
      digitalWrite(pinTrigger2, LOW);
      delayMicroseconds(2);
      digitalWrite(pinTrigger2, HIGH);
      delayMicroseconds(10);
      digitalWrite(pinTrigger2, LOW);
      duration2 = pulseIn(pinEcho2, HIGH);
      break;
    case 2:
      digitalWrite(pinTrigger3, LOW);
      delayMicroseconds(2);
      digitalWrite(pinTrigger3, HIGH);
      delayMicroseconds(10);
      digitalWrite(pinTrigger3, LOW);
      duration3 = pulseIn(pinEcho3, HIGH);
      break;
    
  }
}

void turnBuzzerOn(){
  currentMillis2 = millis();
  if ((unsigned long)(currentMillis2  - previousMillis2 ) >= interval2){ 
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(pinWarning1, HIGH);
    digitalWrite(pinWarning2, HIGH);
    digitalWrite(pinWarning3, HIGH);
    delay(145);
    digitalWrite(pinBuzzer, HIGH);
    digitalWrite(pinWarning1, LOW);
    digitalWrite(pinWarning2, LOW);
    digitalWrite(pinWarning3, LOW);
    delay(125);
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(pinWarning1, HIGH);
    digitalWrite(pinWarning2, HIGH);
    digitalWrite(pinWarning3, HIGH);
    delay(145);
    digitalWrite(pinBuzzer, HIGH);
    digitalWrite(pinWarning1, LOW);
    digitalWrite(pinWarning2, LOW);
    digitalWrite(pinWarning3, LOW);
    delay(55);
    previousMillis2 = currentMillis2;
  }
}


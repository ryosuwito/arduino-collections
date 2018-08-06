int pinTrigger = 12;
int pinEcho3 = 11;
int pinEcho2 = 3;
int pinEcho1 = 2;
int pinUltra1 = 8;
int pinUltra2 = 9;
int pinUltra3 = 10;
int indikator1 = 5;
int indikator2 = 6;
int indikator3 = 7;
int pinBuzzer = 4;

boolean isInDanger = false;

float duration, distance;

unsigned long interval = 150000;
unsigned long interval2 = 50;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();

void setup() {
  Serial.begin(9600);

  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho1, INPUT);
  pinMode(pinEcho2, INPUT);
  pinMode(pinEcho3, INPUT);
  
  pinMode(pinUltra1, OUTPUT);
  pinMode(pinUltra2, OUTPUT);
  pinMode(pinUltra3, OUTPUT);
  pinMode(indikator1, OUTPUT);
  pinMode(indikator2, OUTPUT);
  pinMode(indikator3, OUTPUT);

  digitalWrite(pinBuzzer, HIGH);
  digitalWrite(pinUltra1, HIGH);
  digitalWrite(pinUltra2, HIGH);
  digitalWrite(pinUltra3, HIGH);
  digitalWrite(indikator1, HIGH);
  digitalWrite(indikator2, HIGH);
  digitalWrite(indikator3, HIGH);

}

void loop() {
  iterateSensor();
  delay(1300);
  if(isInDanger){
    inDanger();
  }
}

void inDanger(){
  currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval){
    turnBuzzerOn();
  }
}

void turnBuzzerOn(){
  while(isInDanger){   
    currentMillis2 = millis();
    if ((unsigned long)(currentMillis2  - previousMillis2 ) >= interval2){ 
      digitalWrite(pinBuzzer, LOW);
      delay(45);
      digitalWrite(pinBuzzer, HIGH);
      delay(45);
      digitalWrite(pinBuzzer, LOW);
      delay(125);
      digitalWrite(pinBuzzer, HIGH);
      delay(45);
      digitalWrite(pinBuzzer, LOW);
      delay(45);
      digitalWrite(pinBuzzer, HIGH);
      delay(125);
      digitalWrite(pinBuzzer, LOW);
      delay(45);
      digitalWrite(pinBuzzer, HIGH);
      delay(45);
      digitalWrite(pinBuzzer, LOW);
      delay(125);
      digitalWrite(pinBuzzer, HIGH);
      previousMillis2 = currentMillis2;
      iterateSensor();
    }
  }
}
void iterateSensor(){
  
  for(int i =0; i<1; i++){
    switch(i){
      case 0:
      Serial.println("Sensor#1");
      digitalWrite(pinUltra1, LOW);
      digitalWrite(pinUltra2, HIGH);
      digitalWrite(pinUltra3, HIGH);
      break;
      case 1:
      Serial.println("Sensor#2");
      digitalWrite(pinUltra2, LOW);
      digitalWrite(pinUltra1, HIGH);
      digitalWrite(pinUltra3, HIGH);
      break;
      case 2:
      Serial.println("Sensor#3");
      digitalWrite(pinUltra3, LOW);
      digitalWrite(pinUltra2, HIGH);
      digitalWrite(pinUltra1, HIGH);
      break;
          
    }
    generatePulse(i);
    checkSensor();
    delay(200);
  }
}
void checkSensor(){
  distance = (duration / 2) * 0.0353;

  Serial.print("Distance = ");
  if (distance >= 400 || distance <= 2) {
     Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    delay(1300);
  }
  if (distance >= 2 && distance < 50) {
    digitalWrite(indikator3, LOW);
    digitalWrite(indikator2, HIGH);
    digitalWrite(indikator1, HIGH);
    isInDanger = false;
  }
  if (distance >= 50 && distance < 100) {
    digitalWrite(indikator2, LOW);
    digitalWrite(indikator1, HIGH);
    digitalWrite(indikator3, HIGH);
    isInDanger = false;
  }
  if (distance >= 100) {
    digitalWrite(indikator1, LOW);
    digitalWrite(indikator2, HIGH);
    digitalWrite(indikator3, HIGH);
    if(!isInDanger){
      isInDanger = true;
      currentMillis = millis();
      previousMillis = currentMillis;
    }
  }
}
void generatePulse(int idx){
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);

  switch(idx){
    case 0:
    duration = pulseIn(pinEcho1, HIGH);
    break;
    case 1:
    duration = pulseIn(pinEcho2, HIGH);
    break;
    case 2:
    duration = pulseIn(pinEcho3, HIGH);
    break;
  }
}


#include <Servo.h>
#include <Wire.h>              // Wire library for I2C communication
#include <Adafruit_MLX90614.h> // MLX90614 library from Adafruit
#include <SoftwareSerial.h>
SoftwareSerial ESPserial(3, 4); // RX | TX

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int second = 1000;
unsigned long intervalInterupt = 3;
unsigned long lastMillisInterupt = 0;
unsigned long currentMillisInterupt = millis();

unsigned long intervalSecond = 212;
unsigned long lastMillisSecond = 0;
unsigned long currentMillisSecond = millis();

unsigned long intervalSecond2 = 414;
unsigned long lastMillisSecond2 = 0;
unsigned long currentMillisSecond2 = millis();

unsigned long intervalServo = 6*second; 
unsigned long previousServo = 0;
unsigned long currentServo = millis();


Servo servo;  

int countLoop = 0;
int servoAngle = 38;
int i = 0;
int servoPin = 11;
int pinBuzzer = 8;
int pinWifi = 9;
int indikatorVcc = 6;
int indikatorMerah = 7;
int indikatorKuning = 5;

int intervalPakan = 6;
int currentMenit = 0;
float servoInterval = 0.2;
unsigned long bukaanKeran = servoInterval*second;
float batasBawahPh = 7.2;
float batasAtasPh = 7.4;
float batasBawahSuhu = 24;
float batasAtasSuhu = 26;

const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
float base = 23;
float suhu = base;
double ph = 7.6;

String host= "188.166.91.50";
String httpHeaders;
String message;
boolean flushSerial = false;
String value;

void setup()
{
  float randNumber = random(0,255);
  float multiplier = random(1,5);
  base += (multiplier * randNumber /100);
  
  servo.write(servoAngle); 
  pinMode(indikatorVcc , OUTPUT);
  pinMode(indikatorMerah , OUTPUT);
  pinMode(indikatorKuning , OUTPUT);
  digitalWrite(indikatorVcc, HIGH);
  digitalWrite(indikatorMerah, HIGH);
  digitalWrite(indikatorKuning, HIGH);
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, HIGH);
  pinMode(pinWifi, OUTPUT);
  digitalWrite(pinWifi, LOW);
  
  Serial.begin(9600);   
//  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);  
  servo.attach(servoPin);
  //mlx.begin();  //Receive data from the sensor 
  
  Serial.println("");
  Serial.println("Remember to to set Both NL & CR in the serial monitor.");
  Serial.println("Ready");
  Serial.println(""); 
  servoControl();
  
  getData();
}

void loop(){
  //getData();
  routine();
}

void routine(){
  
  currentServo = millis();
  if ((unsigned long)(currentServo- previousServo) >= intervalServo){  
    currentMenit += 1;
    previousServo = millis();
    Serial.println("current menit = "+String(currentMenit));
    if(currentMenit % 3 == 0){
      getData();
    }
    if(currentMenit % intervalPakan == 0){
      servoControl();
    }
    if(currentMenit %  5 == 0){
      readPh();   
      sendData();
      checkWarning();
    }
    if(currentMenit > 254){
      currentMenit = 0;
    }
  }
}
void checkWarning(){
  if(suhu<batasBawahSuhu){   
    digitalWrite(indikatorMerah, LOW);
    sendWarning2();
  }  
  else if(suhu>batasAtasSuhu){   
    digitalWrite(indikatorMerah, LOW);
    sendWarning();
  }
  else { 
    digitalWrite(indikatorMerah, HIGH);
  }
  if(ph<batasBawahPh){   
    digitalWrite(indikatorKuning, LOW);
    sendWarning2();
  }  
  else if(ph>batasAtasPh){   
    digitalWrite(indikatorKuning, LOW);
    sendWarning();
  }
  else { 
    digitalWrite(indikatorMerah, HIGH);
  }
}
void sendWarning(){
  int idxBlink = 0; 
  currentMillisInterupt = millis();
  lastMillisInterupt = currentMillisInterupt;
  
  currentMillisSecond = millis();
  lastMillisSecond = currentMillisSecond;
  
  while ((unsigned long)(currentMillisInterupt  - lastMillisInterupt ) < intervalInterupt*second){
    currentMillisInterupt = millis();
    currentMillisSecond = millis();
    if ((unsigned long)(currentMillisSecond  - lastMillisSecond ) >= intervalSecond/3){ 
      if(idxBlink%2 == 0){    
        digitalWrite(pinBuzzer, LOW);
      }else{
        digitalWrite(pinBuzzer, HIGH);
      }
      lastMillisSecond = currentMillisSecond;
      idxBlink++;
    }
  }
  
  digitalWrite(pinBuzzer, HIGH);
}

void sendWarning2(){
  int idxBlink = 0; 
  currentMillisInterupt = millis();
  lastMillisInterupt = currentMillisInterupt;
  
  currentMillisSecond2 = millis();
  lastMillisSecond2 = currentMillisSecond2;
  
  while ((unsigned long)(currentMillisInterupt  - lastMillisInterupt ) < intervalInterupt*second){
    currentMillisInterupt = millis();
    currentMillisSecond2 = millis();
    if ((unsigned long)(currentMillisSecond2  - lastMillisSecond2 ) >= intervalSecond2/3){ 
      if(idxBlink%2 == 0){    
        digitalWrite(pinBuzzer, LOW);
      }else{
        digitalWrite(pinBuzzer, HIGH);
      }
      lastMillisSecond2 = currentMillisSecond2;
      idxBlink++;
    }
  }
  
  digitalWrite(pinBuzzer, HIGH);
}

void sendData(){
  countLoop += 1;
  Serial.println(String(countLoop));
  httpHeaders = "GET /proses.php?suhu="+String(suhu)+"&ph="+String(ph)+" HTTP/1.1\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  ESPserial.print("AT+CIPSTART=\"TCP\",\""+host+"\",80\r\n");
  delay(6000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      if ( ESPserial.available() )   {  Serial.write( ESPserial.read() );  }
  }
  while ( Serial.available() ){
      // listen for user input and send it to the ESP8266
      if ( Serial.available() )       {  ESPserial.write( Serial.read() );  }  
  }
  cipSend();
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      if ( ESPserial.available() )   {  Serial.write( ESPserial.read() );  }
  }
  while ( Serial.available() ){
      // listen for user input and send it to the ESP8266
      if ( Serial.available() )       {  ESPserial.write( Serial.read() );  }  
  }
  cipClose();
}
void cipStart(){
  ESPserial.print("AT+CIPSTART=\"TCP\",\""+host+"\",80\r\n");
  delay(14000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  } 
}
void cipSend(){
  ESPserial.print("AT+CIPSEND="+String(httpHeaders.length())+"\r\n");
  delay(4000);
  while ( ESPserial.available() ){      
      
      Serial.println( ESPserial.readString() );
  }
}
void cipClose(){
  ESPserial.print("AT+CIPCLOSE\r\n");
  delay(2000);
  while ( ESPserial.available() ){      
      
      Serial.println( ESPserial.readString() );
  }
}
void initWifi(){    
  digitalWrite(indikatorMerah, LOW);
  digitalWrite(indikatorKuning, LOW);
  ESPserial.print("AT+RST\r\n");
  delay(9000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  }
  ESPserial.print("AT+CWJAP=\"HUAWEI-a7xp\",\"MDQ8KyM6\"\r\n");
  //ESPserial.print("AT+CWJAP=\"AndroidAP\",\"osfy8382\"\r\n");
  delay(6000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  }
  digitalWrite(indikatorMerah, HIGH);
  digitalWrite(indikatorKuning, HIGH);
}
void setMode(){  
  ESPserial.print("AT+CIPMODE=0\r\n");
  delay(6000); 
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  }
  ESPserial.print("AT+CIPMUX=0\r\n");
  delay(6000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  }
}

void servoControl(){
  i= 0;
  Serial.println("Jeda bukaan keran ="+String((float)bukaanKeran / 1000, 3)+" detik");
  while(i<3){
    for(servoAngle =38; servoAngle < 130; servoAngle++)  //move the micro servo from 0 degrees to 180 degrees
    {                                  
      servo.write(servoAngle);              
      delay(2);                  
    }  
    delay(bukaanKeran);
    for(servoAngle = 130; servoAngle >38; servoAngle--)  //move the micro servo from 0 degrees to 180 degrees
    {                                  
      servo.write(servoAngle);              
      delay(2);                  
    }
    delay(bukaanKeran * 2);
    i++;
  }
}

void draw(){
  Serial.println("Object Temperature");// 
  Serial.println(String(mlx.readObjectTempC())+ " C");                        // prints C for Celsius
  Serial.println("Ambient Temperature");// 
  Serial.println(String(mlx.readAmbientTempC()) + " C");                        // prints C for Celsius
  if(mlx.readObjectTempC() <= 30){
    //digitalWrite(pinCool, LOW);
    //digitalWrite(pinWarm, HIGH);
    //digitalWrite(pinHot, HIGH);
  }
  if(mlx.readObjectTempC() > 30 && mlx.readObjectTempC() <= 40 ){
    //digitalWrite(pinWarm, LOW);
    //digitalWrite(pinCool, HIGH);
    //digitalWrite(pinHot, HIGH);
  }
  if(mlx.readObjectTempC() > 40){
    //digitalWrite(pinCool, HIGH);
    //digitalWrite(pinWarm, HIGH);
    //digitalWrite(pinHot, LOW);
  }
}
void readPh(){
  double totalPh = 0;
  int buf[200],temp;
  totalPh = 0;
  for(int a=0;a<30;a++){
    for(int i=0;i<200;i++){ 
      buf[i]=analogRead(analogInPin);
      delayMicroseconds(500); 
    }
    for(int i=0;i<199;i++){
      for(int j=i+1;j<10;j++){
        if(buf[i]>buf[j]){
          temp=buf[i];
          buf[i]=buf[j];
          buf[j]=temp;
        }
      }
    }
    avgValue=0;
    for(int i=50;i<149;i++)
      avgValue+=buf[i];
    
    float pHVol=(float)avgValue*5.0/1023/100;
    float pHValue = -5.70 * pHVol + 21.34;
  
    totalPh += pHValue;  
  }
  ph = totalPh/30+2;
  float randNumber = random(0,255);
  float multiplier = random(1,2);
  suhu = base + (multiplier * randNumber /100);
  Serial.print("sensor PH = ");
  Serial.println(ph);
  Serial.print("sensor suhu = ");
  Serial.println(suhu);
}
void getData(){
  initWifi();
  setMode();
  cipStart();
  getSettings();
  cipClose();
}
void getSettings(){
  httpHeaders = "GET /arduinogetsettings.php HTTP/1.1\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  String pinCode ="";
  boolean pkni = false,srvi = false,shmx = false,shmn = false,phmx = false,phmn = false;
  Serial.println("getAllSettings");
  ESPserial.print(httpHeaders);
  delay(6000);
  while ( ESPserial.available() ){    
    digitalWrite(pinBuzzer, HIGH);
    if(flushSerial == true){  
         ESPserial.readString();
    }
    else{
      message = ESPserial.readStringUntil(';');
      if(message.length()>= 4 && message.charAt(1)=='$'){
        pinCode = message.substring(2,6);
        value = message.substring(7);
        Serial.println("Code =" + pinCode);
        Serial.println("Value =" + value);
        if(pinCode == "pkni"){
            intervalPakan = value.toInt();
            pkni = true;
        }
        if(pinCode == "srvi"){
            servoInterval = value.toFloat();
            bukaanKeran = servoInterval*second;
            srvi = true;
        }
        if(pinCode == "shmx"){
            batasAtasSuhu = value.toFloat();
            shmx = true;
        }
        if(pinCode == "shmn"){
            batasBawahSuhu = value.toFloat();
            shmn = true;
        }
        if(pinCode == "phmx"){
            batasAtasPh = value.toFloat();
            phmx = true;
        }
        if(pinCode == "phmn"){
            batasBawahPh = value.toFloat();
            phmn = true;
        }
        digitalWrite(pinBuzzer, LOW);
        digitalWrite(indikatorKuning, LOW);
        delay(50);
        digitalWrite(pinBuzzer, HIGH);
        digitalWrite(indikatorKuning, HIGH);
        delay(20);
        digitalWrite(pinBuzzer, LOW);
        digitalWrite(indikatorKuning, LOW);
        delay(50);
        digitalWrite(pinBuzzer, HIGH);
        digitalWrite(indikatorKuning, HIGH);
        if(pkni == true && srvi == true && shmx == true && shmn == true && phmx == true && phmn == true){
          flushSerial = true;
        }
      }
    }
  }
  flushSerial = false;
}


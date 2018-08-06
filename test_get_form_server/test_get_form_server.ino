// Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the ESP8266
//
 
int countLoop = 0;
String host= "188.166.91.50";
//String host= "128.199.72.5";
String httpHeaders;
String message;
String messageMin;
String messagePh;
String messagePhMin;
String messageServo;
String messageServoInterval;
boolean flushSerialServo = false;
boolean flushSerialServoInterval = false;
boolean flushSerial = false;
boolean flushSerialMin = false;
boolean flushSerialPh = false;
boolean flushSerialPhMin = false;
int pinBuzzer = 8;
String suhuMaks;
String suhuMin;
String phMin;
String phMaks;
String servoValue;
String servoValueInterval;
#include <SoftwareSerial.h>
SoftwareSerial ESPserial(3, 4); // RX | TX
 
void setup() 
{
    Serial.begin(9600);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600);  
 
    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");    
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, HIGH);
    ESPserial.setTimeout(5000);
    initWifi();
    setMode();
}
 
void loop() 
{
    getData();    
}

void getData(){
  cipStart();
  cipStatus();
  countLoop += 1;
  Serial.println(String(countLoop));
  getPakanInterval();
  getServoInterval();
  getSuhuMaks();
  getSuhuMin();
  getPhMaks();
  getPhMin();
  cipClose();
}
void getPakanInterval(){
  httpHeaders = "GET /getpakaninterval.php HTTP/1.1\r\n";
  httpHeaders += "Connection: keep-alive\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  
  Serial.println("getPakanInterval");
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){    
        digitalWrite(pinBuzzer, HIGH);
        if(flushSerialServo == true){  
             ESPserial.readString();
        }
        else{
          messageServo = ESPserial.readStringUntil(';');
          if(messageServo.length()>= 4 && messageServo.charAt(1)=='$'){
            servoValue = messageServo.substring(7);
            Serial.println("Value =" + servoValue);
            digitalWrite(pinBuzzer, LOW);
            delay(600);
            digitalWrite(pinBuzzer, HIGH);
            delay(100);
            digitalWrite(pinBuzzer, LOW);
            delay(600);
            digitalWrite(pinBuzzer, HIGH);
            flushSerialServo = true;
          }
        }
  }
  flushSerialServo = false;
}
void getServoInterval(){
  httpHeaders = "GET /getservointerval.php HTTP/1.1\r\n";
  httpHeaders += "Connection: keep-alive\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  
  Serial.println("getServoInterval");
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){    
        digitalWrite(pinBuzzer, HIGH);
        if(flushSerialServoInterval == true){  
             ESPserial.readString();
        }
        else{
          messageServoInterval = ESPserial.readStringUntil(';');
          if(messageServoInterval.length()>= 4 && messageServoInterval.charAt(1)=='$'){
            servoValueInterval = messageServoInterval.substring(7);
            Serial.println("Value =" + servoValueInterval);
            digitalWrite(pinBuzzer, LOW);
            delay(100);
            digitalWrite(pinBuzzer, HIGH);
            delay(300);
            digitalWrite(pinBuzzer, LOW);
            delay(400);
            digitalWrite(pinBuzzer, HIGH);
            flushSerialServoInterval = true;
          }
        }
  }
  flushSerialServoInterval = false;
}

void getSuhuMaks(){
  httpHeaders = "GET /getsuhumaks.php HTTP/1.1\r\n";
  httpHeaders += "Connection: keep-alive\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  
  Serial.println("getSuhuMaks");
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){    
        digitalWrite(pinBuzzer, HIGH);
        if(flushSerial == true){  
             ESPserial.readString();
        }
        else{
          message = ESPserial.readStringUntil(';');
          if(message.length()>= 4 && message.charAt(1)=='$'){
            suhuMaks = message.substring(7);
            Serial.println("Value =" + suhuMaks);
            digitalWrite(pinBuzzer, LOW);
            delay(200);
            digitalWrite(pinBuzzer, HIGH);
            flushSerial = true;
          }
        }
  }
  flushSerial = false;
}

void getSuhuMin(){
  httpHeaders = "GET /getsuhumin.php HTTP/1.1\r\n";
  httpHeaders += "Connection: keep-alive\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  
  Serial.println("getSuhuMin");
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){     
        digitalWrite(pinBuzzer, HIGH);
        if(flushSerialMin == true){  
             ESPserial.readString();
        }
        else{
          messageMin = ESPserial.readStringUntil(';');
          if(messageMin.length()>= 4 && messageMin.charAt(1)=='$'){  
            suhuMin = messageMin.substring(7);
            Serial.println("Value =" + suhuMin);
            digitalWrite(pinBuzzer, LOW);
            delay(700);
            digitalWrite(pinBuzzer, HIGH);
            flushSerialMin = true;
          }
          else{
          } 
        }
  }
  flushSerialMin = false;
}

void getPhMaks(){
  httpHeaders = "GET /getphmaks.php HTTP/1.1\r\n";
  httpHeaders += "Connection: keep-alive\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  
  Serial.println("getPhMaks");
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){    
        digitalWrite(pinBuzzer, HIGH);
        if(flushSerialPh == true){  
             ESPserial.readString();
        }
        else{
          messagePh = ESPserial.readStringUntil(';');
          if(messagePh.length()>= 4 && messagePh.charAt(1)=='$'){
            phMaks = messagePh.substring(7);
            Serial.println("Value =" + phMaks);
            digitalWrite(pinBuzzer, LOW);
            delay(500);
            digitalWrite(pinBuzzer, HIGH);
            delay(500);
            digitalWrite(pinBuzzer, LOW);
            delay(500);
            digitalWrite(pinBuzzer, HIGH);
            flushSerialPh = true;
          }
        }
  }
  flushSerialPh = false;
}

void getPhMin(){
  httpHeaders = "GET /getphmin.php HTTP/1.1\r\n";
  httpHeaders += "Connection: keep-alive\r\n";
  httpHeaders += "Host: smartkoi.topters.us\r\n\r\n";
  cipSend();
  
  Serial.println("getPhMin");
  ESPserial.print(httpHeaders);
  delay(4000);
  while ( ESPserial.available() ){    
        digitalWrite(pinBuzzer, HIGH);
        if(flushSerialPhMin == true){  
             ESPserial.readString();
        }
        else{
          messagePhMin = ESPserial.readStringUntil(';');
          if(messagePhMin.length()>= 4 && messagePhMin.charAt(1)=='$'){
            phMin = messagePhMin.substring(7);
            Serial.println("Value =" + phMin);
            digitalWrite(pinBuzzer, LOW);
            delay(200);
            digitalWrite(pinBuzzer, HIGH);
            delay(200);
            digitalWrite(pinBuzzer, LOW);
            delay(200);
            digitalWrite(pinBuzzer, HIGH);
            flushSerialPhMin = true;
          }
        }
  }
  flushSerialPhMin = false;
}
void cipStart(){
  ESPserial.print("AT+CIPSTART=\"TCP\",\""+host+"\",80\r\n");
  delay(4000);
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
void cipStatus(){ 
  ESPserial.print("AT+CIPSTATUS\r\n");
  delay(4000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString());
  }
}
void cipClose(){
  ESPserial.print("AT+CIPCLOSE\r\n");
  delay(4000);
  while ( ESPserial.available() ){      
      
      Serial.println( ESPserial.readString() );
  }
}
void initWifi(){    
  ESPserial.print("AT+RST\r\n");
  delay(12000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  }
  ESPserial.print("AT+CWJAP=\"HUAWEI-a7xp\",\"MDQ8KyM6\"\r\n");
  delay(9000);
  while ( ESPserial.available() ){      
      // listen for communication from the ESP8266 and then write it to the serial monitor
      Serial.println( ESPserial.readString() );
  }
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


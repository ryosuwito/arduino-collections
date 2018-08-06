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


void setup()
{
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.begin(9600);    // communication with the host computer
  // Start the software serial for communication with the ESP8266
  //Serial3.begin(115200);

  delay(1000);

  Serial.println("Setting Baudrate to 9600");
  //Serial3.println("AT+CWMODE=1");
  //Serial3.println("AT+UART_DEF=9600,8,1,0,0");

  Serial3.begin(9600);
  Serial3.println("AT+GMR");
  delay(15000);
  Serial.println("");
  Serial.println("Remember to to set Both NL & CR in the serial monitor.");
  Serial.println("Ready");
  Serial.println("");
  Serial3.println("AT+CWJAP=\"HUAWEI-a7xp\",\"MDQ8KyM6\"");
  delay(15000);
  Serial3.println("AT+CIFSR");

  Serial3.println("AT+CIPSTATUS");

  if ( Serial3.available() ){
    Serial.write( Serial3.read() );
  }
}

void loop()
{
  // listen for communication from the ESP8266 and then write it to the serial monitor
  if ( Serial3.available() )   {
    Serial.write( Serial3.read() );
  }

  // listen for user input and send it to the ESP8266
  if ( Serial.available() )       {
    Serial3.write( Serial.read() );
  }
}

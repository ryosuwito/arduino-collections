#include <Wire.h>
#define slaveAddress 0x5A

void draw(void) 
{   
}

void setup(void) 
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(slaveAddress);
}

void loop(void) 
{
  byte dataLow = 0x00;
  byte dataHigh = 0x00;
  delay(10);

  Wire.write(0x07);    // This is the command to view object temperature in the sensor's RAM
  delay(10);

  Wire.endTransmission(false);
  delay(10);

  Wire.requestFrom(slaveAddress, 2);
  delay(10);
  
  while (Wire.available()){
    dataLow = Wire.read();
    dataHigh = Wire.read();
  }
  delay(10);
  
  double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
  double tempData = 0x0000; // zero out the data

   tempData = (double)(((dataHigh & 0x007F) << 8) + dataLow);
  tempData = (tempData * tempFactor)-0.01;

  float celcius = tempData - 273.15;
  float fahrenheit = (celcius*1.8) + 32;

  Serial.print((String) celcius);
  Serial.println("F");

  delay(100);    
  
  delay(2000);  // Delay of 1sec 
}

                       /*END OF FILE*/

const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[500],temp;
int bufPh[10], tempPh;
float base = 25;
float suhu = base;
float ph = 7.6;
float totalPh = 0;
void setup() {
 // initialize serial communication at 9600 bits per second:
 Serial.begin(9600);
}

// the loop routine runs over and over showing the voltage on A0
void loop() {
  totalPh = 0;
  for(int a=0;a<10;a++){
    for(int i=0;i<500;i++){ 
      buf[i]=analogRead(analogInPin);
      delayMicroseconds(500); 
    }
    for(int i=0;i<499;i++){
      for(int j=i+1;j<10;j++){
        if(buf[i]>buf[j]){
          temp=buf[i];
          buf[i]=buf[j];
          buf[j]=temp;
        }
      }
    }
    avgValue=0;
    for(int i=200;i<299;i++)
      avgValue+=buf[i];
    
    float pHVol=(float)avgValue*5.0/1023/100;
    float pHValue = -5.70 * pHVol + 21.34;
  
    totalPh += pHValue;  
  }
  ph = totalPh/10;
  Serial.print("sensor PH = ");
  Serial.println(ph);
}

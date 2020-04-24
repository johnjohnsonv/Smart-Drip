#define BLYNK_PRINT Serial
#include "DHT.h" 
#include <SimpleTimer.h>// including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define dht_dpin 0
int RESET =D2;

DHT dht(dht_dpin, DHTTYPE);
BlynkTimer timer; 
char auth[] = "EvWHyJcg_3kcdB1HXHnv4RWCYa5leSNM";  //code sent via email
const int sensorCaliberation = A0; 
int randomValue ;
int calibValue;
int caliberatedValue;



void setup()
{ 
  Serial.begin(9600);
  Blynk.begin(auth, "Corona", "344Ahostel");  //wifi name and password
  dht.begin();
  timer.setInterval(2000, sendUptime);

  Serial.println("Reading From the Sensor ...");
  Serial.println("Caliberation starting");
  calibValue= analogRead(sensorCaliberation);
  delay(2000);
  pinMode(13,OUTPUT);
  pinMode(RESET,OUTPUT);
  delay(700);

}

void sendUptime(){
  float h = dht.readHumidity();
 float t = dht.readTemperature(); 
 Serial.println("Humidity and temperature\n\n");
 Serial.print("Current humidity = ");
 Serial.print(h);
 Serial.print("% ");
 Serial.print("temperature = ");
 Serial.print(t); 
 Blynk.virtualWrite(V0, t);
 Blynk.virtualWrite(V1, h);
 randomValue= analogRead(sensorCaliberation);
  caliberatedValue = map(randomValue,2*calibValue,calibValue,0,100);
  Blynk.virtualWrite(V3,caliberatedValue);
  Blynk.virtualWrite(V4,calibValue);
  if(caliberatedValue > 100)
  {caliberatedValue =100;}
  
  Serial.print("Moisture: ");
  Serial.print(caliberatedValue);
  Serial.print("% Field Capacity:");
  Serial.print("100%:");
  Serial.print(calibValue);
  Serial.print(" Current Value:");
  Serial.print(randomValue);
  Serial.println("");
  
  if(t < 40 && caliberatedValue < 0){
   while(caliberatedValue < 90){
    Blynk.virtualWrite(V2, 1);
     digitalWrite(13,HIGH);// turn the LED on (HIGH is the voltage level)
     randomValue= analogRead(sensorCaliberation);
    caliberatedValue = map(randomValue,2*calibValue,calibValue,0,100);
    Blynk.virtualWrite(V3,caliberatedValue);
  Blynk.virtualWrite(V4,calibValue);
    if(caliberatedValue > 100)
  {caliberatedValue =100;}
  
  Serial.print("Moisture: ");
  Serial.print(caliberatedValue);
  Serial.print("% Field Capacity:");
  Serial.print("100%:");
  Serial.print(calibValue);
  Serial.print(" Current Value:");
  Serial.print(randomValue);
  Serial.println("");
      Blynk.virtualWrite(V3,caliberatedValue);
   }   
  }
  else{
     Blynk.virtualWrite(V2, 0);
     digitalWrite(13,LOW);// turn the LED on (HIGH is the voltage level)
     
  }
  
  Blynk.virtualWrite(V5, 1);
}
void loop() {
    Blynk.run();
    timer.run();
    delay(1000);
}
BLYNK_WRITE(V5){
  int pinValue=param.asInt();
  if(pinValue==1){
    digitalWrite(RESET,HIGH);
    ESP.restart();
  }else{
    digitalWrite(RESET,LOW);
  }
}
BLYNK_WRITE(V6){
  if(digitalRead(13) == LOW){
    digitalWrite(13,HIGH);
  }else{
    digitalWrite(13,LOW);
  }
}

#include <dht.h>

#define dht_dpin A3
dht DHT;

void setup()
{
  Serial.begin(9600);   
  delay(300);//Let system settle   
  Serial.println("Humidity and temperature\n\n");   
  delay(700);//Wait rest of 1000ms recommended delay before   
  //accessing sensor   
}

void loop()
{
  DHT.read11(dht_dpin);   
    
  Serial.print("Current humidity = ");   
  Serial.print(DHT.humidity);   
  Serial.print("% ");   
  Serial.print("temperature = ");   
  Serial.print(DHT.temperature);   
  Serial.println("C ");   
  delay(1000);
}

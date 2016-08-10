int photocellPin = 2; // photocell on analog pin 2
int photocellVal = 0; // photocell variable
int minLight = 600;   // threshold
int ledstripPin = 12;
int ledState = 0; 

void setup() {
  pinMode(ledstripPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read value from photocell and output to serial port 
  photocellVal = analogRead(photocellPin);
  Serial.println(photocellVal);   
  
  // light on led strip when threshold arrived
  if (photocellVal < minLight && ledState == 0) {
    digitalWrite(ledstripPin, HIGH); // turn on LED
    ledState = 1;
  }
  
  // light off led strip when out of threshold 
  if (photocellVal > minLight && ledState == 1) {
    digitalWrite(ledstripPin, LOW); // turn off LED
    ledState = 0;
  }
  delay(100);       
}

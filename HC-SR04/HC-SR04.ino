// pins numbers
const int trigPin = 12;
const int echoPin = 13;
const int vccPin= 11;

// HC-SR04 modules is plugged directly into the arduino
// GND -> GND
// ECHO -> Pin 13
// TRIG -> Pin 12
// VCC -> Pin 11

void setup() 
{
  pinMode(vccPin, OUTPUT); 
  digitalWrite(vccPin, HIGH); // power on the vccPin
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600); // Starts the serial communication
}

void loop() 
{
  long duration;
  int distance;
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
 
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(200);
}

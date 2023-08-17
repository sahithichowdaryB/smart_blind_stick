#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11); // RX, TX pins for GSM module
int _timeout;
String _buffer;
String number = "+919955993622"; // change with your number
const int buttonPin = 8;
bool buttonState = false;
bool previousButtonState = false;
const int waterSensorPin = A0; // Analog input pin for the water sensor
const int trigPin1 = 2; // Ultrasonic sensor 1 trig pin
const int echoPin1 = 3; // Ultrasonic sensor 1 echo pin
const int trigPin2 = 4; // Ultrasonic sensor 2 trig pin
const int echoPin2 = 5; // Ultrasonic sensor 2 echo pin
const int buzzerPin = 6; // Digital output pin for the buzzer
const int thresholdDistance = 75; // Threshold distance in centimeters (2 meters = 
200 centimeters)
void setup() {
 delay(7000);
 Serial.begin(9600);
 _buffer.reserve(50);
 Serial.println("System Started...");
 sim.begin(9600);
 delay(1000);
 pinMode(buttonPin, INPUT_PULLUP);
 pinMode(waterSensorPin, INPUT); // Set the water sensor pin as input
 pinMode(trigPin1, OUTPUT); // Set the ultrasonic sensor 1 trig pin as output
 pinMode(echoPin1, INPUT); // Set the ultrasonic sensor 1 echo pin as input
 pinMode(trigPin2, OUTPUT); // Set the ultrasonic sensor 2 trig pin as output
 pinMode(echoPin2, INPUT); // Set the ultrasonic sensor 2 echo pin as input
 pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output
 Serial.println("Press the button to send a message");
}
void loop() {
 buttonState = digitalRead(buttonPin);
 if (buttonState != previousButtonState) {
 if (buttonState == LOW) {
 SendMessage();
 }
 previousButtonState = buttonState;
 }
 if (sim.available() > 0) {
 Serial.write(sim.read());
 }
 // Read the analog value from the water sensor
 int waterSensorValue = analogRead(waterSensorPin);
 // Check if water is detected
 if (waterSensorValue < 500) { // Adjust this threshold value according to your 
sensor
 digitalWrite(buzzerPin, LOW); // Activate the buzzer
 Serial.println("Water not detected!"); // Print a message to the Serial Monitor
 } else {
 Serial.println("Water detected!");
 digitalWrite(buzzerPin, HIGH); // Deactivate the buzzer
 }
 // Measure distance from ultrasonic sensor 1
 long duration1, distance1;
 digitalWrite(trigPin1, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH);
 distance1 = duration1 * 0.034 / 2;
 // Measure distance from ultrasonic sensor 2
 long duration2, distance2;
 digitalWrite(trigPin2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH);
 distance2 = duration2 * 0.034 / 2;
 // Print the distances on the serial monitor
 Serial.print("Distance 1: ");
 Serial.print(distance1);
 Serial.print(" cm\t");
 Serial.print("Distance 2: ");
 Serial.print(distance2);
 Serial.println(" cm");
 // Check if any of the distances cross the threshold
 if (distance1 < thresholdDistance || distance2 < thresholdDistance) {
 // If threshold is crossed, activate the buzzer
 digitalWrite(buzzerPin, HIGH);
 } else {
 // Otherwise, turn off the buzzer
 digitalWrite(buzzerPin, LOW);
 }
 // Delay before taking the next reading
 delay(11);
}
void SendMessage() {
 sim.println("AT+CMGF=1"); // Sets the GSM Module in Text Mode
 delay(1000);
 sim.println("AT+CMGS=\"" + number + "\"\r"); // Mobile phone number to send 
message
 delay(1000);
 String SMS = "I need help!"; // Modify the message content as per your needs
 sim.println(SMS);
 delay(100);
 sim.println((char)26); // ASCII code of CTRL+Z
 delay(1000);
 _buffer = _readSerial();
 Serial.println("**********Message sent!************");
}
String _readSerial() {
 _timeout = 0;
 while (!sim.available() && _timeout < 12000) {
 delay(13);
 _timeout++;
 }
 if (sim.available()) {
 return sim.readString();
 }
}


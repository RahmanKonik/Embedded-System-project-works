#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "LIDARLite_v4LED.h"

LIDARLite_v4LED myLIDAR;

const int trigPin = 9;          // Trigger pin for the ultrasonic sensor
const int echoPin = 10;         // Echo pin for the ultrasonic sensor
const int potPin = A0;          // Potentiometer pin for controlling the sensor angle
int width = 0;                  // Width of the rectangle (in cm)
int height = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(20,3);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Wire.begin(); //Join I2C bus

  //check if LIDAR will acknowledge over I2C
  if (myLIDAR.begin() == false) {
    lcd.println("Device did not acknowledge! Freezing.");
    while(1);
  }
}

void loop() {

  lcd.setCursor(0,0);

  float newDistance = myLIDAR.getDistance();
  //lcd.println("Dist. "+ String(newDistance)+ " cm");

  delay(2000);
  width = newDistance;
  lcd.print("Width: ");
  lcd.print(width);

  // Measure the height of the rectangle
  while (analogRead(potPin) < 1000) {
    delay(100);
  }
  height = newDistance;
  lcd.print("Height: ");
  lcd.print(height);
  

  // Calculate the area of the rectangle and display the result
  float area = width * height;
  lcd.print("Area: ");
  lcd.print(area);
  
  
  delay(100);
  lcd.clear();


}
/* 
int measureDistance() {
  // Measure the distance using the ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Wait until the sensor provides a stable measurement
  int distance2 = distance;
  do {
    delay(50);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
  } while (abs(distance - distance2) > 1);

  return distance;
} */

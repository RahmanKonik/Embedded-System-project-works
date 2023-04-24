#include <LiquidCrystal.h>
#include <TimerOne.h>

// Define LCD pins
//LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define button pin
volatile int pulseCount = 8;
volatile byte time = 0;
volatile int buttonPin = 2;


// Define global variables
volatile float frequency = 0;  // calculated frequency
bool showFrequency = true;  // flag to indicate whether to display frequency or wind speed

void pulse_isr() {
  pulseCount++;  // increment pulse counter
}

void timerInter() {
  time++;
}

void setup() {
  
  pinMode(pulseCount, INPUT);  // set pin 2 as input
  attachInterrupt(digitalPinToInterrupt(pulseCount), pulse_isr, RISING);  // attach ISR to rising edge of pin 2
 
  Timer1.initialize(100000);//set timer interrupt in microsecond
  Timer1.attachInterrupt(timerInter);
 
  lcd.begin(16, 4);  // initialize LCD
}

void loop()
{
  if (digitalRead(buttonPin) == HIGH) {
    showFrequency = !showFrequency; // toggle showFrequency flag
    delay(100); // debounce button
  }
 
  if (showFrequency) {
    frequency = pulseCount / (time * 0.1); // calculate frequency
    lcd.setCursor(0,0);
    lcd.print("Frequency: ");
    lcd.print(frequency);
    lcd.print(" Hz   ");
  } else {
    // display wind speed
    float wind = -0.24 + (frequency * 0.699);
    lcd.setCursor(0,0);
    lcd.print("Wind speed: ");
    lcd.print(wind);
    lcd.print("m/s");
   
  }
 
  delay(500); // update LCD every 500ms
}
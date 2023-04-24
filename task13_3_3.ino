
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define button pin
const int pulseCountPin = 7;
volatile byte time = 0;
const int buttonPin = 2;

volatile boolean buttonPressedA = false;
volatile boolean buttonPressedB = false;
volatile boolean buttonPressedC = false;


// Define global variables
volatile float frequency = 0; 

int ANALOG_PIN = A2;
volatile unsigned long pulseCount = 0;
volatile boolean ButtonPressed;
int ButtonState = 0;
int previousButtonState = 0;

int degreeValue;
String windDirection;

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {45, 44, 43, 42}; 
byte colPins[COLS] = {41, 40, 39, 38}; 

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

float wind_speed_avg = 0.0;


void setup() {

  pinMode(pulseCountPin, INPUT);  // set pin 2 as input
  attachInterrupt(digitalPinToInterrupt(pulseCountPin), pulse_isr, RISING);  // attach ISR to rising edge of pin 2
 
  Timer1.initialize(100000);//set timer interrupt in microsecond
  Timer1.attachInterrupt(timerInter);
 
  lcd.begin(16, 4);  // initialize LCD

}
void pulse_isr() {
  pulseCount++;  // increment pulse counter
}

void timerInter() {
  time++;
  
  if(time > 9){

    frequency = pulseCount / (time * 0.1);
    float wind = - 0.24 + (frequency * 0.699);
   /*  lcd.setCursor(0,0);
    lcd.print("Wind speed: "); */
    
    float wind_speed_sum = 0.0;

    for (int i = 0; i < 10; i++) {
      wind_speed_sum += analogRead(ANALOG_PIN);
    }
    wind_speed_avg = (wind_speed_sum / wind) * (5.0 / 1024.0);
  }
}

void loop() {

  char key = keypad.getKey();

  float voltageValue = analogRead(ANALOG_PIN) * ( 5.0 / 1024);

  if (voltageValue < 1.2) {
      degreeValue = 0;
    } else{
      degreeValue = ((voltageValue - 1.2) / 3.8) * 360;
    }

 if(key == 'A'){
    buttonPressedA = true;
    buttonPressedB = false;
    buttonPressedC = false;

  } else if(key == 'B'){
    buttonPressedA = false;
    buttonPressedB = true;
    buttonPressedC = false;

  } 




  if (buttonPressedA == true) {
    
    lcd.clear();
   /*  frequency = pulseCount / (time * 0.1);
    float wind = -0.24 + (frequency * 0.699);
    lcd.setCursor(0,0);
    lcd.print("Wind speed: ");
    

    float wind_speed_sum = 0.0;

    for (int i = 0; i < wind; i++) {
      wind_speed_sum += analogRead(ANALOG_PIN);
    }
    wind_speed_avg = (wind_speed_sum / wind) * (5.0 / 1024.0); */
    lcd.setCursor(0,0);
    lcd.print("Wind speed: ");
    lcd.print(wind_speed_avg);
    lcd.print("m/s");
  } else if(buttonPressedB == true){
    lcd.clear();
    if (voltageValue < 1.2) {
      lcd.print("Passing");
    } else if (degreeValue >= 337.5 || (degreeValue <= 22.5 && degreeValue > 0)) {
      windDirection = "N";
    } else if (degreeValue > 22.5 && degreeValue <= 67.5) {
      windDirection = "NE";
    } else if (degreeValue > 67.5 && degreeValue <= 112.5) {
      windDirection = "E";
    } else if (degreeValue > 112.5 && degreeValue <= 157.5) {
      windDirection = "SE";
    } else if (degreeValue > 157.5 && degreeValue <= 202.5) {
      windDirection = "S";
    } else if (degreeValue > 202.5 && degreeValue <= 247.5) {
      windDirection = "S";
    } else if (degreeValue > 247.5 && degreeValue <= 292.5) {
      windDirection = "W";
    } else {
      windDirection = "NW";
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Direction: ");
    lcd.print(windDirection);
    lcd.setCursor(0, 1);
    lcd.print("Deg.: ");
    lcd.print(degreeValue);
  } 
    

    delay(1000);
}
      





  
#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ANALOG_PIN = A2;
const int buttonPin = 2;

volatile boolean ButtonPressed;
int ButtonState = 0;
int previousButtonState = 0;

int degreeValue;
String windDirection;
bool DisplayDirection = true;


void setup() {
  
  lcd.begin(20, 4);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

}


void loop() {

  lcd.setCursor(0, 0);
  ButtonState = digitalRead(buttonPin);

  if (ButtonState != previousButtonState) {
    if (ButtonState == LOW) {
      DisplayDirection = !DisplayDirection;
      lcd.clear();
    }
  }
  previousButtonState = ButtonState;

  float voltageValue = analogRead(ANALOG_PIN) * ( 5.0 / 1024);

  if (voltageValue < 1.2) {
      degreeValue = 0;
    } else{
      degreeValue = ((voltageValue - 1.2) / 3.8) * 360;
    }
  
  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(voltageValue);
  

  if (voltageValue < 1.2) {
    lcd.print("Passing");
  } else {
    lcd.print(degreeValue);
  }

  if (DisplayDirection) {
    lcd.setCursor(0, 0);

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
    lcd.print("Dir.: ");
    lcd.print(windDirection);
    } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(degreeValue);
    lcd.print(" degrees");
    delay(500);
  }
 
}


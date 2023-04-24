#include <Keypad.h>
#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

void setup(){
  Serial.begin(9600);
  lcd.begin(20,4);
}
  
void loop(){
  char customKey = keypad.getKey();
  
  if (customKey != NO_KEY){

    Serial.println(customKey);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Key_Pressed: ");
    lcd.print(customKey);
  }
}
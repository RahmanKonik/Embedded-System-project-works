
#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ANALOG_PIN = A2;

void setup() {
  lcd.begin(20, 4);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  
  digitalWrite(6, 1);

}


void loop() {

  lcd.setCursor(0, 0);

  float voltage_value = analogRead(ANALOG_PIN) * ( 5.0 / 1024);

  lcd.println("old Voltage :"+ String(voltage_value));

  float new_range_value = 50 + ((voltage_value * 250) / 5.0);

  lcd.setCursor(0, 1);

  lcd.println("New Voltage :"+ String(new_range_value));

  

  if (new_range_value > 4.0 ){
    
    digitalWrite(LED_BUILTIN, 1);
    
  }else{
    digitalWrite(LED_BUILTIN, 0);
  }

  // MEASURE THE TIME TO DO AD CONVERSION USING THE REGISTER CONTROL

  unsigned long Start_time = micros();
  float Register_value = AD_conversion(A2);
  unsigned long Register_time = micros() - Start_time;



  //Serial.println("register :"+ String(Start_time));
  lcd.setCursor(0, 3);

  lcd.println("register :"+ String(Register_time));
  delay(1000);
 
}

int AD_conversion(byte ch){

  DIDR0 = B11111111;// all ad pins to ad mode
  ADMUX = B01000000 | ch; // reference voltage = 5v / input channel = ch
  ADCSRA = B11000111;

  while((ADCSRA & B01000000) !=0);

  return ADC;

}

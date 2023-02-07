
#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ANALOG_PIN = A2;

void setup() {
  lcd.begin(20, 4);
  pinMode(LED_BUILTIN, OUTPUT);

  // for serial monitor begining
  Serial.begin(9600);
  //EX-6
  digitalWrite(6, 1); // HIGH = 1, LOW =0 (EX-7)

}


void loop() {

  // EX_2
  int PIN_state = digitalRead(13);
  lcd.setCursor(0, 1);
  if(analogRead(ANALOG_PIN) > 200){

    lcd.println("Dig.:"+ String(PIN_state) +" Ana.: "+String(analogRead(ANALOG_PIN)));
  }else{
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.println("Waring:");
    lcd.setCursor(0, 2);
    lcd.println("Value too low!");
    //lcd.clear();
  }
  //lcd.clear();
  //lcd.println("Dig.:"+ String(PIN_state) +" Ana.: "+String(analogRead(ANALOG_PIN)));
  

  const float PULSE_DURATION = 100;
  const float PULSE_HIGH = 70;
  const float PULSE_LOW = 30;


  digitalWrite(LED_BUILTIN, 1);  
  delay(PULSE_HIGH);// 70% high 

  digitalWrite(LED_BUILTIN, 0);   
  delay(PULSE_LOW); // 30% low (EX_1) 

  // EX- 3 function call.
  Calculate(PULSE_DURATION, PULSE_HIGH );
  //lcd.println("Ana.: "+String(analogRead(ANALOG_PIN)));
}

// Ex-3

void Calculate (float ENTIRE_PULSE, float HIGH_PULSE){
   
  float dutyCycle = ( HIGH_PULSE / ENTIRE_PULSE ) * 100; // (700/1000) *100;

  // Ex-5

  float Frequency = ( 1 / ENTIRE_PULSE );

  lcd.setCursor(0, 2);
  lcd.print("Dut: "+ String(int(dutyCycle)) +"%");
  lcd.println("Fr: "+ String(Frequency) +"Hz  ");


}

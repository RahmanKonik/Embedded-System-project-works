

#include <LiquidCrystal.h>
#include <TimerOne.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int LED_PIN = 13;
const int button_Pin = 2;
const int time_pin = 7;

volatile unsigned long counter =0;
volatile unsigned long old_time =0;
unsigned long time =0;
float windSpeed = 0.0;
volatile unsigned long windfrequency = 0;
volatile boolean button_pressed = true;



void setup() {
  
  lcd.begin(20, 4);
  //pinMode(time_pin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(button_Pin), pin_ISR, RISING);

  Timer1.initialize(500000);// every 5 second call it
  Timer1.attachInterrupt(Timer_int_routine);
  pinMode(time_pin, INPUT);

  Serial.begin(9600);

}

void loop() {
 
  if(digitalRead(time_pin) == HIGH){

    button_pressed = !button_pressed;

    while(digitalRead(time_pin) == HIGH);
    
    if(button_pressed){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Frequency : ");
      lcd.print(windfrequency);
    }else{
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Speed: ");
     lcd.print(windSpeed);
     lcd.print(" m/s");
    }
}
  

  /* int i = 65;

  for (i = 65 ; i <= 90 ; i++){
    lcd.clear();
    lcd.setCursor(i-65, 0);
    lcd.print(String(char(i)));
    delay(500);
  } */
/* 
  int READING_PRESS_PIN = digitalRead(button_Pin);
  int BUTTON_STATE = 0;
  if (READING_PRESS_PIN == 1){
    lcd.clear();
    lcd.print("Pressed");
  }
  else{
    lcd.clear();
    lcd.print("Not Pressed");
  } */
/*   lcd.setCursor(0,0);
  lcd.print("Frequency : ");
  lcd.print(frequency);
  lcd.setCursor(0,1);
  lcd.print("Speed: ");
  float wind = -0.24 + (frequency * 0.699);
  lcd.print(wind);
  lcd.print("m/s"); */

/*   lcd.print("Count : ");
  lcd.print(counter); */

  /* delay(500);
  lcd.clear(); */

}
void pin_ISR(){

 /*  unsigned long present_time = millis();

  if( digitalRead(button_Pin) == LOW){

    // time difference between two falling edges
    frequency = 1000.0 / (present_time - old_time);
    
    // replace the old_time by new present_time.
    old_time = present_time;

  } */

	counter++;    // increment variable number_of_pressing
}

void Timer_int_routine() {

  time++;

  if ( time > 9){
    windfrequency = counter / 5;
    counter = 0;
    windSpeed = - 0.24 + windfrequency * 0.699;
    time = 0;
  }
  
}


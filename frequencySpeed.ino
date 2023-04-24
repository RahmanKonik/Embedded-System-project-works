

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
  pinMode(button_Pin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(button_Pin), pin_ISR, RISING);

  Timer1.initialize(100000);
  Timer1.attachInterrupt(Timer_int_routine);

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
      lcd.print(" Hz");
    }else{
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Speed: ");
     lcd.print(windSpeed);
     lcd.print(" m/s");
    }
  }
  
  lcd.setCursor(20,1);
  lcd.print("No-idea group!");
  delay(1000);

}

void pin_ISR(){

	counter++; 
}

void Timer_int_routine() {

  time++;

  if ( time > 9){
    windfrequency = counter / 1;
    counter = 0;
    windSpeed = - 0.24 + windfrequency * 0.699;
    time = 0;
  }
  
}


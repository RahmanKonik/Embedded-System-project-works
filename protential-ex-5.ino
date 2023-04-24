#include <LiquidCrystal.h>
#include <Wire.h>

#define CMPS14_address 0x60

#define Motor_forward         1
#define Motor_return          0
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10


const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


volatile int Lcounter = 0;
volatile int Count = 0;


void setup() {

  DDRB= B11111111;
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(23,INPUT);
  pinMode(24,INPUT);
  
  attachInterrupt(digitalPinToInterrupt(2), pin_ISR, RISING);
  Serial.begin(9600);
  Serial.println("Write a command.");  
  
  lcd.begin(16, 2);
  //lcd.print("left encB: ");
  
}
void pin_ISR() {
    Lcounter++; 
    Count++;
  }
int conversionVoltage(int joyStickValue){
  int voltage = joyStickValue * 1.9;
  return voltage;
 }
 
void loop() {
  double degree;
  int xJoystick;
  int pwm_R=0;
  int pwm_L=0;

  Wire.begin();

    Wire.beginTransmission(CMPS14_address);
    Wire.write(1);
    Wire.endTransmission(false);
    Wire.requestFrom(CMPS14_address, 1, true);

    if (Wire.available() >= 1 ){
      byte raw = Wire.read();
     degree = raw*1.4;
    }
  int destination = analogRead(A0)*0.35156;
  
  if (destination > degree+20){
    digitalWrite( Motor_L_dir_pin, Motor_forward);
    digitalWrite( Motor_R_dir_pin, Motor_return);
    while(destination > degree+10){
      analogWrite(Motor_L_pwm_pin,200);
          analogWrite(Motor_R_pwm_pin,200);
         //update the degree inside the while loop to avoid infinite loop 
          Wire.begin();

    Wire.beginTransmission(CMPS14_address);
    Wire.write(1);
    Wire.endTransmission(false);
    Wire.requestFrom(CMPS14_address, 1, true);

    if (Wire.available() >= 1 ){
      byte raw = Wire.read();
     degree = raw*1.4;
    }
    if (degree >= destination+5) break;
    
        }
        
        analogWrite(Motor_L_pwm_pin,0);
        analogWrite(Motor_R_pwm_pin,0);
    }
  
  else if(destination < degree-20){
    digitalWrite( Motor_L_dir_pin, Motor_return);
    digitalWrite( Motor_R_dir_pin, Motor_forward);
  
  while(destination < degree-10){
      analogWrite(Motor_L_pwm_pin,200);
      analogWrite(Motor_R_pwm_pin,200);
         //update the degree inside the while loop to avoid infinite loop 
          Wire.begin();

    Wire.beginTransmission(CMPS14_address);
    Wire.write(1);
    Wire.endTransmission(false);
    Wire.requestFrom(CMPS14_address, 1, true);

    if (Wire.available() >= 1 ){
      byte raw = Wire.read();
     degree = raw*1.4;
    }
    if (degree <= destination-5) break;
    
        }
      analogWrite(Motor_L_pwm_pin,0);
      analogWrite(Motor_R_pwm_pin,0);
  }
  else {
    analogWrite(Motor_L_pwm_pin,0);
    analogWrite(Motor_R_pwm_pin,0);
   
    
  }
}

//    Alyk  jatko course 2021       30.3.2020 KN
//                                  2021 SV, EK
//    

#include <Ethernet.h>                                 // incluide Ethernet library W5100
#include <PubSubClient.h>                             // include MQTT library      
#include <TimerOne.h> 

//libraries from week10
#include <Keypad.h>
#include <LiquidCrystal.h>
//#include <ArduinoJson.h>

///////////////////////////////////////////////////////

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define button pin
volatile int pulseCount = 8;
unsigned long time = 0;
volatile int buttonPin = 7;

volatile boolean buttonPressedA = false;
volatile boolean buttonPressedB = false;
//volatile boolean buttonPressedC = false;


// Define global variables
volatile float frequency = 0;  // calculated frequency
//bool showFrequency = true;  // flag to indicate whether to display frequency or wind s

int ANALOG_PIN = A2;
//const int buttonPin = 2;

volatile boolean ButtonPressed;
int ButtonState = 0;
int previousButtonState = 0;

int degreeValue;
String windDirections;

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


////////////////////////////////////////////////////////////////////////////

                                // include timer library

EthernetClient ethClient;                               // Ethernet object var  

static uint8_t mymac[6] = { 0x44,0x76,0x58,0x10,0x00,0x62 };

//  MQTT settings
 
unsigned int Port = 1883;                          //  MQTT port number
byte server[] = { 10,6,0,21 };                    // TAMK IP

char* deviceId     = "ekrkjhgfejh";                   // * set your device id (will be the MQTT client username) *yksilöllinen*
char* clientId     = "a731fsd4";                       // * set a random string (max 23 chars, will be the MQTT client id) *yksilöllinen*
char* deviceSecret = "tamk";                        // * set your device secret (will be the MQTT client password) *kaikille yhteinen*

//  MQTT Server settings  

void callback(char* topic, byte* payload, unsigned int length); // subscription callback for received MQTTT messages   

PubSubClient client(server, Port, callback, ethClient);   // mqtt client 

//  MQTT topic names 

 #define inTopic    "ICT1B_in_2020"                    // * MQTT channel where data are received 
 #define outTopic   "ICT1B_out_2020"                   // * MQTT channel where data is send 
                          
//  SETUP section

void setup() {
    Serial.begin(9600);                                 // Serial monitor baudrate  = 9600
    Serial.println("Start 20.3.2021");                   // print to serial monitor
    delay(500);
    fetch_IP();                                         // initialize Ethernet connection
    Connect_MQTT_server(); 
    
                                 // connect to MQTT server    

////////////////////////////////////////////////////////
   pinMode(pulseCount, INPUT);  // set pin 2 as input
   attachInterrupt(digitalPinToInterrupt(pulseCount), pulse_isr, RISING);  // attach ISR to rising edge of pin 2
 
   Timer1.initialize(100000);//set timer interrupt in microsecond
   Timer1.attachInterrupt(timerInter);
 
   lcd.begin(16, 4);  // initialize LCD


////////////////////////////////////////////////////////  

}

void pulse_isr() {
  pulseCount++;  // increment pulse counter
}

void timerInter() {
  time++;
}

void loop(){

//////////////////////////////////////////////

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
    //buttonPressedC = false;

  } else if(key == 'B'){
    buttonPressedA = false;
    buttonPressedB = true;
    //buttonPressedC = false;

  } 


  float windSpeed = 0.0;
  if (buttonPressedA == true) {
    
    lcd.clear();
    frequency = pulseCount / (time * 0.1);
    float windSpeed = -0.24 + (frequency * 0.699);
    lcd.setCursor(0,0);
    lcd.print("Wind speed: ");
    lcd.print(windSpeed);
    
    /*
    float wind_speed_sum = 0.0;

    for (int i = 0; i < wind; i++) {
      wind_speed_sum += analogRead(ANALOG_PIN);
    }
    wind_speed_avg = (wind_speed_sum / wind) * (5.0 / 1024.0);

    lcd.print(wind_speed_avg);
    */
    lcd.print("m/s");
  } else if(buttonPressedB == true){
    lcd.clear();
    if (voltageValue < 1.2) {
      lcd.print("Passing");
    } else if (degreeValue >= 337.5 || (degreeValue <= 22.5 && degreeValue > 0)) {
      windDirections = "N";
    } else if (degreeValue > 22.5 && degreeValue <= 67.5) {
      windDirections = "NE";
    } else if (degreeValue > 67.5 && degreeValue <= 112.5) {
      windDirections = "E";
    } else if (degreeValue > 112.5 && degreeValue <= 157.5) {
      windDirections = "SE";
    } else if (degreeValue > 157.5 && degreeValue <= 202.5) {
      windDirections = "S";
    } else if (degreeValue > 202.5 && degreeValue <= 247.5) {
      windDirections = "S";
    } else if (degreeValue > 247.5 && degreeValue <= 292.5) {
      windDirections = "W";
    } else {
      windDirections = "NW";
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Direction: ");
    lcd.print(windDirections);
    lcd.setCursor(0, 1);
    lcd.print("Deg.: ");
    lcd.print(degreeValue);
  } 

  // Send MQTT message to broker
  if (client.connected()){ 
    const char* windDirections = windDirections.c_str();
    send_MQTT_message(windDirections,windSpeed);
  } else {                                                           // Reconnect if connection is lost
    delay(500);
    Serial.println("No, re-connecting" );
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);                                            // Wait for reconnecting
  }

  // Delay before checking again
  delay(1000);
}

//  GET IP number from DHCP server

void fetch_IP(void){
  byte rev=1;
  rev=Ethernet.begin(mymac);                  // get IP number
  Serial.print( F("\nW5100 Revision ") );
  
  if (rev == 0){
    Serial.println( F( "Failed to access Ethernet controller" ) );
  }    
                 
  Serial.println( F( "Setting up DHCP" ));
  Serial.print("Connected with IP: "); 
  Serial.println(Ethernet.localIP()); 
  delay(1500);
}

//  MQTT Routines
                                                         
void send_MQTT_message(const char* dir, int spd){                     // Send MQTT message
  char bufa[50];
  char fufa[50];                             //  Print message to serial monitor
  if (client.connected()){ 
    sprintf(bufa,"Team anonymous Wind Direction: %s", dir);              // create message with header and data
    Serial.println( bufa ); 
    client.publish(outTopic,bufa);  
    sprintf(fufa,"Team anonymousWind Speed (m/s): %d", spd);              // create message with header and data
    Serial.println( fufa ); 
    client.publish(outTopic,fufa); 
    delay(10000);    
                      // send message to MQTT server        
  }
  else{                                                           //   Re connect if connection is lost
    delay(500);
    Serial.println("No, re-connecting" );
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);                                            // wait for reconnecting
  }
}


//  MQTT server connection  
                                           
void Connect_MQTT_server(){ 
  Serial.println(" Connecting to MQTT" );
  Serial.print(server[0]); Serial.print(".");     // Print MQTT server IP number to Serial monitor
  Serial.print(server[1]); Serial.print(".");
  Serial.print(server[2]); Serial.print(".");
  Serial.println(server[3]); 
  delay(500);
  
  if (!client.connected()){                                   // check if allready connected  
    if (client.connect(clientId, deviceId, deviceSecret)){ // connection to MQTT server 
      Serial.println(" Connected OK " );
      client.subscribe(inTopic);                        // subscript to in topic        
    } 
    else{
       Serial.println(client.state());
    }    
  } 
}

//  Receive incoming MQTT message   
 
void callback(char* topic, byte* payload, unsigned int length){ 
  char* receiv_string;                               // copy the payload content into a char* 
  receiv_string = (char*) malloc(length + 1); 
  memcpy(receiv_string, payload, length);           // copy received message to receiv_string 
  receiv_string[length] = '\0';           
  Serial.println( receiv_string );
  free(receiv_string); 
} 



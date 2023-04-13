//    Alyk  jatko course 2021       30.3.2020 KN
//                                  2021 SV, EK
//    

#include <Ethernet.h>                                 // incluide Ethernet library W5100
#include <PubSubClient.h>                             // include MQTT library      
#include <TimerOne.h>                                 // include timer library

/////////////////////////////

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int pulseCountPin = 8;// signal pin
const int buttonPin = 2;
const int ANALOG_PIN = A2;

volatile boolean buttonPressedA = false;
volatile boolean buttonPressedB = false;
volatile boolean buttonPressedC = false;

// Define global variables

volatile unsigned long frequency = 0; 
volatile unsigned long pulseCount = 0;
volatile boolean ButtonPressed;
int ButtonState = 0;
int previousButtonState = 0;
unsigned long time = 0;
int degreeValue;
String windDirection;
float windSpeed = 0.0;

bool displayDirection;
bool displaySpeed;
bool displayBoth;


//////keypad

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



//////////////////////////

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
 #define outTopic   "ICT4_out_2020"                   // * MQTT channel where data is send 
                          
//  SETUP section


void setup() {
    Serial.begin(9600);                                 // Serial monitor baudrate  = 9600
    Serial.println("Start 19.3.2021");                   // print to serial monitor
    delay(500);
    fetch_IP();                                         // initialize Ethernet connection
    Connect_MQTT_server();                             // connect to MQTT server 

    /////////////////////

    lcd.begin(16, 2);  // initialize LCD
    
    // speed pin = 2

    pinMode(ANALOG_PIN, INPUT);
    pinMode(buttonPin, INPUT);
    
    // singal pin = 8
    
    attachInterrupt(digitalPinToInterrupt(pulseCountPin), pulse_isr, RISING);  
 
    Timer1.initialize(500000);//set timer interrupt in microsecond
    Timer1.attachInterrupt(timerInter);



    ////////////////////   
}


void loop(){

  char key = keypad.getKey();

  if (key == '1'){

    buttonPressedA = true;
    buttonPressedB = false;
    buttonPressedC = false;

  } else if(key == '2'){

    buttonPressedA = false;
    buttonPressedB = true;
    buttonPressedC = false;

  } else if(key == '3'){

    buttonPressedA = false;
    buttonPressedB = false;
    buttonPressedC = true;

  } else {

    buttonPressedA = false;
    buttonPressedB = false;
    buttonPressedC = false;

  }

  if(buttonPressedA == true){

    displayDirection = true;
    displaySpeed = false;
    displayBoth = false;

  } else if(buttonPressedB == true){

    displayDirection = false;
    displaySpeed = true;
    displayBoth = false;

  } else if(buttonPressedC == true){

    displayDirection = false;
    displaySpeed = false;
    displayBoth = true;
    
  }


  lcd.setCursor(20, 1);
  lcd.print("1-Dir  2-Spd 3-Both");

  float voltageValue = (analogRead(ANALOG_PIN) *  5.0) / 1023;

  if (voltageValue < 1.2) {

    degreeValue = 0;
  } else{
    degreeValue = ((voltageValue - 1.2) / 3.8) * 360;
  }



  if (displayDirection == true) {

    lcd.setCursor(0, 0);
    if (voltageValue < 1.2) {
      lcd.print("Passing...");
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
    lcd.setCursor(20, 1);
    lcd.print("1-Dir  2-Spd 3-Both");


    ////

  } else if (displaySpeed == true){

    ////
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wind speed: ");
    lcd.print(windSpeed);
    lcd.print("m/s");
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    lcd.print(frequency);
    lcd.setCursor(20, 1);
    lcd.print("1-Dir  2-Spd 3-Both");

  } else if (displayBoth == true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wind speed: ");
    lcd.print(windSpeed);
    lcd.setCursor(0, 1);
    lcd.print("Direction: ");
    lcd.print(degreeValue);
    lcd.setCursor(20, 1);
    lcd.print("1-Dir  2-Spd 3-Both");
  }



// Send MQTT message to broker

if (client.connected()){ 
  const char* windDirections = windDirection.c_str();
  send_MQTT_message(degreeValue,windSpeed);
} else {                                                           // Reconnect if connection is lost
    
  Serial.println("No, re-connecting" );
  client.connect(clientId, deviceId, deviceSecret);
    
} 

delay(1000);

  ////////////////////////////////
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
  //delay(1500);
}



//  MQTT Routines
                                                         
void send_MQTT_message(int dir, float spd){              // Send MQTT message
  char bufa[50];
  char fufa[50];
  char sufa[50];                                        //  Print message to serial monitor
  if (client.connected()){ 
   
        int timeDelay = 1000;                           // This is for adjust the time delay of MQTT + JSON transferring
        if (displayDirection == true){

          const size_t bufferSize = JSON_OBJECT_SIZE(2) + 30;
          StaticJsonDocument<bufferSize> jsonDoc;

          jsonDoc["S_name1"] = "KR Wind Direction";
          jsonDoc["S_value1"] = dir;

          String jsonStr;
          serializeJson(jsonDoc, jsonStr);

          char message[jsonStr.length() + 10];
          sprintf(message, "IOTJS=%s", jsonStr.c_str());

          // create message with header and data
          Serial.println( message ); 
          client.publish(outTopic,message); 
          delay(timeDelay);


        } else if (displaySpeed == true){

          const size_t bufferSize = JSON_OBJECT_SIZE(2) + 30;
          StaticJsonDocument<bufferSize> jsonDoc;

          jsonDoc["S_name1"] = "KR Wind Speed";
          jsonDoc["S_value1"] = spd;

          String jsonStr;
          serializeJson(jsonDoc, jsonStr);

          char message[jsonStr.length() + 10];
          sprintf(message, "IOTJS=%s", jsonStr.c_str());

          // create message with header and data
          Serial.println( message ); 
          client.publish(outTopic,message);
          delay(timeDelay);



        } else if (displayBoth == true) {

          /////////////////////////////////////////////////////


          //wind direction here
          const size_t bufferSize = JSON_OBJECT_SIZE(2) + 30;
          StaticJsonDocument<bufferSize> jsonDoc;

          jsonDoc["S_name1"] = "KR Wind Direction";
          jsonDoc["S_value1"] = dir;

          String jsonStr;
          serializeJson(jsonDoc, jsonStr);

          char message[jsonStr.length() + 10];
          sprintf(message, "IOTJS=%s", jsonStr.c_str());

          // create message with header and data
          Serial.println(message);
          client.publish(outTopic, message);

          /////////////////////////////////////////////////////


          //wind speed here

          jsonDoc.clear(); // Clear the previous data from the JSON document
          delay(500);

          StaticJsonDocument<bufferSize> jsonDoc2;

          jsonDoc2["S_name1"] = "KR Wind Speed";
          jsonDoc2["S_value1"] = spd;

          String jsonStr2;
          serializeJson(jsonDoc2, jsonStr2);

          char message2[jsonStr2.length() + 10];
          sprintf(message2, "IOTJS=%s", jsonStr2.c_str());
          // create message with header and data
          Serial.println( message2 ); 
          client.publish(outTopic,message2);
          delay(timeDelay);
    }

  } else {
                                                               //   Re connect if connection is lost
    //delay(500);
    Serial.println("No, re-connecting" );
    client.connect(clientId, deviceId, deviceSecret);
    delay(5000);                                            // wait for reconnecting
  }
}


//  MQTT server connection  
                                           
void Connect_MQTT_server(){ 
  Serial.println(" Connecting to MQTT" );
  Serial.print(server[0]); Serial.print(".");     // Print MQTT server IP number to Serial monitor
  Serial.print(server[1]); Serial.print(".");
  Serial.print(server[2]); Serial.print(".");
  Serial.println(server[3]); 
  //delay(500);
  
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

//////////////////

void pulse_isr() {
  pulseCount++;  // increment pulse counter
}

void timerInter() {

  time++;
  if (time > 19) { // Perform wind speed calculation every 5 seconds
    frequency = pulseCount / 10; // Multiply pulse count by 2 to get frequency in Hz
    pulseCount = 0; // Reset pulse count
    windSpeed = - 0.24 + frequency * 0.699; 
    time = 0; // counter reset time
  }
}


/////////////////

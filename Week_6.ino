
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  const float PELUSE_DURATION = 1000;
  const float LED_HIGH_DURATION = 300;
  const float LED_LOW_DURATION = 700;

  // PASSING THE VALUE INSIDE THE FUNCTION
  calculate(PELUSE_DURATION, LED_HIGH_DURATION );

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(300);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(700);                      // wait for a second
}

// calcluate the passing value in led_ high

void calculate(float DURATION_PULSE, float LED_HIGH){

  // finding the passing value in led high

  float dutyCycle = ( LED_HIGH / DURATION_PULSE) * 100;

  // frequency value

  float frequency = 1 / DURATION_PULSE;

  Serial.println(dutyCycle);
  Serial.println(frequency);

}

/*  LASER TRIPWIRE MODULE

    The Laser Tripwire Module is a versatile tripwire system which is working with Arduino UNO or upper versions.
    For driving this code as stable, You need one laser circuit for transmitter and one sensor circuit for receiver.
    For a better stability, do not change anything in this library and edit constant value that given at the top.

    This Library was written by Berk Altun - www.vberkaltun.com
    All right reserved - 2017
*/

// Constant values for tripwire
#define pin_sensor A0
#define pin_laser 02
#define pin_buzzer 03

// Constant values for button
#define pin_manuel 04
#define pin_automatic 04

// Related with your environment and ambient light
#define cut_off 650

// Working and displaying duration variant
#define delay_buzzer 100
#define delay_text 1000

// Store manuel status for auto processing
bool flag_manuel = false;

void setup() {

  // Begin serial communcation
  Serial.begin(9600);

  // Initialize digital pin as an output
  pinMode(pin_laser, OUTPUT);

  // Initialize digital pin as an output
  pinMode(pin_buzzer, OUTPUT);

  // Initialize laser module
  initialize_laser();
}

void loop() {

  buttonStatus = digitalRead(button);
  
  if (buttonStatus == HIGH) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void initialize_laser() {

  // Read the input pin and - or debug value
  Serial.println("LASER INITIALIZING ...");

  // Wait for specified time
  delay(delay_text);

  // -----

  // Turn the pin on (HIGH is the voltage level)
  digitalWrite(pin_laser, HIGH);

  // Depending on the value that returned from the sensor, run recursively. Otherwise, break process tree
  if (initialize_sensor() == true) {

    // Depending on the value that returned from the sensor, run recursively. Otherwise, break process tree
    while (flag_manuel == false && initialize_sensor() == true) stepper_downside();
  }
  else {

    // Depending on the value that returned from the sensor, run recursively. Otherwise, break process tree
    while (flag_manuel == false && initialize_sensor() == false) stepper_upside();
  }

  // -----

  // Turn the pin off (LOW is the voltage level)
  digitalWrite(pin_laser, LOW);

  // Initialize buzzer module
  if (flag_manuel == false) initialize_buzzer();
}

bool initialize_sensor() {

  // Store received value in variable
  int sensor_value = analogRead(pin_sensor);

  // Read the input pin and - or debug value
  Serial.println("SENSOR VALUE = " + String(sensor_value));

  // If laser tripwire interrupted by an object, break tripwire process
  return (sensor_value <= cut_off ? false : true);
}

void initialize_buzzer() {

  // Read the input pin and - or debug value
  Serial.println("BUZZER > TRUE");

  // Turn the pin on (HIGH is the voltage level)
  digitalWrite(pin_buzzer, HIGH);

  // Wait for specified time
  delay(delay_buzzer);

  // Turn the pin off (LOW is the voltage level)
  digitalWrite(pin_buzzer, LOW);
}

void stepper_upside() {

  // Read the input pin and - or debug value
  Serial.println("STEPPER > UPSIDE");
}

void stepper_downside() {

  // Read the input pin and - or debug value
  Serial.println("STEPPER > DOWNSIDE");
}


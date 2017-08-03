/*  LASER TRIPWIRE MODULE

    The Laser Tripwire Module is a versatile tripwire system which is working with Arduino UNO or upper versions.
    For driving this code as stable, You need one laser circuit for transmitter and one sensor circuit for receiver.
    For a better stability, do not change anything in this library and edit constant value that given at the top.

    This Library was written by Berk Altun - www.vberkaltun.com
    All right reserved - 2017
*/

// Constant values for Laser, Sensor and Buzzer
#define pin_sensor A0
#define pin_laser 02
#define pin_buzzer 03

// Related with your environment and ambient light
#define cut_off 650

// Working duration of buzzer
#define delay_buzzer 100

// Displaying duration of text
#define delay_text 1000

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

    // Read the input pin and - or debug value
    Serial.println("STEPPER > DOWNSIDE");

    while (true) {
      if (stepper_downside() == true) break;
    }
  }
  else {

    // Read the input pin and - or debug value
    Serial.println("STEPPER > UPSIDE");

    while (true) {
      if (stepper_downside() == true) break;
    }
  }

  // -----

  // Turn the pin off (LOW is the voltage level)
  digitalWrite(pin_laser, LOW);

  // Initialize buzzer module
  initialize_buzzer();
}

bool initialize_sensor() {

  // Read the input pin and - or debug value
  Serial.println("SENSOR VALUE = " + String(analogRead(pin_sensor)));

  // If laser tripwire interrupted by an object, break tripwire process
  return (analogRead(pin_sensor) <= cut_off ? false : true);
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

bool stepper_upside() {



  return (initialize_sensor() == false ? false : true);
}

bool stepper_downside() {

  return (initialize_sensor() == true ? false : true);
}

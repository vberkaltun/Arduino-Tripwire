/*  LASER TRIPWIRE MODULE

    The Laser Tripwire Module is a versatile tripwire system which is working with Arduino UNO or upper versions.
    For driving this code as stable, You need one laser circuit for transmitter and one sensor circuit for receiver.
    For a better stability, do not change anything in this library and edit constant value that given at the top.

    This Library was written by Berk Altun. - www.vberkaltun.com
    All right reserved. - 2017

    Version 1.0 - Designed with Keyes Buzzer, Keyes Lazer, Keyes LDR and L298N Dual H-Bridge Motor Controller.
*/

// Constant values for tripwire
#define pin_sensor A0
#define pin_laser 02
#define pin_buzzer 03

// Constant values for button
#define pin_upside 04
#define pin_automatic 05
#define pin_downside 06

// Related with your environment and ambient light
#define cut_off 650

// Running duration of variant
#define delay_buzzer 100
#define delay_sensor 250

void setup() {

  // Begin serial communcation
  Serial.begin(9600);

  // Initialize digital pin as an output
  pinMode(pin_laser, OUTPUT);
  pinMode(pin_buzzer, OUTPUT);

  // Initialize digital pin as an input
  pinMode(pin_upside, INPUT);
  pinMode(pin_automatic, INPUT);
  pinMode(pin_downside, INPUT);
}

void loop() {

  if (digitalRead(pin_automatic) == HIGH) {

    // Move stepper motor as automatic
    stepper_automatic();
  }
  else if (digitalRead(pin_upside) == HIGH) {

    // Move stepper motor to upside
    stepper_upside();
  }
  else if  (digitalRead(pin_downside) == HIGH) {

    // Move stepper motor to downside
    stepper_downside();
  }
}

bool initialize_laser() {

  // Detect manuel inputs. if pressed, store it as true
  bool initialize_value = (digitalRead(pin_upside) == HIGH || digitalRead(pin_downside) == HIGH) ? false : true;

  // Detect automatic inputs. If pressed, override it to manuel inputs
  return digitalRead(pin_automatic) == HIGH ? true : initialize_value;
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

void stepper_automatic() {

  // Read the input pin and - or debug value
  Serial.println("LASER INITIALIZING ...");

  // Turn the pin on (HIGH is the voltage level)
  digitalWrite(pin_laser, HIGH);

  // -----

  // Wait for specified time
  delay(delay_sensor);

  // Depending on the value that returned from the sensor, run recursively. Otherwise, break process tree
  if (initialize_sensor() == true) {

    // Depending on the value that returned from the sensor, run recursively. Otherwise, break process tree
    while (initialize_laser() == true && initialize_sensor() == true) stepper_downside();
  }
  else {

    // Depending on the value that returned from the sensor, run recursively. Otherwise, break process tree
    while (initialize_laser() == true && initialize_sensor() == false) stepper_upside();
  }

  // -----

  // Turn the pin off (LOW is the voltage level)
  digitalWrite(pin_laser, LOW);

  // Initialize buzzer module
  if (initialize_laser() == true) initialize_buzzer();
}

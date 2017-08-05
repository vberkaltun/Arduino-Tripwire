/*  LASER TRIPWIRE MODULE

    The Laser Tripwire Module is a versatile tripwire system which is working with Arduino UNO or upper versions.
    For driving this code as stable, You need one laser circuit for transmitter and one sensor circuit for receiver.
    For a better stability, do not change anything in this library and edit constant value that given at the top.

    This Library was written by Berk Altun. - www.vberkaltun.com
    All right reserved. - 2017

    Version 1.0 - Designed with Keyes Buzzer, Keyes Lazer, Keyes LDR and L298N Dual H-Bridge Motor Controller.
*/

#include <Stepper.h>

// Constant values of tripwire IO
#define pin_sensor A0
#define pin_laser 2
#define pin_buzzer 3

// Constant values of button IO
#define pin_upside 4
#define pin_automatic 5
#define pin_downside 6

// Cut off value of LDR sensor module, related with your ambient light
#define cut_off 650

// Running duration of buzzer and sensor
#define delay_buzzer 100
#define delay_sensor 250

// Constant values of stepper IO
#define steppper_pin_AA 8
#define steppper_pin_AB 9
#define steppper_pin_BA 10
#define steppper_pin_BB 11

// How many steps per revolution and speed the motor has
#define steppper_step 200
#define steppper_degree 1.8
#define steppper_speed 120

// Declaring of stepper object
Stepper stepper(steppper_step, steppper_pin_AA, steppper_pin_AB, steppper_pin_BA, steppper_pin_BB);

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

  // Initialize stepper speed
  stepper.setSpeed(steppper_speed);
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

  // Move a step further
  stepper.step(- steppper_degree);
}

void stepper_downside() {

  // Read the input pin and - or debug value
  Serial.println("STEPPER > DOWNSIDE");

  // Move a step further
  stepper.step(+ steppper_degree);
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

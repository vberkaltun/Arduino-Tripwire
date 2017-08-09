/*  LASER TRIPWIRE MODULE

    The Laser Tripwire Module is a versatile tripwire system which is working with Arduino UNO or upper versions.
    For driving this code as stable, You need one laser circuit for transmitter and one sensor circuit for receiver.
    For a better stability, do not change anything in this library and edit constant value that given at the top.

    This Library was written by Berk Altun - www.vberkaltun.com
    GNU General Public License v3.0 - 2017

    Version 3.0 - Designed with Keyes Buzzer, Keyes Lazer, Keyes LDR and Digital Stepper Drive CWD556.
*/

// Constant values of tripwire IO
#define pin_sensor A0
#define pin_laser 2
#define pin_buzzer 3

// Constant values of button IO
#define pin_upside 4
#define pin_automatic 6
#define pin_downside 5

// Cut off value of LDR sensor module, related with your ambient light
#define cut_off 650

// Running duration of buzzer and sensor
#define delay_buzzer 100
#define delay_sensor 250

void setup() {

  // Begin serial communcation
  Serial.begin(9600);

  // Initialize tripwire module pins
  pinMode(pin_sensor, INPUT);
  pinMode(pin_laser, OUTPUT);
  pinMode(pin_buzzer, OUTPUT);

  // Initialize button module pins
  pinMode(pin_upside, INPUT);
  pinMode(pin_automatic, INPUT);
  pinMode(pin_downside, INPUT);
}

void loop() {

  // Read digital input and choose related process
  if (digitalRead(pin_upside) == LOW && digitalRead(pin_downside) == LOW & digitalRead(pin_automatic) == HIGH) {

    // Initialize laser module
    initialize_laser(true);

    // Wait for specified time
    delay(delay_sensor);

    // Detect the position of sensor at console
    if (initialize_sensor() == true) {

      // Depending on the value that returned from the sensor, run recursively to downside
      while (digitalRead(pin_upside) == LOW && digitalRead(pin_downside) == LOW && initialize_sensor() == true)
        stepper_downside();
    }
    else {

      // Depending on the value that returned from the sensor, run recursively to upside
      while (digitalRead(pin_upside) == LOW && digitalRead(pin_downside) == LOW  && initialize_sensor() == false)
        stepper_upside();
    }

    // Initialize laser module
    initialize_laser(false);

    // Initialize buzzer module if automatic process is not interrupted by manuel process
    if (digitalRead(pin_upside) == LOW && digitalRead(pin_downside) == LOW) initialize_buzzer();
  }
  else if (digitalRead(pin_upside) == HIGH && digitalRead(pin_downside) == LOW & digitalRead(pin_automatic) == LOW) {

    // Initialize laser module
    initialize_laser(false);

    // Move stepper motor to upside
    stepper_upside();
  }
  else if  (digitalRead(pin_upside) == LOW && digitalRead(pin_downside) == HIGH & digitalRead(pin_automatic) == LOW) {

    // Initialize laser module
    initialize_laser(false);

    // Move stepper motor to downside
    stepper_downside();
  }
}

int check_button() {

  // Store calculated value in variable
  int button_value = 0;

  // Make a bitwise. Each bit store a value associated with same button
  bitWrite(button_value, 0 , digitalRead(pin_upside));
  bitWrite(button_value, 1 , digitalRead(pin_automatic));
  bitWrite(button_value, 2 , digitalRead(pin_downside));

  // Return calculated value to caller method
  return button_value;
}

void initialize_laser(bool value) {

  // Check flag and laser value. Depending on the status, do not do same process again
  if (value == true && digitalRead(pin_laser) == LOW)
  {
    // Read the input pin and - or debug value
    Serial.println("LASER INITIALIZING > HIGH");

    // Activate laser module
    digitalWrite(pin_laser, HIGH);
  }
  else if (value == false && digitalRead(pin_laser) == HIGH)
  {
    // Read the input pin and - or debug value
    Serial.println("LASER INITIALIZING > LOW");

    // Deactivate laser module
    digitalWrite(pin_laser, LOW);
  }
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
  Serial.println("BUZZER > ACTIVE");

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

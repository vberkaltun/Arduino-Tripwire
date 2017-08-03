/*  LASER TRIPWIRE MODULE

    The Laser Tripwire Module is a versatile tripwire system which is working Arduino UNO.
    For driving this code as stable, You need one laser circuit for transmitter and one sensor circuit for receiver.
    In receiver side, Buzzer code is embedded to inside. You can not split or deactivating it.

    This Library was written by Berk Altun - www.vberkaltun.com
    All right reserved - 2017
*/

// Constant values for Laser, Sensor and Buzzer
#define pin_sensor A0
#define pin_laser 02
#define pin_buzzer 03

// This is related with your environment and ambient light
#define cut_off 650

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

  // Turn the pin on (HIGH is the voltage level)
  digitalWrite(pin_laser, HIGH);

  // While sensor return true, run recursively. Otherwise, break process tree
  while (true) if (initialize_sensor() == false) break;

  // Turn the pin off (LOW is the voltage level)
  digitalWrite(pin_laser, LOW);

  // Initialize buzzer module
  initialize_buzzer();
}

bool initialize_sensor() {

  // Read the input pin and debug value
  Serial.println(analogRead(pin_sensor));

  // If laser tripwire interrupted by an object, break tripwire process
  return (analogRead(pin_sensor) <= cut_off ? false : true);
}

bool initialize_buzzer() {

  // Turn the pin on (HIGH is the voltage level)
  digitalWrite(pin_buzzer, HIGH);

  // Wait for 10 miliseconds
  delay(100);

  // Turn the pin off (LOW is the voltage level)
  digitalWrite(pin_buzzer, LOW);
}

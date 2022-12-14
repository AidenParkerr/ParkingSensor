
/**
  This program calculates the distance to an object using the HC-SR04 ultrasonic module and displays this value onto an LCD display.
  Calculates the distance in both feet or centimetres. Formula used to calculate the distance to an object is D = T * S.

  Author - Aiden Parker
  Date - 21/11/22
*/

#include <LiquidCrystal.h>

#define LED 3               // define LED pin to 3.
#define TRIG 9              // define trigger pin of sensor module to 6.
#define ECHO 8              // define echo pin of sensor module to 7.
#define SPEED_OF_SOUND 343  //The climate of the enviroment is assumed to be dry air @ 20°C, \
                            //resulting in the speed of sound = 343 m/s.

const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;  // Setting up LCD pins.
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                   // Pass LCD pins into LiquidCrystal constructor.
int limit = 50;

void setup() {
  /*
    // *Set up method.
    Set LCD grid layout to 16x1 // ColxRow
    Set trigger pin to OUTPUT.
    Set echo pin to INPUT.
    Establish connection with Serial.Monitor at 9,600 bits / s
  */
  lcd.begin(16, 1); // Set the grid layout of the display to 16x1; Columns x Rows
  pinMode(TRIG, OUTPUT);  // Set trigger pin to OUTPUT.
  pinMode(ECHO, INPUT);   // Set echo pin to INPUT.
  Serial.begin(9600);     // Set data rate to Serial.Monitor @ 9,600 bits / s.
}

float calculateDistanceFoot(double timeElapsed) {
  /*
    Computes the distance to an object, in feet, using the formula D = T * S / F.
    The climate of the enviroment is assumed to be dry air @ 20°C,
    resulting in the speed of sound = 343 m/s.

    Distance = (timeElapsed * (speedOfSound in cm) / convert to foot) / 2
  */

  return (timeElapsed * (SPEED_OF_SOUND * 0.0001) / 30.48) / 2;
}

float calculateDistanceCM(double timeElapsed) {
  /*
    Computes the distance to an object, in centimetres, using the formula D = T * S.
    The climate of the enviroment is assumed to be dry air @ 20°C,
    resulting in the speed of sound = 343 m/s.

    Distance = (timeElapsed * (speedOfSound in cm)) / 2
  */
  return (timeElapsed * (SPEED_OF_SOUND * 0.0001)) / 2;  // 343 * 0.0001 = 0.0343 = 34,300
}

void sendTrigPulse() {
  /*
    Ultrasonic module used is HC-SR04. Emits a 40KHz ultrasonic pulse.
    When the Trigger pin is at a HIGH state, a 10 µs pulse is emitted.
    8 40KHz waves are then sent from the module, with the echo pin receiving
    the bounce back pulses, timing the duration of each recieved pulse in microseconds.
  */
  digitalWrite(TRIG, LOW);   //cease emitting.
  delay(10);                 // 8 pulses / 10 µs.
  digitalWrite(TRIG, HIGH);  // Emit pulse.
  delay(10);                 // 8 pulses / 10 µs.
  digitalWrite(TRIG, LOW);   //cease emitting.
}

double getEchoTime() {
  // Collects the time it took to acknolwedge the return pulse wave and returns it.
  return pulseIn(ECHO, HIGH);
}


double getDistance() {
  /*
   * This method initatites the trigger pulses and calls the getEchoTime() method to 
   * 
   */
  sendTrigPulse();                       // emit ultrasonic waves.
  double echoTime = getEchoTime();       // time taken for wave to be detected.
  return calculateDistanceCM(echoTime);  // return the distance in centimeters.
}

void lcdDisplayMessage(String text) {
  /*
     This method takes in a String arguement and displays it to the LCD display followed by
     the measurement.

  */
  lcd.print(text + "cm");  // print the distance
}

void blinkLED(double distance) {
  /*
  * Method to flash an LED with a delay in change of state, 
  * the delay is determined by the arguement given.
  */
  digitalWrite(LED, LOW);   // Turn LED off.
  digitalWrite(LED, HIGH);  // Turn LED on.
  delay(distance);          // delay for {distance} µs
  digitalWrite(LED, LOW);   // Turn LED off.
  delay(distance);          // delay for {distance} µs
}

void loop() {
  /*
    Continuous code execution method.
    This method sends output pulses from the ultrasonic sensor and calculates
    the distance of an object based on the time a return pulse is recieved.
    The distance is then displayed on an LCD display and compared against a pre-defined
    limit. If the limit is surpassed, an LED flashes, otherwise the LED is off.
  */
  double distance = getDistance();      // Initiate distance retrieval.
  lcdDisplayMessage((String)distance);  // Print the distance to the LCD display.
  Serial.println(distance);
  if (distance < limit) {
    // If the object is within the limit e.g. 30cm
    blinkLED(distance);  // Flash LED.
  }
  delay(200);   // Delay for 1/5 second.
  lcd.clear();  // Clear the LCDdisplay.
}

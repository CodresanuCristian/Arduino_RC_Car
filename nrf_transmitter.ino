#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Objects and constants --------------
RF24 remote(7,8); // CE, CSN
const byte ADDRESS[][6] = {"00001", "00002"};  // pipe name through which the two modelues will comunicate.
const int HANDLING = A0;                       // potentiometer for servo motor
const int TORQUE = A1;                         // potentiometer for speed motor
const byte SERVO_MIN_ANGLE = 30;
const byte SERVO_MAX_ANGLE = 105;
const byte TORQUE_MIN_POWER = 0;
const byte TORQUE_MAX_POWER = 250;


struct Data_Package{
  byte servoAngle = 0;
  byte torque = 0; 
};
Data_Package data;


// Variable ---------------------------
boolean message = 0;




// SETUP ==================================================================
void setup() {
  Serial.begin(115200);
  remote.begin();
  remote.openWritingPipe(ADDRESS[1]);    // 00002
  remote.openReadingPipe(1, ADDRESS[0]); // 00001
  remote.setPALevel(RF24_PA_MIN);        // set the Power Amplifier level.
  remote.setDataRate(RF24_250KBPS);      // set Data Rate
}



// LOOP ==================================================================
void loop() {
  // Send data from handling potentiometer to the servo motor
  delay(5);
  remote.stopListening();
  
  int handling = analogRead(HANDLING);
  int torque = analogRead(TORQUE);
  data.servoAngle = map(handling, 0, 1023, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  data.torque = map(torque, 0, 1023, TORQUE_MIN_POWER, TORQUE_MAX_POWER);
  
  remote.write(&data, sizeof(Data_Package));
  Serial.println("Servo Angle: "+(String)data.servoAngle);
  Serial.println("Torque: "+(String)data.torque);    
}

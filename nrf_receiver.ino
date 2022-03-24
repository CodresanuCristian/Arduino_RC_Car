#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Objects and constants --------------
Servo servo;
Servo esc;
RF24 car(7,8); // CE, CSN
const byte ADDRESS[][6] = {"00001", "00002"};  // pipe name through which the two modelues will comunicate.
const byte SERVO_MID_ANGLE = 68;


struct Data_Package{
  byte servoAngle = 0;
  byte torque = 0; 
};
Data_Package data;


// Variable ---------------------------
boolean servoData = false;



// SETUP ==================================================================
void setup() {
  Serial.begin(115200);
  esc.attach(10);
  servo.attach(9);
  servo.write(SERVO_MID_ANGLE);
  car.begin();
  car.openWritingPipe(ADDRESS[0]);     // 00001
  car.openReadingPipe(1, ADDRESS[1]);  // 00002
  car.setPALevel(RF24_PA_MIN);         // set the Power Amplifier level.
  car.setDataRate(RF24_250KBPS);
}



// LOOP ==================================================================
void loop() {
  // Listening  
  delay(5);
  car.startListening();
  if (car.available())
  {
    // if receive data we will send it to the servo and we change the message to true (true = receive data)  
    while (car.available()){
      car.read(&data, sizeof(Data_Package));
      servo.write(data.servoAngle);
      esc.write(data.torque);
      Serial.println("Am primit pentru Servo Angle: "+(String)data.servoAngle);
      Serial.println("Am primit pentru TORQUE: "+(String)data.torque);
    }
  }
}

#include <Arduino.h>
#include <HardwareSerial.h>
#include <ES-INTERGRATE-ODR-01.h>
// Serial_1: RX: GPIO 16, TX: GPIO 17
// put function declarations here:

ES_INTERGRATE_ODR_01 sensor; 

void setup(){
    Serial.begin(115200);
    Serial.println("Initializing...");
    sensor.init(); // initialize the sensor
    Serial.println("Sensor initialized.");
    // Serial.begin(115200); 
    // Serial.println("Initializing...");
    // Serial2.begin(4800, SERIAL_8N1); 
    // Serial.println("Serial1 initialized.");
}

// uint8_t data[8] = {0x01, 0x03, 0x01, 0xf4, 0x00, 0x08, 0x04, 0x02}; // example data
// void loop(){
//   Serial2.write(data, sizeof(data)); // send the data to the sensor
//   Serial.println("Data sent to the sensor.");
//   if (Serial2.available() > 0) { // check if there is data available to read
//     uint8_t recv_data[5 + 2 * 8]; // 5 bytes for the header, 2 bytes for each of the 8 sensors
//     Serial2.readBytes(recv_data, sizeof(recv_data)); // read the data from the sensor
//     Serial.print("Received data: ");
//     for (int i = 0; i < sizeof(recv_data); ++i) {
//       Serial.print("0x");
//       Serial.print(recv_data[i], HEX);
//       Serial.print(", ");
//     }
//     Serial.println();
//   }
//   delay(1000); // delay for 1 second
// }

void loop(){
  float *result = sensor.readAll(); // read all the data from the sensor
  Serial.println("Data: ");
  for (int i = 0; i < 8; ++i) {
    Serial.print(sensor.sensor_name[i]); 
    Serial.print(": ");
    Serial.print(result[i]);
    Serial.print(" ");
    Serial.println(sensor.unit[i]); 
  }
  Serial.println("Data read from the sensor.");
  delay(1000); // delay for 1 second
}

// void NewFunction()
// {
//   if (Serial2.available() > 0)
//   { // check if there is data available to read
//     uint8_t recv_data[8];
//     Serial2.readBytes(recv_data, sizeof(recv_data)); // read the data from the sensor
//     Serial.print("Received data: ");
//     for (int i = 0; i < sizeof(recv_data); ++i)
//     {
//       Serial.print(recv_data[i], HEX);
//       Serial.print(" ");
//     }
//     Serial.println();
//   }
// }

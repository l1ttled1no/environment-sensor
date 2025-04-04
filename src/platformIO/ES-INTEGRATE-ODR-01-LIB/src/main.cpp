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

// uint8_t data[8] = {0x01, 0x03, 0x01, 0xf4, 0x00, 0x00, 0x05, 0xc4}; // example data
void loop(){
  // Serial.print("Humidity: ");
  //   float humid = sensor.read(humidity); 
  //   Serial.print(humid);
  //   Serial.println("%");
  //   Serial.println(); 
  //   delay(1000); 
  // Read all data from the sensor
  // float *val = sensor.readAll(); // read all data from the sensor
  // Serial.println("Sensor read: ");
  // for (int type = humidity; type < all; ++type){
  //   Serial.print(sensor.sensor_name[type]);
  //   Serial.print(": ");
  //   Serial.print(val[type]);
  //   Serial.print(" ");
  //   Serial.println(sensor.unit[type]);
  // }
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

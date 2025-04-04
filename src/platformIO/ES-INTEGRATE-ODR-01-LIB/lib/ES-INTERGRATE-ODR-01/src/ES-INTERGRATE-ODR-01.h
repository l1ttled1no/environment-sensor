/*
    Author: l1ttled1no
    Date: 03/04/2025
    Note from author: 
    - This is the header file of the ES-INTERGRATE-ODR-01 sensor.
    - This is meant to build on ESP-32 only. (Because of using HardwareSerial)
    Changelog: 
    - 03/04/2025: Created the header file and added the basic functions.
*/


#ifndef ES_INTERGRATE_ODR_01_H
#define ES_INTERGRATE_ODR_01_H

// Define some address value of the function sensor 

#define HUMIDITY

#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <HardwareSerial.h>
// #include <SoftwareSerial.h>
// Default address code of the sensor
#define DEFAULT_ADDR_CODE 0x01
#define DEFAULT_BAUD_RATE 4800 
#define DEFAULT_SLAVE_ID 0x01 // Default slave ID of the sensor
#define FUNCTION_CODE 0x03 // Function code of the sensor

// Define the length of the data of the sensor. Data sent: 8 bytes
#define SEND_DATA_LEN 8 // length of the data sent to the sensor
#define DEFAULT_RECV_DATA_LEN 7 // Length for 1 data recv only. 

// Define the register address of the sensor
#define HUMID_ADDR 500 // change to the hex later
#define TEMP_ADDR 501 // change to the hex later
#define NOISE_ADDR 502 // change to the hex later
#define PM25_ADDR 503 // change to the hex later
#define PM10_ADDR 504 // change to the hex later
#define ATM_ADDR 505 // change to the hex later
#define HILUX_ADDR 506 // change to the hex later
#define LOLUX_ADDR 507 // change to the hex later

enum SensorType {
    humidity = 0,
    temp = 1,
    noise = 2,
    pm25 = 3,
    pm10 = 4,
    atm = 5,
    hilux = 6,
    lolux = 7, 
    all = 8
};

class ES_INTERGRATE_ODR_01 {
    private: 
        uint16_t baudRate = 0; 
        uint8_t slave_id = 0; 
        HardwareSerial *serial = nullptr; // serial port of the sensor
        uint8_t div_val [8] = {10, 10, 10, 1, 1, 10, 1, 1}; // actual val = val / div_val[sensortype]
        char *unit[8] = {"%", "C", "dB", "ug/m3", "ug/m3", "atm", "lux", "lux"}; // unit of the sensor
        char *sensor_name[8] = {"Humidity", "Temperature", "Noise", "PM2.5", "PM10", "Air Pressure", "Light Lux", "Low Lux"}; // name of the sensor
    public: 
        ES_INTERGRATE_ODR_01(
            uint16_t baudRate,
            uint8_t slave_id, 
            HardwareSerial serial
        ); 

        void init(); //test for the initialization of the sensor
        float read(uint8_t data);
        float *readAll(); // read all the data from the sensor and return the data in a float array. The data is in the order of humidity, temperature, noise, PM2.5, PM10, air pressure, light lux, low lux.
        void print(int data, HardwareSerial _serial); // print the data to the serial monitor. This one for debug only 
    private: 
        uint16_t _read(uint8_t *data, uint8_t len, SensorType dt); // read the data from the sensor
        uint16_t crc16_modbus(uint8_t *data, uint8_t len); // calculate the crc16 of the data
        

};



#endif // ES_INTERGRATE_ODR_01_H
# ENVIRONMENTAL MONITORING SENSOR USING RS-485 PROTOCOL

## 1. Introduction
This project is designed to monitor environmental parameters such as temperature, humidity, and air quality using a sensor module. The data is transmitted over an RS-485 communication protocol to a microcontroller for processing and display. The system is intended for use in various applications, including smart homes, industrial automation, and environmental monitoring.

## 2. Components Required
- Microcontroller. (Note: I personally use an ESP32 board for this project, to upload the data to MQTT broker)
- ES-INTEGRATE-ODR-01 sensor. (Note: This is a custom sensor module designed for this project. Link to the sensor can be found /href{https://epcb.vn/products/cam-bien-giam-sat-cac-moi-truong-ngoai-troi-sensor-outdoor}{here}.)
- A RS-485 to TTL converter. (Note: This is used to connect the sensor to the PC for coding with Python.)
- A RS-485 to TTL converter. (Note: This is used to connect the sensor to the ESP32 board for coding with Arduino IDE.)
- 12-20V power supply for the sensor module. 
- 5V power supply for the ESP32 board.

## 3. Circuit Diagram

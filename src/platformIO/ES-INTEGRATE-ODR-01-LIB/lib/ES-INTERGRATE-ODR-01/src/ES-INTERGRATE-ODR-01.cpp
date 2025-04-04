/*
    Author: l1ttled1no
    Date: 03/04/2025
     Author: l1ttled1no
    Date: 03/04/2025
    Note from author: 
    - This is the header file of the ES-INTERGRATE-ODR-01 sensor.
    - This is meant to build on ESP-32 only. (Because of using HardwareSerial)
    Changelog: 
    - 03/04/2025: Created the header file and added the basic functions. 
*/
#include "ES-INTERGRATE-ODR-01.h"


ES_INTERGRATE_ODR_01::ES_INTERGRATE_ODR_01(
    uint16_t baudRate = DEFAULT_BAUD_RATE,
    uint8_t slave_id = DEFAULT_SLAVE_ID, 
    HardwareSerial serial = Serial1
) {
    this->baudRate = baudRate; 
    this->slave_id = slave_id; 
    this->serial = &serial; 
}

void ES_INTERGRATE_ODR_01::init() {
    this->serial->begin(this->baudRate); // start the serial port of the sensor
    delay(1000); // wait for the sensor to initialize
    this->serial->flush(); // flush the serial port of the sensor
}

float ES_INTERGRATE_ODR_01::read(uint8_t data){
    if (data < humidity || data > lolux) {
        return -1; // invalid data type
    }
    float val = 0; 
}

float *ES_INTERGRATE_ODR_01::readAll() {   
    //TODO
}

void ES_INTERGRATE_ODR_01::print(int data, HardwareSerial _serial = Serial) { // Set for Serial monitoring. 
    if (data < humidity || data > lolux) {
        _serial.println("Invalid data type. Please enter a valid data type.");
        return;
    }
    else if (data != all) {
        _serial.println("Data: ");
        float val = this->read(data); 
        _serial.print(this->sensor_name[data]); 
        _serial.print(": ");
        _serial.print(val);
        _serial.print(" ");
        _serial.println(this->unit[data]); 
    }
    else {
        float *val = this->readAll();
        _serial.println("Sensor read: ");
        for (int type = humidity; type < all; ++type){
            _serial.print(this->sensor_name[type]);
            _serial.print(": ");
            _serial.print(val[type]);
            _serial.print(" ");
            _serial.println(this->unit[type]);
        } 
    }
}

uint16_t ES_INTERGRATE_ODR_01::_read(uint8_t *data, uint8_t len, SensorType dt = all) {
    if (dt == all) return 0; 
    /*
    uint8_t data = {addr_code, func_code, init_addr (2bytes), num_of_reg (2bytes), crc (2bytes), low_crc_code(1 byte), high_crc_code(1 byte)};
    If size of these not mention, the default is 1 byte for each type (addrcode,... ).
    */
    uint8_t send_data[SEND_DATA_LEN]; 
    send_data[0] = this->slave_id & 0xFF; // slave id
    send_data[1] = FUNCTION_CODE & 0xFF; 
    switch (dt){
        case humidity:
            send_data[2] = HUMID_ADDR & 0xFF; // address of the sensor
            break;
        case temp:
            send_data[2] = TEMP_ADDR & 0xFF; // address of the sensor
            break;
        case noise:
            send_data[2] = NOISE_ADDR & 0xFF; // address of the sensor
            break;
        case pm25:
            send_data[2] = PM25_ADDR & 0xFF; // address of the sensor
            break;
        case pm10:
            send_data[2] = PM10_ADDR & 0xFF; // address of the sensor
            break;
        case atm:
            send_data[2] = ATM_ADDR & 0xFF; // address of the sensor
            break; 
        case hilux:
            send_data[2] = HILUX_ADDR & 0xFF; // address of the sensor
            break;
        case lolux:
            send_data[2] = LOLUX_ADDR & 0xFF; // address of the sensor
            break;
            default: break; 
    }
    // Note: since we only get 1 value, we only need to read 1 register.
    send_data[3] = 0x00; 
    send_data[4] = 0x01; 
    uint16_t crc = crc16_modbus(send_data, SEND_DATA_LEN - 2); // calculate the crc of the data
    send_data[5] = crc & 0xFF; // low byte of crc
    send_data[6] = (crc >> 8) & 0xFF; // high byte of crc 
    // Send the data, then read the data from the sensor
    this->serial->write(send_data, sizeof(send_data)); // send the data to the sensor
    // Set while loop to read the data from the sensor
    while (true){
        if (this->serial->available() > 0) { // check if there is data available to read
            uint8_t recv_data[DEFAULT_RECV_DATA_LEN]; 
            this->serial->readBytes(recv_data, sizeof(recv_data)); // read the data from the sensor
            /*
            receive data format:
            [slave_id (1 byte), function_code (1 byte), effective_byte (1 byte), data (2 bytes), crc (2 bytes)]
            */
            
            // Check if the data is valid
            if (
                recv_data[0] == this->slave_id && // check if the slave id is correct
                recv_data[1] == FUNCTION_CODE
            ) {
                uint8_t data_check[] = {0, 0, 0, 0, 0}; 
                for (int i = 0; i < 5; ++i) {
                    data_check[i] = recv_data[i]; 
                }
                uint16_t crc_check = crc16_modbus(data_check, sizeof(data_check)); // calculate the crc of the data
                if (
                    (crc_check & 0xFF) == recv_data[5]
                    && ((crc_check >> 8) & 0xFF) == recv_data[6] // check if the crc is correct
                )
                {
                    //Start to transform data to integer date (uint16_t)
                    uint16_t result = 0; 
                    for (int i = 0; i < 2; ++i){
                        result = result << 8; // shift left 8 bits
                        result = result | recv_data[i + 3]; 
                    }
                    return result; // return the data
                }
            }
        }
    } 
    return 0; 
}


uint16_t crc16_modbus(const uint8_t *data, uint8_t len) {
    uint16_t crc = 0xFFFF; // Initialize CRC to 0xFFFF
    uint16_t polynomial = 0xA001; // Modbus polynomial
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i]; // XOR byte into the least significant byte of CRC
        for (uint8_t j = 0; j < 8; j++) { // Process each bit
            if (crc & 0x0001) { // If the least significant bit is set
                crc >>= 1; // Shift right
                crc ^= polynomial; // XOR with the polynomial
            } else {
                crc >>= 1; // Just shift right
            }
        }
    }
    return crc; // Return the calculated CRC
}



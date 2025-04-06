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
    - 04/04/2025: Implement read each sensor. read_all is bugging rn dino. fix it.  
*/
#include "ES-INTERGRATE-ODR-01.h"

ES_INTERGRATE_ODR_01::ES_INTERGRATE_ODR_01(){
    this->baudRate = DEFAULT_BAUD_RATE; 
    this->slave_id = DEFAULT_SLAVE_ID; 
    this->serial = &Serial2; // default serial port of the sensor
}

ES_INTERGRATE_ODR_01::ES_INTERGRATE_ODR_01(
    uint16_t baudRate = DEFAULT_BAUD_RATE,
    uint8_t slave_id = DEFAULT_SLAVE_ID, 
    HardwareSerial serial = Serial2
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

float ES_INTERGRATE_ODR_01::read(SensorType data){
    if (data < humidity || data > lolux) {
        return -1; // invalid data type
    }
    float val = 0; 
    uint16_t read_result = this->_read((SensorType)data); // read the data from the sensor
    float result = (float)read_result / this->div_val[data]; // divide the data by the divisor
    return result; 
}

float *ES_INTERGRATE_ODR_01::readAll() {   
    float *result = new float[8]{0}; // dynamically allocate memory for the result array
    uint16_t *read_result = this->_readAll(); // read the data from the sensor
    if (read_result == nullptr) {
        delete[] read_result; // free memory if read_result is null
        return result; // return null if there was an error reading the data
    }
    for (int i = 0; i < 8; ++i) {
        result[i] = (float)read_result[i] / this->div_val[i]; // divide the data by the divisor
    }
    delete[] read_result; // delete the array to free memory
    return result; // return the dynamically allocated array
}

void ES_INTERGRATE_ODR_01::print(SensorType data, HardwareSerial _serial = Serial) { // Set for Serial monitoring. 
    //TODO: read data. 
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

uint16_t ES_INTERGRATE_ODR_01::_read(SensorType dt = all) {
    if (dt == all) {
        return 0; 
        
    }
    /*
    uint8_t data = {addr_code, func_code, init_addr (2bytes), num_of_reg (2bytes), crc (2bytes), low_crc_code(1 byte), high_crc_code(1 byte)};
    If size of these not mention, the default is 1 byte for each type (addrcode,... ).
    */
   else {
       uint8_t send_data[SEND_DATA_LEN]; 
       send_data[0] = this->slave_id & 0xFF; // slave id
       send_data[1] = FUNCTION_CODE & 0xFF; 
       switch (dt){
           case humidity:
               send_data[2] = HUMID_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = HUMID_ADDR & 0xFF; // address of the sensor
               break;
           case temp:
               send_data[2] = TEMP_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = TEMP_ADDR & 0xFF; // address of the sensor
               break;
           case noise:
               send_data[2] = NOISE_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = NOISE_ADDR & 0xFF; // address of the sensor
               break;
           case pm25:
               send_data[2] = PM25_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = PM25_ADDR & 0xFF; // address of the sensor
               break;
           case pm10:
               send_data[2] = PM10_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = PM10_ADDR & 0xFF; // address of the sensor
               break;
           case atm:
               send_data[2] = ATM_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = ATM_ADDR & 0xFF; // address of the sensor
               break; 
           case hilux:
               send_data[2] = HILUX_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = HILUX_ADDR & 0xFF; // address of the sensor
               break;
           case lolux:
               send_data[2] = LOLUX_ADDR >> 8 & 0xFF; // address of the sensor
               send_data[3] = LOLUX_ADDR & 0xFF; // address of the sensor
               break;
               default: break; 
       }
       // Note: since we only get 1 value, we only need to read 1 register.
       send_data[4] = 0x00; 
       send_data[5] = 0x01; 
       uint16_t crc = crc16_modbus(send_data, SEND_DATA_LEN - 2); // calculate the crc of the data
       send_data[6] = crc & 0xFF; // low byte of crc
       send_data[7] = (crc >> 8) & 0xFF; // high byte of crc 
       // Send the data, then read the data from the sensor
       this->serial->write(send_data, sizeof(send_data)); // send the data to the sensor
       delay(100); // wait for the sensor to respond 
   
       // FOR DEBUG: 
    //    Serial.print("Send data: ");
    //    for (int i = 0; i < sizeof(send_data); ++i) {
    //        Serial.print(send_data[i], HEX); // print the data in hex format
    //        Serial.print(" ");
    //    }
    //    Serial.println(); // print a new line
    // //    Serial.print("Send data length: ");
    //    Serial.println(sizeof(send_data)); // print the length of the data
    //    // Set while loop to read the data from the sensor
       while (true){
           delay(100); 
        //    Serial.println("Reading data... "); // print the data
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
}

uint16_t *ES_INTERGRATE_ODR_01::_readAll() {
    uint8_t send_data[SEND_DATA_LEN];

    // Prepare the Modbus request
    send_data[0] = this->slave_id & 0xFF; // Slave ID
    send_data[1] = FUNCTION_CODE & 0xFF; // Function code
    send_data[2] = HUMID_ADDR >> 8 & 0xFF; // High byte of starting address
    send_data[3] = HUMID_ADDR & 0xFF; // Low byte of starting address
    send_data[4] = 0x00; // High byte of number of registers
    send_data[5] = 0x08; // Low byte of number of registers
    uint16_t crc = crc16_modbus(send_data, SEND_DATA_LEN - 2); // Calculate CRC
    send_data[6] = crc & 0xFF; // Low byte of CRC
    send_data[7] = (crc >> 8) & 0xFF; // High byte of CRC

    // Send the request
    this->serial->write(send_data, sizeof(send_data));

    // Timeout mechanism
    unsigned long start_time = millis();
    while (millis() - start_time < 10000) { // 2-second timeout
        if (this->serial->available() > 0) {
            uint8_t recv_data[5 + 2 * 8]; // Expected response size: 5 header bytes + 2 bytes per register
            this->serial->readBytes(recv_data, sizeof(recv_data));

            // Validate the response
            if (recv_data[0] == this->slave_id && recv_data[1] == FUNCTION_CODE) {
                // Calculate CRC on the received data (excluding the CRC bytes)
                uint16_t crc_check = crc16_modbus(recv_data, sizeof(recv_data) - 2);
                if ((crc_check & 0xFF) == recv_data[sizeof(recv_data) - 2] &&
                    ((crc_check >> 8) & 0xFF) == recv_data[sizeof(recv_data) - 1]) {
                    
                    // Parse the data
                    uint16_t *result = new uint16_t[8];
                    for (int i = 0; i < 8; ++i) {
                        result[i] = (recv_data[3 + i * 2] << 8) | recv_data[4 + i * 2];
                    }
                    return result; // Return the parsed data
                } else {
                    // Serial.println("CRC check failed.");
                    return nullptr; // CRC validation failed
                }
            }
        }
    }

    // Serial.println("Sensor response timeout.");
    return nullptr; // Timeout occurred
}

uint16_t ES_INTERGRATE_ODR_01:: crc16_modbus(const uint8_t *data, uint8_t len) {
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



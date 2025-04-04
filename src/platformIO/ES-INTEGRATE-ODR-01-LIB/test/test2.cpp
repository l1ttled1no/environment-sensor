#include <iostream>
#include <cstdint>
#include <iomanip>

using namespace std; 

// uint16_t ES_INTERGRATE_ODR_01::_read(uint8_t *data, uint8_t len, SensorType dt = all) {
//     if (dt == all) return 0; 
//     /*
//     uint8_t data = {addr_code, func_code, init_addr (2bytes), num_of_reg (2bytes), crc (2bytes), low_crc_code(1 byte), high_crc_code(1 byte)};
//     If size of these not mention, the default is 1 byte for each type (addrcode,... ).
//     */
//     uint8_t send_data[SEND_DATA_LEN]; 
//     send_data[0] = this->slave_id & 0xFF; // slave id
//     send_data[1] = FUNCTION_CODE & 0xFF; 
//     switch (dt){
//         case humidity:
//             send_data[2] = HUMID_ADDR & 0xFF; // address of the sensor
//             break;
//         case temp:
//             send_data[2] = TEMP_ADDR & 0xFF; // address of the sensor
//             break;
//         case noise:
//             send_data[2] = NOISE_ADDR & 0xFF; // address of the sensor
//             break;
//         case pm25:
//             send_data[2] = PM25_ADDR & 0xFF; // address of the sensor
//             break;
//         case pm10:
//             send_data[2] = PM10_ADDR & 0xFF; // address of the sensor
//             break;
//         case atm:
//             send_data[2] = ATM_ADDR & 0xFF; // address of the sensor
//             break; 
//         case hilux:
//             send_data[2] = HILUX_ADDR & 0xFF; // address of the sensor
//             break;
//         case lolux:
//             send_data[2] = LOLUX_ADDR & 0xFF; // address of the sensor
//             break;
//             default: break; 
//     }
//     // Note: since we only get 1 value, we only need to read 1 register.
//     send_data[3] = 0x00; 
//     send_data[4] = 0x01; 
//     uint16_t crc = crc16_modbus(send_data, SEND_DATA_LEN - 2); // calculate the crc of the data
//     send_data[5] = crc & 0xFF; // low byte of crc
//     send_data[6] = (crc >> 8) & 0xFF; // high byte of crc 
//     // Send the data, then read the data from the sensor
//     this->serial->write(send_data, sizeof(send_data)); // send the data to the sensor
//     // Set while loop to read the data from the sensor
//     while (true){
//         if (this->serial->available() > 0) { // check if there is data available to read
//             uint8_t recv_data[DEFAULT_RECV_DATA_LEN]; 
//             this->serial->readBytes(recv_data, sizeof(recv_data)); // read the data from the sensor
//             /*
//             receive data format:
//             [slave_id (1 byte), function_code (1 byte), effective_byte (1 byte), data (2 bytes), crc (2 bytes)]
//             */
            
//             // Check if the data is valid
//             if (
//                 recv_data[0] == this->slave_id && // check if the slave id is correct
//                 recv_data[1] == FUNCTION_CODE
//             ) {
//                 uint8_t data_check[] = {0, 0, 0, 0, 0}; 
//                 for (int i = 0; i < 5; ++i) {
//                     data_check[i] = recv_data[i]; 
//                 }
//                 uint16_t crc_check = crc16_modbus(data_check, sizeof(data_check)); // calculate the crc of the data
//                 if (
//                     (crc_check & 0xFF) == recv_data[5]
//                     && ((crc_check >> 8) & 0xFF) == recv_data[6] // check if the crc is correct
//                 )
//                 {
//                     //Start to transform data to integer date (uint16_t)
//                     uint16_t result = 0; 
//                     for (int i = 0; i < 2; ++i){
//                         result = result << 8; // shift left 8 bits
//                         result = result | recv_data[i + 3]; 
//                     }
//                     return result; // return the data
//                 }
//             }
//         }
//     } 
//     return 0; 
// }

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

uint16_t return_res_test (uint8_t *recv_data){
    if (
            recv_data[0] == 0x01 && // check if the slave id is correct
            recv_data[1] == 0x03
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
            return 0; 

}


void testcase1 (){
    uint8_t data [] = {0x01, 0x03, 0x04, 0x02, 0x92, 0xd8, 0x88}; 
    cout << "Data receive (int): ";
    cout << (int)return_res_test(data) << endl;
    cout << "Data receive (hex): ";
    cout << hex << return_res_test(data) << endl;
    cout << endl;
    // return true;
}

int main(){
    testcase1();
    return 0;
}
#include <iostream>
#include <cstdint>
#include <iomanip>

using namespace std;

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

void testcase1() {
    uint8_t data[] = {0x03, 0x03, 0x01, 0xf4, 0x00, 0x02};
    uint8_t len = sizeof(data) / sizeof(data[0]);
    uint16_t crc = crc16_modbus(data, len);
    cout << "CRC: " << hex << uppercase << setw(4) << setfill('0') << crc << endl;
    // Expected Result: 0x85E7
    // add to the end of the data array
    uint8_t last_data[sizeof(data) + 2];
    for (int i = 0; i < sizeof(data); i++) {
        last_data[i] = data[i];
    }
    last_data[sizeof(data)] = (crc >> 8) & 0xFF; // high byte
    last_data[sizeof(data) + 1] = crc & 0xFF; // low byte
    // print the last data in hex format
    cout << "Last Data: ";
    for (int i = 0; i < sizeof(last_data); i++) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)last_data[i] << " ";
    }
    cout << endl;

}

void testcase2() {
    uint8_t data[] = {0x03, 0x03, 0x04, 0x02, 0x92, 0xFF, 0x9B};
    //print data in hex format
    cout << "Data: ";
    for (int i = 0; i < sizeof(data); i++) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)data[i] << " ";
    }
    uint8_t len = sizeof(data) / sizeof(data[0]);
    uint16_t crc = crc16_modbus(data, len);
    cout << "\nCRC: " << hex << setw(4) << setfill('0') << crc << endl;
    // add to the end of the data array
    uint8_t last_data[sizeof(data) + 2];
    for (int i = 0; i < sizeof(data); i++) {
        last_data[i] = data[i];
    }
    last_data[sizeof(data)] = (crc) & 0xFF; // high byte
    last_data[sizeof(data) + 1] = (crc >> 8) & 0xFF; // low byte
    // print the last data in hex format
    cout << "Last Data: ";
    for (int i = 0; i < sizeof(last_data); i++) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)last_data[i] << " ";
    }
    cout << endl;
    // Expected Result: 0x79FD
}

int main() {
    testcase1();
    testcase2();
    return 0;
}

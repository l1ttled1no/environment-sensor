def crc16_modbus(data: bytes) -> int:
    """Calculates the CRC-16-Modbus checksum."""
    crc = 0xFFFF
    polynomial = 0xA001  
    for byte in data:
        crc ^= byte  
        for _ in range(8):  
            lsb_is_one = crc & 0x0001
            crc >>= 1  
            if lsb_is_one:
                crc ^= polynomial  
    return crc


def main():
    data = bytes([0x01, 0x03, 0x04, 0x02, 0x92])
    crc = crc16_modbus(data)
    print(f"CRC-16-Modbus checksum: {crc:#04x}")  # Output: CRC-16-Modbus checksum: 0x92
    # Result: d8 88 
    data_2 = bytes([0x03, 0x03, 0x04, 0x02, 0x92, 0xff, 0x9b])
    crc_2 = crc16_modbus(data_2)
    print(f"CRC-16-Modbus checksum: {crc_2:#04x}")  # Output: CRC-16-Modbus checksum: 0x9b
if __name__ == "__main__":
    main()

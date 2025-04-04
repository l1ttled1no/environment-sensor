import time

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
    # Example usage of the crc16_modbus function
    data = bytearray([0x1, 0x3, 0x10, 0x1, 0xB8, 0x1, 0x17, 0x3, 0x40, 0x0, 0xA, 0x0, 0xE, 0x3, 0xF4, 0x0, 0x0, 0x0, 0x9B])
    crc = crc16_modbus(data)
    print(f"CRC-16-Modbus checksum: {crc:04X}")

# print (hex(0x21f4))
# print (hex(0x21f4 >> 8))

if __name__ == "__main__":
    main()
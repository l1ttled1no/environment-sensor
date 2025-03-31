import serial 
import time

# BAUD = 4800 
# ADDRESS = 1

class ES_Intergrate_ODR_01:
    def __init__(self, port, baudrate, slave_id):
        self.port = port
        self.baudrate = baudrate
        self.slave_id = slave_id
        self.rs485 = None
        self.function_code = [['humidity', hex(500), 10], 
                              ['temperature', hex(501), 10],
                              ['noise', hex(502), 10],
                              ['pm2.5', hex(503), 1],
                              ['pm10', hex(504), 1],
                              ['atmosphere', hex(505), 10], 
                              ['lux', hex(507), 1]]
    
    def init(self):
        try: 
            self.rs485 = serial.Serial(port=self.port, baudrate=self.baudrate, parity=serial.PARITY_NONE, 
                                       stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=1)
            print(f"RS485 initialized on {self.port} at {self.baudrate} baud.")
        except serial.SerialException as e:
            print(f"Error initializing RS485: {e}")
            return False
        return True
    
    def print_info(self): 
        print(f"Port: {self.port}")
        print(f"Baudrate: {self.baudrate}")
        print(f"Slave ID: {self.slave_id}")
    
    def write(self, data):
        try:
            self.rs485.write(data)
            # print(f"Data sent: {data.hex().upper()}")
        except serial.SerialTimeoutException as e:
            print(f"Error sending data: {e}")
    
    def read(self, size=1):
        data = b''
        try:
            data = self.rs485.read(size)
            # print(f"Data received: {data.hex().upper()}")
            return data
        except serial.SerialTimeoutException as e:
            print(f"Error reading data: {e}")
            return None
    
    def crc16_modbus(self, data: bytes) -> int:
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

    def function_opcode(self, data_type='all'):
        result = []
        if data_type == 'all': 
            result = [fc[1] for fc in self.function_code]
        elif len(data_type) < 1 or len(data_type) > len(self.function_code): 
            print("Error: Invalid data type size. The size must be within the range of 1 to 7.")
        else: 
            result = [fc[1] for fc in self.function_code if fc[0] in data_type]
        return result
    
    def read_data(self, one_data_type):
        """
        Build the RS485 request message.
        """
        if one_data_type == 'all':
            msg = bytearray()
            msg.append(self.slave_id)
            msg.append(3)
            start_address = int(self.function_code[0][1], 16)
            msg += start_address.to_bytes(2, byteorder='big')
            num_registers = len(self.function_code) if one_data_type == 'all' else len(one_data_type)
            msg += num_registers.to_bytes(2, byteorder='big')
            crc = self.crc16_modbus(bytes(msg))
            msg += crc.to_bytes(2, byteorder='little')
            
            self.write(msg)

        else: 
            msg = bytearray()
            msg.append(self.slave_id)
            msg.append(3)
            for fc in self.function_code:
                if fc[0] == one_data_type: 
                    start_address = int(fc[1], 16)
                    break
            msg += start_address.to_bytes(2, byteorder='big')
            num_registers = 0x01
            msg += num_registers.to_bytes(2, byteorder='big')
            crc = self.crc16_modbus(bytes(msg))
            msg += crc.to_bytes(2, byteorder='little')

            self.write(msg)
        # time.sleep(0.1)  # Wait for the response
        rx_msg = self.read(8)
        if rx_msg is None: 
            print("Error: No data received.")
            return None
        else: 
            if (one_data_type != 'all'): 
                result = self.process_msg(rx_msg, one_data_type)
                return result
            else:
                return None # do this later
        # return self.read(8)
    def process_msg(self, msg, data_type):
        if data_type == 'all': 
            pass # do this later
        else: 
            """ 
            First byte: Slave ID (Don't need it, ignore)
            Second byte: Function code (Don't need it, ignore)
            Third byte: Effective bytes (Don't need it, ignore)
            Fourth to n-th byte: Data (Need to process, 2 bytes for each data type, 
                but we only need 2 bytes in this because we only read one data type)
            Last two bytes: for CRC only, dont need to process
            """
            data = msg[3:-2]
            data = int.from_bytes(data, byteorder='big')
            # Convert to the data type specified in the function_code list
            for fc in self.function_code: 
                if fc[0] == data_type: 
                    data = data / fc[2]
                    break
            return data
        

def main(): 
    rs485 = ES_Intergrate_ODR_01(port='COM5', baudrate=BAUD, slave_id=Address)
    if rs485.init():
        rs485.print_info()
        # data_type = 'all'
        while True:
            data = rs485.read_data('temperature')
            if data:
                # print("\nData: ", data.hex().upper())
                # process_data = rs485.process_msg(data, 'temperature')
                print("Temperature: ", data, "°C")
                print("Repeating in 3 seconds...")
                for i in range(3, 0, -1):
                    print(".", end='')
                    time.sleep(1)

if __name__ == "__main__":
    main()

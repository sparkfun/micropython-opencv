from .cv2_camera import CV2_Camera
from machine import Pin
from time import sleep_us

class DVP_Camera(CV2_Camera):
    def __init__(
        self,
        i2c,
        i2c_address
    ):
        super().__init__()

        self.i2c = i2c
        self.i2c_address = i2c_address

    def readRegister(self, reg, nbytes=1):
        self.i2c.writeto(self.i2c_address, bytes([reg >> 8, reg & 0xFF]))
        return self.i2c.readfrom(self.i2c_address, nbytes)
    
    def writeRegister(self, reg, data):
        if isinstance(data, int):
            data = bytes([data])
        elif isinstance(data, (list, tuple)):
            data = bytes(data)
        self.i2c.writeto(self.i2c_address, bytes([reg >> 8, reg & 0xFF]) + data)

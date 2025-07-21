from .cv2_touch_screen import CV2_Touch_Screen

# Derived from:
# https://github.com/fbiego/CST816S
class CST816(CV2_Touch_Screen):
    _I2C_ADDRESS = 0x15
    _CHIP_ID = 0xB6
    
    # Registers
    _REG_GESTURE_ID = 0x01
    _REG_FINGER_NUM = 0x02
    _REG_X_POS_H = 0x03
    _REG_X_POS_L = 0x04
    _REG_Y_POS_H = 0x05
    _REG_Y_POS_L = 0x06
    _REG_BPC0H = 0xB0
    _REG_BPC0L = 0xB1
    _REG_BPC1H = 0xB2
    _REG_BPC1L = 0xB3
    _REG_CHIP_ID = 0xA7
    _REG_PROJ_ID = 0xA8
    _REG_FW_VERSION = 0xA9
    _REG_MOTION_MASK = 0xEC
    _REG_IRQ_PULSE_WIDTH = 0xED
    _REG_NOR_SCAN_PER = 0xEE
    _REG_MOTION_SL_ANGLE = 0xEF
    _REG_LP_SCAN_RAW_1H = 0xF0
    _REG_LP_SCAN_RAW_1L = 0xF1
    _REG_LP_SCAN_RAW_2H = 0xF2
    _REG_LP_SCAN_RAW_2L = 0xF3
    _REG_LP_AUTO_WAKE_TIME = 0xF4
    _REG_LP_SCAN_TH = 0xF5
    _REG_LP_SCAN_WIN = 0xF6
    _REG_LP_SCAN_FREQ = 0xF7
    _REG_LP_SCAN_IDAC = 0xF8
    _REG_AUTO_SLEEP_TIME = 0xF9
    _REG_IRQ_CTL = 0xFA
    _REG_AUTO_RESET = 0xFB
    _REG_LONG_PRESS_TIME = 0xFC
    _REG_IO_CTL = 0xFD
    _REG_DIS_AUTO_SLEEP = 0xFE

    def __init__(self, i2c, address=_I2C_ADDRESS, width=240, height=320, rotation=1):
        self.i2c = i2c
        self.address = address
        self.width = width
        self.height = height
        self.rotation = rotation

    def is_connected(self):
        """
        Check if the CST816 touch screen is connected by reading the chip ID.

        Returns:
            bool: True if connected, False otherwise
        """
        try:
            # Try to read the chip ID
            # If it throws an I/O error - the device isn't connected
            chip_id = self.read_register_value(self._REG_CHIP_ID)

            # Confirm the chip ID is correct
            if chip_id == self._CHIP_ID:
                return True
            else:
                return False
        except:
            return False

    def getChipID(self):
        return self.read_register_value(self._REG_CHIP_ID)

    def is_touched(self):
        """
        Check if the touch screen is currently being touched.

        Returns:
            bool: True if touching, False otherwise
        """
        # Read the number of touches
        touch_num = self.read_register_value(self._REG_FINGER_NUM)

        # If there are any touches, return True
        return touch_num > 0

    def get_touch_xy(self):
        x = self.read_register_value(self._REG_X_POS_H, 2) & 0x0FFF
        y = self.read_register_value(self._REG_Y_POS_H, 2) & 0x0FFF

        # Adjust for the rotation
        if self.rotation == 0:
            x,y = x, y
        elif self.rotation == 1:
            x,y = y, self.width - x
        elif self.rotation == 2:
            x,y = self.height - x, self.width - y
        elif self.rotation == 3:
            x,y = self.height - y, x

        return (x, y)

    def read_register_value(self, reg, num_bytes=1):
        """
        Read a single byte from the specified register.

        Args:
            reg (int): Register address to read from

        Returns:
            int: Value read from the register
        """
        data = self.i2c.readfrom_mem(self.address, reg, num_bytes)
        value = 0
        for i in range(num_bytes):
            value = (value << 8) | data[i]
        return value

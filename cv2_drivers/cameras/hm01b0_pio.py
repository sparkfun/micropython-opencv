from .hm01b0 import HM01B0
from .dvp_rp2_pio import DVP_RP2_PIO
from ulab import numpy as np

class HM01B0_PIO(HM01B0, DVP_RP2_PIO):
    """
    OpenCV HM01B0 camera driver using a PIO interface. Only available on
    Raspberry Pi RP2 processors.
    """
    def __init__(
        self,
        i2c,
        sm_id,
        pin_d0,
        pin_vsync,
        pin_hsync,
        pin_pclk,
        pin_xclk = None,
        xclk_freq = 25_000_000,
        num_data_pins = 1,
        i2c_address = 0x24,
    ):
        """
        Initializes the HM01B0 PIO camera driver.

        Args:
            i2c (I2C): I2C object for communication
            sm_id (int): PIO state machine ID
            pin_d0 (int): Data 0 pin number for DVP interface
            pin_vsync (int): Vertical sync pin number
            pin_hsync (int): Horizontal sync pin number
            pin_pclk (int): Pixel clock pin number
            pin_xclk (int, optional): External clock pin number
            xclk_freq (int, optional): Frequency in Hz for the external clock
                Default is 25 MHz
            num_data_pins (int, optional): Number of data pins used in DVP interface
                Default is 1
            i2c_address (int, optional): I2C address of the camera
                Default is 0x24
        """
        # Create the frame buffer
        self._buffer = np.zeros((244, 324), dtype=np.uint8)

        # Call both parent constructors
        DVP_RP2_PIO.__init__(
            self,
            pin_d0,
            pin_vsync,
            pin_hsync,
            pin_pclk,
            pin_xclk,
            xclk_freq,
            sm_id,
            num_data_pins,
            bytes_per_frame = self._buffer.size,
            byte_swap = True
        )
        HM01B0.__init__(
            self,
            i2c,
            i2c_address,
            num_data_pins
        )

    def open(self):
        """
        Opens the camera and prepares it for capturing images.
        """
        self._active(True)

    def release(self):
        """
        Releases the camera and frees any resources.
        """
        self._active(False)

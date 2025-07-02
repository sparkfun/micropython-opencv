from .ov5640 import OV5640
from .dvp_rp2_pio import DVP_RP2_PIO
from ulab import numpy as np

class OV5640_PIO(OV5640, DVP_RP2_PIO):
    def __init__(
        self,
        i2c,
        pin_d0,
        pin_vsync,
        pin_hsync,
        pin_pclk,
        pin_xclk = None,
        xclk_freq = 5_000_000,
        sm_id = 0,
        i2c_address = 0x3c
    ):
        # Create the frame buffer
        self.buffer = np.zeros((240, 320, 2), dtype=np.uint8)

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
            num_data_pins = 8,
            bytes_per_frame = self.buffer.size,
            byte_swap = False
        )
        OV5640.__init__(
            self,
            i2c,
            i2c_address
        )

    def open(self):
        self.active(True)

    def release(self):
        self.active(False)

from .ov5640 import OV5640
from .dvp_rp2_pio import DVP_RP2_PIO

class OV5640_PIO(OV5640, DVP_RP2_PIO):
    def __init__(
        self,
        i2c,
        pin_d0,
        pin_vsync,
        pin_hsync,
        pin_pclk,
        pin_xclk = None,
        sm_id = 0,
        num_data_pins = 8,
        i2c_address = 0x3c,
    ):
        # Call both parent constructors
        DVP_RP2_PIO.__init__(self, pin_d0, pin_vsync, pin_hsync, pin_pclk, pin_xclk, sm_id, num_data_pins)
        OV5640.__init__(self, i2c, i2c_address, num_data_pins)

    def open(self):
        self.active(True)

    def release(self):
        self.active(False)

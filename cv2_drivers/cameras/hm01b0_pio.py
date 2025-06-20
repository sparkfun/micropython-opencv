import rp2
from machine import Pin
from ulab import numpy as np
from time import sleep_us
import cv2

# Derived from:
# https://github.com/openmv/openmv/blob/5acf5baf92b4314a549bdd068138e5df6cc0bac7/drivers/sensors/hm01b0.c
class HM01B0_PIO():
    
    # Read only registers
    MODEL_ID_H = 0x0000
    MODEL_ID_L = 0x0001
    FRAME_COUNT = 0x0005
    PIXEL_ORDER = 0x0006
    # Sensor mode control
    MODE_SELECT = 0x0100
    IMG_ORIENTATION = 0x0101
    SW_RESET = 0x0103
    GRP_PARAM_HOLD = 0x0104
    # Sensor exposure gain control
    INTEGRATION_H = 0x0202
    INTEGRATION_L = 0x0203
    ANALOG_GAIN = 0x0205
    DIGITAL_GAIN_H = 0x020E
    DIGITAL_GAIN_L = 0x020F
    # Frame timing control
    FRAME_LEN_LINES_H = 0x0340
    FRAME_LEN_LINES_L = 0x0341
    LINE_LEN_PCK_H = 0x0342
    LINE_LEN_PCK_L = 0x0343
    # Binning mode control
    READOUT_X = 0x0383
    READOUT_Y = 0x0387
    BINNING_MODE = 0x0390
    # Test pattern control
    TEST_PATTERN_MODE = 0x0601
    # Black level control
    BLC_CFG = 0x1000
    BLC_TGT = 0x1003
    BLI_EN = 0x1006
    BLC2_TGT = 0x1007
    #  Sensor reserved
    DPC_CTRL = 0x1008
    SINGLE_THR_HOT = 0x100B
    SINGLE_THR_COLD = 0x100C
    # VSYNC,HSYNC and pixel shift register
    VSYNC_HSYNC_PIXEL_SHIFT_EN = 0x1012
    # Automatic exposure gain control
    AE_CTRL = 0x2100
    AE_TARGET_MEAN = 0x2101
    AE_MIN_MEAN = 0x2102
    CONVERGE_IN_TH = 0x2103
    CONVERGE_OUT_TH = 0x2104
    MAX_INTG_H = 0x2105
    MAX_INTG_L = 0x2106
    MIN_INTG = 0x2107
    MAX_AGAIN_FULL = 0x2108
    MAX_AGAIN_BIN2 = 0x2109
    MIN_AGAIN = 0x210A
    MAX_DGAIN = 0x210B
    MIN_DGAIN = 0x210C
    DAMPING_FACTOR = 0x210D
    FS_CTRL = 0x210E
    FS_60HZ_H = 0x210F
    FS_60HZ_L = 0x2110
    FS_50HZ_H = 0x2111
    FS_50HZ_L = 0x2112
    FS_HYST_TH = 0x2113
    # Motion detection control
    MD_CTRL = 0x2150
    I2C_CLEAR = 0x2153
    WMEAN_DIFF_TH_H = 0x2155
    WMEAN_DIFF_TH_M = 0x2156
    WMEAN_DIFF_TH_L = 0x2157
    MD_THH = 0x2158
    MD_THM1 = 0x2159
    MD_THM2 = 0x215A
    MD_THL = 0x215B
    STATISTIC_CTRL = 0x2000
    MD_LROI_X_START_H = 0x2011
    MD_LROI_X_START_L = 0x2012
    MD_LROI_Y_START_H = 0x2013
    MD_LROI_Y_START_L = 0x2014
    MD_LROI_X_END_H = 0x2015
    MD_LROI_X_END_L = 0x2016
    MD_LROI_Y_END_H = 0x2017
    MD_LROI_Y_END_L = 0x2018
    MD_INTERRUPT = 0x2160
    #  Sensor timing control
    QVGA_WIN_EN = 0x3010
    SIX_BIT_MODE_EN = 0x3011
    PMU_AUTOSLEEP_FRAMECNT = 0x3020
    ADVANCE_VSYNC = 0x3022
    ADVANCE_HSYNC = 0x3023
    EARLY_GAIN = 0x3035
    #  IO and clock control
    BIT_CONTROL = 0x3059
    OSC_CLK_DIV = 0x3060
    ANA_Register_11 = 0x3061
    IO_DRIVE_STR = 0x3062
    IO_DRIVE_STR2 = 0x3063
    ANA_Register_14 = 0x3064
    OUTPUT_PIN_STATUS_CONTROL = 0x3065
    ANA_Register_17 = 0x3067
    PCLK_POLARITY = 0x3068
    
    # Useful values of Himax registers
    HIMAX_RESET = 0x01
    HIMAX_MODE_STANDBY = 0x00
    HIMAX_MODE_STREAMING = 0x01     # I2C triggered streaming enable
    HIMAX_MODE_STREAMING_NFRAMES = 0x03     # Output N frames
    HIMAX_MODE_STREAMING_TRIG = 0x05     # Hardware Trigger
    # HIMAX_SET_HMIRROR  (r, x)         ((r & 0xFE) | ((x & 1) << 0))
    # HIMAX_SET_VMIRROR  (r, x)         ((r & 0xFD) | ((x & 1) << 1))

    PCLK_RISING_EDGE = 0x00
    PCLK_FALLING_EDGE = 0x01
    AE_CTRL_ENABLE = 0x00
    AE_CTRL_DISABLE = 0x01
    
    HIMAX_BOOT_RETRY = 10
    HIMAX_LINE_LEN_PCK_FULL = 0x178
    HIMAX_FRAME_LENGTH_FULL = 0x109

    HIMAX_LINE_LEN_PCK_QVGA = 0x178
    HIMAX_FRAME_LENGTH_QVGA = 0x104

    HIMAX_LINE_LEN_PCK_QQVGA = 0x178
    HIMAX_FRAME_LENGTH_QQVGA = 0x084

    INIT_COMMANDS = (
        (0x3044,               0x0A),          #  Increase CDS time for settling
        (0x3045,               0x00),          #  Make symmetric for cds_tg and rst_tg
        (0x3047,               0x0A),          #  Increase CDS time for settling
        (0x3050,               0xC0),          #  Make negative offset up to 4x
        (0x3051,               0x42),
        (0x3052,               0x50),
        (0x3053,               0x00),
        (0x3054,               0x03),          #  tuning sf sig clamping as lowest
        (0x3055,               0xF7),          #  tuning dsun
        (0x3056,               0xF8),          #  increase adc nonoverlap clk
        (0x3057,               0x29),          #  increase adc pwr for missing code
        (0x3058,               0x1F),          #  turn on dsun
        (0x3059,               0x1E),
        (0x3064,               0x00),
        (0x3065,               0x04),          #  pad pull 0
        (ANA_Register_17,      0x00),          #  Disable internal oscillator

        (0x1012,               0x00),          #  Sync. shift disable

        (AE_CTRL,              0x01),          #Automatic Exposure
        (AE_TARGET_MEAN,       0x80),          #AE target mean          [Def: 0x3C]
        (AE_MIN_MEAN,          0x0A),          #AE min target mean      [Def: 0x0A]
        (CONVERGE_IN_TH,       0x03),          #Converge in threshold   [Def: 0x03]
        (CONVERGE_OUT_TH,      0x05),          #Converge out threshold  [Def: 0x05]
        (MAX_INTG_H,           (HIMAX_FRAME_LENGTH_QVGA - 2) >> 8),          #Maximum INTG High Byte  [Def: 0x01]
        (MAX_INTG_L,           (HIMAX_FRAME_LENGTH_QVGA - 2) & 0xFF),        #Maximum INTG Low Byte   [Def: 0x54]
        (MAX_AGAIN_FULL,       0x04),          #Maximum Analog gain in full frame mode [Def: 0x03]
        (MAX_AGAIN_BIN2,       0x04),          #Maximum Analog gain in bin2 mode       [Def: 0x04]
        (MAX_DGAIN,            0xC0),

        (INTEGRATION_H,        0x01),          #Integration H           [Def: 0x01]
        (INTEGRATION_L,        0x08),          #Integration L           [Def: 0x08]
        (ANALOG_GAIN,          0x00),          #Analog Global Gain      [Def: 0x00]
        (DAMPING_FACTOR,       0x20),          #Damping Factor          [Def: 0x20]
        (DIGITAL_GAIN_H,       0x01),          #Digital Gain High       [Def: 0x01]
        (DIGITAL_GAIN_L,       0x00),          #Digital Gain Low        [Def: 0x00]

        (MD_CTRL,              0x00),
        (FRAME_LEN_LINES_H,    HIMAX_FRAME_LENGTH_QVGA >> 8),
        (FRAME_LEN_LINES_L,    HIMAX_FRAME_LENGTH_QVGA & 0xFF),
        (LINE_LEN_PCK_H,       HIMAX_LINE_LEN_PCK_QVGA >> 8),
        (LINE_LEN_PCK_L,       HIMAX_LINE_LEN_PCK_QVGA & 0xFF),
        (QVGA_WIN_EN,          0x01),          # Enable QVGA window readout
        (0x3059,               0x22), # 1-bit mode
        (OSC_CLK_DIV,          0x14),
        (IMG_ORIENTATION,      0x00),          # change the orientation
        (0x0104,               0x01),
        (MODE_SELECT,          0x01), # Streaming mode
    )

    def __init__(
        self,
        i2c,
        pin_d0,
        pin_vsync,
        pin_hsync,
        pin_pclk,
        sm_id = 0,
        i2c_address = 0x24,
    ):
        self.i2c = i2c
        self.pin_d0 = pin_d0
        self.pin_vsync = pin_vsync
        self.pin_hsync = pin_hsync
        self.pin_pclk = pin_pclk
        self.sm_id = sm_id
        self.i2c_address = i2c_address
        self.buffer = np.zeros((244, 324), dtype=np.uint8)

        Pin(pin_d0, Pin.IN)
        Pin(pin_vsync, Pin.IN)
        Pin(pin_hsync, Pin.IN)
        Pin(pin_pclk, Pin.IN)

        self.soft_reset()
        self.send_init()
        self.start_pio_dma()
    
    def is_connected(self):
        try:
            # Try to read the chip ID
            # If it throws an I/O error - the device isn't connected
            id = self.getChipID()
            
            # Confirm the chip ID is correct
            if id == 0x01B0:
                return True
            else:
                return False
        except:
            return False

    def getChipID(self):
        """
        Reads the chip ID from the HM01B0 sensor.
        Returns:
            int: The chip ID as a 16-bit integer.
        """
        data = self.readRegister(self.MODEL_ID_H, 2)
        return (data[0] << 8) | data[1]

    def soft_reset(self):
        """
        Performs a software reset of the HM01B0 sensor.
        This resets the sensor to its default state.
        """
        self.writeRegister(self.SW_RESET, self.HIMAX_RESET)

    def setMode(self, mode):
        """
        Sets the operating mode of the HM01B0 sensor.
        Args:
            mode (int): The mode to set, e.g., MODE_STREAMING.
        """
        self.writeRegister(self.MODE_SELECT, mode)

    def trigger(self):
        self.writeRegister(self.MODE_SELECT, self.HIMAX_MODE_STREAMING_NFRAMES)

    def set_n_frames(self, n_frames):
        self.writeRegister(self.PMU_AUTOSLEEP_FRAMECNT, n_frames)

    def send_init(self):
        """
        Initializes the HM01B0 sensor with default settings.
        This includes setting up exposure, gain, and frame timing.
        """
        for reg, value in self.INIT_COMMANDS:
            self.writeRegister(reg, value)
            sleep_us(1000)

    def readRegister(self, reg, nbytes=1):
        self.i2c.writeto(self.i2c_address, bytes([reg >> 8, reg & 0xFF]))
        return self.i2c.readfrom(self.i2c_address, nbytes)
    
    def writeRegister(self, reg, data):
        if isinstance(data, int):
            data = bytes([data])
        elif isinstance(data, (list, tuple)):
            data = bytes(data)
        self.i2c.writeto(self.i2c_address, bytes([reg >> 8, reg & 0xFF]) + data)

    def start_pio_dma(self):
        program = self._pio_read_dvp
        program[0][0] |= self.pin_hsync & 0x1F
        program[0][1] |= self.pin_pclk & 0x1F
        program[0][3] |= self.pin_pclk & 0x1F
        self.sm = rp2.StateMachine(
            self.sm_id,
            program,
            in_base = self.pin_d0
        )
        self.sm.active(1)

        self.dma = rp2.DMA()
        req_num = ((self.sm_id // 4) << 3) + (self.sm_id % 4) + 4
        dma_ctrl = self.dma.pack_ctrl(
            size = 2, # 0 = 8-bit, 1 = 16-bit, 2 = 32-bit
            inc_read = False,
            treq_sel = req_num,
            bswap = True
            # irq_quiet = False
        )
        self.dma.config(
            read = self.sm,
            count = 244 * 324 // 4,
            ctrl = dma_ctrl
        )
        
    def active(self, active = None):
        if active == None:
            return self.sm.active()
        
        self.sm.active(active)

        if active:
            Pin(self.pin_vsync).irq(
                trigger = Pin.IRQ_FALLING,
                handler = lambda pin: self._vsync_handler()
            )
        else:
            Pin(self.pin_vsync).irq(
                handler = None
            )

    def open(self):
        self.active(True)

    def release(self):
        self.active(False)

    def read(self, image = None):
        """
        Reads a frame from the camera.
        Returns:
            tuple: (success, frame)
        """
        return (True, cv2.cvtColor(self.buffer, cv2.COLOR_BayerRG2BGR, image))

    def _vsync_handler(self):
        # Disable DMA before reconfiguring it
        self.dma.active(False)

        # Reset state machine to ensure ISR is cleared
        self.sm.restart()

        # Ensure PIO RX FIFO is empty (it's not emptied by `sm.restart()`)
        while self.sm.rx_fifo() > 0:
            self.sm.get()

        # Reset the DMA write address
        self.dma.write = self.buffer

        # Start the DMA
        self.dma.active(True)

    @rp2.asm_pio(
            in_shiftdir = rp2.PIO.SHIFT_LEFT,
            push_thresh = 32,
            autopush = True
        )
    def _pio_read_dvp():
        wait(1, gpio, 0) # Mask in HSYNC pin
        wait(1, gpio, 0) # Mask in PCLK pin
        in_(pins, 1)     # Mask in number of pins
        wait(0, gpio, 0) # Mask in PCLK pin

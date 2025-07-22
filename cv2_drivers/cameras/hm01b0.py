from .dvp_camera import DVP_Camera
from time import sleep_us
import cv2

# This class is derived from:
# https://github.com/openmv/openmv/blob/5acf5baf92b4314a549bdd068138e5df6cc0bac7/drivers/sensors/hm01b0.c
# Released under the MIT license.
# Copyright (C) 2013-2024 OpenMV, LLC.
class HM01B0(DVP_Camera):
    """
    Base class for OpenCV HM01B0 camera drivers.
    """
    # Read only registers
    _MODEL_ID_H = 0x0000
    _MODEL_ID_L = 0x0001
    _FRAME_COUNT = 0x0005
    _PIXEL_ORDER = 0x0006
    # Sensor mode control
    _MODE_SELECT = 0x0100
    _IMG_ORIENTATION = 0x0101
    _SW_RESET = 0x0103
    _GRP_PARAM_HOLD = 0x0104
    # Sensor exposure gain control
    _INTEGRATION_H = 0x0202
    _INTEGRATION_L = 0x0203
    _ANALOG_GAIN = 0x0205
    _DIGITAL_GAIN_H = 0x020E
    _DIGITAL_GAIN_L = 0x020F
    # Frame timing control
    _FRAME_LEN_LINES_H = 0x0340
    _FRAME_LEN_LINES_L = 0x0341
    _LINE_LEN_PCK_H = 0x0342
    _LINE_LEN_PCK_L = 0x0343
    # Binning mode control
    _READOUT_X = 0x0383
    _READOUT_Y = 0x0387
    _BINNING_MODE = 0x0390
    # Test pattern control
    _TEST_PATTERN_MODE = 0x0601
    # Black level control
    _BLC_CFG = 0x1000
    _BLC_TGT = 0x1003
    _BLI_EN = 0x1006
    _BLC2_TGT = 0x1007
    #  Sensor reserved
    _DPC_CTRL = 0x1008
    _SINGLE_THR_HOT = 0x100B
    _SINGLE_THR_COLD = 0x100C
    # VSYNC,HSYNC and pixel shift register
    _VSYNC_HSYNC_PIXEL_SHIFT_EN = 0x1012
    # Automatic exposure gain control
    _AE_CTRL = 0x2100
    _AE_TARGET_MEAN = 0x2101
    _AE_MIN_MEAN = 0x2102
    _CONVERGE_IN_TH = 0x2103
    _CONVERGE_OUT_TH = 0x2104
    _MAX_INTG_H = 0x2105
    _MAX_INTG_L = 0x2106
    _MIN_INTG = 0x2107
    _MAX_AGAIN_FULL = 0x2108
    _MAX_AGAIN_BIN2 = 0x2109
    _MIN_AGAIN = 0x210A
    _MAX_DGAIN = 0x210B
    _MIN_DGAIN = 0x210C
    _DAMPING_FACTOR = 0x210D
    _FS_CTRL = 0x210E
    _FS_60HZ_H = 0x210F
    _FS_60HZ_L = 0x2110
    _FS_50HZ_H = 0x2111
    _FS_50HZ_L = 0x2112
    _FS_HYST_TH = 0x2113
    # Motion detection control
    _MD_CTRL = 0x2150
    _I2C_CLEAR = 0x2153
    _WMEAN_DIFF_TH_H = 0x2155
    _WMEAN_DIFF_TH_M = 0x2156
    _WMEAN_DIFF_TH_L = 0x2157
    _MD_THH = 0x2158
    _MD_THM1 = 0x2159
    _MD_THM2 = 0x215A
    _MD_THL = 0x215B
    _STATISTIC_CTRL = 0x2000
    _MD_LROI_X_START_H = 0x2011
    _MD_LROI_X_START_L = 0x2012
    _MD_LROI_Y_START_H = 0x2013
    _MD_LROI_Y_START_L = 0x2014
    _MD_LROI_X_END_H = 0x2015
    _MD_LROI_X_END_L = 0x2016
    _MD_LROI_Y_END_H = 0x2017
    _MD_LROI_Y_END_L = 0x2018
    _MD_INTERRUPT = 0x2160
    #  Sensor timing control
    _QVGA_WIN_EN = 0x3010
    _SIX_BIT_MODE_EN = 0x3011
    _PMU_AUTOSLEEP_FRAMECNT = 0x3020
    _ADVANCE_VSYNC = 0x3022
    _ADVANCE_HSYNC = 0x3023
    _EARLY_GAIN = 0x3035
    #  IO and clock control
    _BIT_CONTROL = 0x3059
    _OSC_CLK_DIV = 0x3060
    _ANA_Register_11 = 0x3061
    _IO_DRIVE_STR = 0x3062
    _IO_DRIVE_STR2 = 0x3063
    _ANA_Register_14 = 0x3064
    _OUTPUT_PIN_STATUS_CONTROL = 0x3065
    _ANA_Register_17 = 0x3067
    _PCLK_POLARITY = 0x3068
    
    # Useful values of Himax registers
    _HIMAX_RESET = 0x01
    _HIMAX_MODE_STANDBY = 0x00
    _HIMAX_MODE_STREAMING = 0x01     # I2C triggered streaming enable
    _HIMAX_MODE_STREAMING_NFRAMES = 0x03     # Output N frames
    _HIMAX_MODE_STREAMING_TRIG = 0x05     # Hardware Trigger
    # _HIMAX_SET_HMIRROR  (r, x)         ((r & 0xFE) | ((x & 1) << 0))
    # _HIMAX_SET_VMIRROR  (r, x)         ((r & 0xFD) | ((x & 1) << 1))

    _PCLK_RISING_EDGE = 0x00
    _PCLK_FALLING_EDGE = 0x01
    _AE_CTRL_ENABLE = 0x00
    _AE_CTRL_DISABLE = 0x01

    _HIMAX_BOOT_RETRY = 10
    _HIMAX_LINE_LEN_PCK_FULL = 0x178
    _HIMAX_FRAME_LENGTH_FULL = 0x109

    _HIMAX_LINE_LEN_PCK_QVGA = 0x178
    _HIMAX_FRAME_LENGTH_QVGA = 0x104

    _HIMAX_LINE_LEN_PCK_QQVGA = 0x178
    _HIMAX_FRAME_LENGTH_QQVGA = 0x084

    _INIT_COMMANDS = (
        (_BLC_TGT,              0x08),          #  BLC target :8  at 8 bit mode
        (_BLC2_TGT,             0x08),          #  BLI target :8  at 8 bit mode
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
        (_ANA_Register_17,      0x00),          #  Disable internal oscillator

        (_BLC_CFG,              0x43),          #  BLC_on, IIR

        (0x1001,               0x43),          #  BLC dithering en
        (0x1002,               0x43),          #  blc_darkpixel_thd
        (0x0350,               0x7F),          #  Dgain Control
        (_BLI_EN,               0x01),          #  BLI enable
        (0x1003,               0x00),          #  BLI Target [Def: 0x20]

        (_DPC_CTRL,             0x01),          #  DPC option 0: DPC off   1 : mono   3 : bayer1   5 : bayer2
        (0x1009,               0xA0),          #  cluster hot pixel th
        (0x100A,               0x60),          #  cluster cold pixel th
        (_SINGLE_THR_HOT,       0x90),          #  single hot pixel th
        (_SINGLE_THR_COLD,      0x40),          #  single cold pixel th
        (0x1012,               0x00),          #  Sync. shift disable
        (_STATISTIC_CTRL,       0x07),          #  AE stat en | MD LROI stat en | magic
        (0x2003,               0x00),
        (0x2004,               0x1C),
        (0x2007,               0x00),
        (0x2008,               0x58),
        (0x200B,               0x00),
        (0x200C,               0x7A),
        (0x200F,               0x00),
        (0x2010,               0xB8),
        (0x2013,               0x00),
        (0x2014,               0x58),
        (0x2017,               0x00),
        (0x2018,               0x9B),

        (_AE_CTRL,              0x01),          #Automatic Exposure
        (_AE_TARGET_MEAN,       0x64),          #AE target mean          [Def: 0x3C]
        (_AE_MIN_MEAN,          0x0A),          #AE min target mean      [Def: 0x0A]
        (_CONVERGE_IN_TH,       0x03),          #Converge in threshold   [Def: 0x03]
        (_CONVERGE_OUT_TH,      0x05),          #Converge out threshold  [Def: 0x05]
        (_MAX_INTG_H,           (_HIMAX_FRAME_LENGTH_QVGA - 2) >> 8),          #Maximum INTG High Byte  [Def: 0x01]
        (_MAX_INTG_L,           (_HIMAX_FRAME_LENGTH_QVGA - 2) & 0xFF),        #Maximum INTG Low Byte   [Def: 0x54]
        (_MAX_AGAIN_FULL,       0x04),          #Maximum Analog gain in full frame mode [Def: 0x03]
        (_MAX_AGAIN_BIN2,       0x04),          #Maximum Analog gain in bin2 mode       [Def: 0x04]
        (_MAX_DGAIN,            0xC0),

        (_INTEGRATION_H,        0x01),          #Integration H           [Def: 0x01]
        (_INTEGRATION_L,        0x08),          #Integration L           [Def: 0x08]
        (_ANALOG_GAIN,          0x00),          #Analog Global Gain      [Def: 0x00]
        (_DAMPING_FACTOR,       0x20),          #Damping Factor          [Def: 0x20]
        (_DIGITAL_GAIN_H,       0x01),          #Digital Gain High       [Def: 0x01]
        (_DIGITAL_GAIN_L,       0x00),          #Digital Gain Low        [Def: 0x00]

        (_FS_CTRL,              0x00),          #Flicker Control

        (_FS_60HZ_H,            0x00),
        (_FS_60HZ_L,            0x3C),
        (_FS_50HZ_H,            0x00),
        (_FS_50HZ_L,            0x32),

        (_MD_CTRL,              0x00),
        (_FRAME_LEN_LINES_H,    _HIMAX_FRAME_LENGTH_QVGA >> 8),
        (_FRAME_LEN_LINES_L,    _HIMAX_FRAME_LENGTH_QVGA & 0xFF),
        (_LINE_LEN_PCK_H,       _HIMAX_LINE_LEN_PCK_QVGA >> 8),
        (_LINE_LEN_PCK_L,       _HIMAX_LINE_LEN_PCK_QVGA & 0xFF),
        (_QVGA_WIN_EN,          0x01),          # Enable QVGA window readout
        (0x0383,               0x01),
        (0x0387,               0x01),
        (0x0390,               0x00),
        (0x3011,               0x70),
        (0x3059,               0x22), # 1-bit mode
        (_OSC_CLK_DIV,          0x14),
        (_IMG_ORIENTATION,      0x00),          # change the orientation
        (0x0104,               0x01),
        (_MODE_SELECT,          0x01), # Streaming mode
    )

    def __init__(
        self,
        i2c,
        i2c_address = 0x24,
        num_data_pins = 1
    ):
        """
        Initializes the HM01B0 camera with default settings.

        Args:
            i2c (I2C): I2C object for communication
            i2c_address (int, optional): I2C address (default: 0x24)
            num_data_pins (int, optional): Number of data pins
                - 1 (Default)
                - 4
                - 8
        """
        super().__init__(i2c, i2c_address)

        self._soft_reset()
        self._send_init(num_data_pins)
    
    def _is_connected(self):
        """
        Checks if the camera is connected by reading the chip ID.

        Returns:
            bool: True if the camera is connected and the chip ID is correct,
                  otherwise False.
        """
        try:
            # Try to read the chip ID
            # If it throws an I/O error - the device isn't connected
            id = self._get_chip_id()
            
            # Confirm the chip ID is correct
            if id == 0x01B0:
                return True
            else:
                return False
        except:
            return False

    def _get_chip_id(self):
        """
        Reads the chip ID.

        Returns:
            int: The chip ID of the HM01B0 (should be 0x01B0).
        """
        data = self._read_register(self._MODEL_ID_H, 2)
        return (data[0] << 8) | data[1]

    def _soft_reset(self):
        """
        Performs a software reset of the HM01B0 sensor.
        This resets the sensor to its default state.
        """
        # HM01B0 can require multiple attempts to reset properly
        for i in range(self._HIMAX_BOOT_RETRY):
            self._write_register(self._SW_RESET, self._HIMAX_RESET)
            sleep_us(1000)
            mode = self._read_register(self._MODE_SELECT)
            if mode[0] == self._HIMAX_MODE_STANDBY:
                break
            sleep_us(10000)

    def _set_mode(self, mode):
        """
        Sets the operating mode of the HM01B0 sensor.
        Args:
            mode (int): The mode to set, e.g., MODE_STREAMING.
        """
        self._write_register(self._MODE_SELECT, mode)

    def _trigger(self):
        self._write_register(self._MODE_SELECT, self._HIMAX_MODE_STREAMING_NFRAMES)

    def _set_n_frames(self, n_frames):
        self._write_register(self._PMU_AUTOSLEEP_FRAMECNT, n_frames)

    def _send_init(self, num_data_pins):
        """
        Initializes the HM01B0 sensor with default settings.
        This includes setting up exposure, gain, and frame timing.
        """
        for reg, value in self._INIT_COMMANDS:
            if reg == 0x3059:
                # Set the data pin mode based on the number of data pins
                if num_data_pins == 1:
                    value = 0x22
                elif num_data_pins == 4:
                    value = 0x42
                else:
                    value = 0x02
            self._write_register(reg, value)
            sleep_us(1000)

    def read(self, image=None):
        """
        Reads an image from the camera.

        Args:
            image (ndarray, optional): Image to read into

        Returns:
            tuple: (success, image)
                - success (bool): True if the image was read, otherwise False
                - image (ndarray): The captured image, or None if reading failed
        """
        return (True, cv2.cvtColor(self._buffer, cv2.COLOR_BayerRG2BGR, image))

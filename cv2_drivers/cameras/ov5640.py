from .dvp_camera import DVP_Camera
from time import sleep_us
import cv2
# from micropython import const

# Derived from:
# https://github.com/adafruit/Adafruit_CircuitPython_OV5640
class OV5640(DVP_Camera):

    OV5640_COLOR_RGB = 0
    OV5640_COLOR_YUV = 1
    OV5640_COLOR_GRAYSCALE = 2
    OV5640_COLOR_JPEG = 3

    # fmt: off

    _SYSTEM_RESET00 = 0x3000 # Reset for Individual Block
    # (0: enable block; 1: reset block)
    # Bit[7]: Reset BIST
    # Bit[6]: Reset MCU program memory
    # Bit[5]: Reset MCU
    # Bit[4]: Reset OTP
    # Bit[3]: Reset STB
    # Bit[2]: Reset d5060
    # Bit[1]: Reset timing control
    # Bit[0]: Reset array control

    _SYSTEM_RESET02 = 0x3002 # Reset for Individual Block
    # (0: enable block; 1: reset block)
    # Bit[7]: Reset VFIFO
    # Bit[5]: Reset format
    # Bit[4]: Reset JFIFO
    # Bit[3]: Reset SFIFO
    # Bit[2]: Reset JPG
    # Bit[1]: Reset format MUX
    # Bit[0]: Reset average

    _CLOCK_ENABLE02 = 0x3006 # Clock Enable Control
    # (0: disable clock; 1: enable clock)
    # Bit[7]: Enable PSRAM clock
    # Bit[6]: Enable FMT clock
    # Bit[5]: Enable JPEG 2x clock
    # Bit[3]: Enable JPEG clock
    # Bit[1]: Enable format MUX clock
    # Bit[0]: Enable average clock

    _SYSTEM_CTROL0 = 0x3008
    # Bit[7]: Software reset
    # Bit[6]: Software power down
    # Bit[5]: Reserved
    # Bit[4]: SRB clock SYNC enable
    # Bit[3]: Isolation suspend select
    # Bit[2:0]: Not used

    _CHIP_ID_HIGH = 0x300A

    _DRIVE_CAPABILITY = 0x302C
    # Bit[7:6]:
    #          00: 1x
    #          01: 2x
    #          10: 3x
    #          11: 4x

    _SC_PLLS_CTRL0 = 0x303A
    # Bit[7]: PLLS bypass
    _SC_PLLS_CTRL1 = 0x303B
    # Bit[4:0]: PLLS multiplier
    _SC_PLLS_CTRL2 = 0x303C
    # Bit[6:4]: PLLS charge pump control
    # Bit[3:0]: PLLS system divider
    _SC_PLLS_CTRL3 = 0x303D
    # Bit[5:4]: PLLS pre-divider
    #          00: 1
    #          01: 1.5
    #          10: 2
    #          11: 3
    # Bit[2]: PLLS root-divider - 1
    # Bit[1:0]: PLLS seld5
    #          00: 1
    #          01: 1
    #          10: 2
    #          11: 2.5

    # AEC/AGC control functions
    _AEC_PK_MANUAL = 0x3503
    # AEC Manual Mode Control
    # Bit[7:6]: Reserved
    # Bit[5]: Gain delay option
    #         Valid when 0x3503[4]=1’b0
    #         0: Delay one frame latch
    #         1: One frame latch
    # Bit[4:2]: Reserved
    # Bit[1]: AGC manual
    #         0: Auto enable
    #         1: Manual enable
    # Bit[0]: AEC manual
    #         0: Auto enable
    #         1: Manual enable

    # gain = {0x350A[1:0], 0x350B[7:0]} / 16


    _X_ADDR_ST_H = 0x3800
    # Bit[3:0]: X address start[11:8]
    _X_ADDR_ST_L = 0x3801
    # Bit[7:0]: X address start[7:0]
    _Y_ADDR_ST_H = 0x3802
    # Bit[2:0]: Y address start[10:8]
    _Y_ADDR_ST_L = 0x3803
    # Bit[7:0]: Y address start[7:0]
    _X_ADDR_END_H = 0x3804
    # Bit[3:0]: X address end[11:8]
    _X_ADDR_END_L = 0x3805
    # Bit[7:0]:
    _Y_ADDR_END_H = 0x3806
    # Bit[2:0]: Y address end[10:8]
    _Y_ADDR_END_L = 0x3807
    # Bit[7:0]:
    # Size after scaling
    _X_OUTPUT_SIZE_H = 0x3808
    # Bit[3:0]: DVP output horizontal width[11:8]
    _X_OUTPUT_SIZE_L = 0x3809
    # Bit[7:0]:
    _Y_OUTPUT_SIZE_H = 0x380A
    # Bit[2:0]: DVP output vertical height[10:8]
    _Y_OUTPUT_SIZE_L = 0x380B
    # Bit[7:0]:
    _X_TOTAL_SIZE_H = 0x380C
    # Bit[3:0]: Total horizontal size[11:8]
    _X_TOTAL_SIZE_L = 0x380D
    # Bit[7:0]:
    _Y_TOTAL_SIZE_H = 0x380E
    # Bit[7:0]: Total vertical size[15:8]
    _Y_TOTAL_SIZE_L = 0x380F
    # Bit[7:0]:
    _X_OFFSET_H = 0x3810
    # Bit[3:0]: ISP horizontal offset[11:8]
    _X_OFFSET_L = 0x3811
    # Bit[7:0]:
    _Y_OFFSET_H = 0x3812
    # Bit[2:0]: ISP vertical offset[10:8]
    _Y_OFFSET_L = 0x3813
    # Bit[7:0]:
    _X_INCREMENT = 0x3814
    # Bit[7:4]: Horizontal odd subsample increment
    # Bit[3:0]: Horizontal even subsample increment
    _Y_INCREMENT = 0x3815
    # Bit[7:4]: Vertical odd subsample increment
    # Bit[3:0]: Vertical even subsample increment
    # Size before scaling
    # X_INPUT_SIZE =    (X_ADDR_END - X_ADDR_ST + 1 - (2 * X_OFFSET))
    # Y_INPUT_SIZE =    (Y_ADDR_END - Y_ADDR_ST + 1 - (2 * Y_OFFSET))

    # mirror and flip registers
    _TIMING_TC_REG20 = 0x3820
    # Timing Control Register
    # Bit[2:1]: Vertical flip enable
    #         00: Normal
    #         11: Vertical flip
    # Bit[0]: Vertical binning enable
    _TIMING_TC_REG21 = 0x3821
    # Timing Control Register
    # Bit[5]: Compression Enable
    # Bit[2:1]: Horizontal mirror enable
    #         00: Normal
    #         11: Horizontal mirror
    # Bit[0]: Horizontal binning enable

    _PCLK_RATIO = 0x3824
    # Bit[4:0]: PCLK ratio manual

    # frame control registers
    _FRAME_CTRL01 = 0x4201
    # Control Passed Frame Number When both ON and OFF number set to 0x00,frame
    # control is in bypass mode
    # Bit[7:4]: Not used
    # Bit[3:0]: Frame ON number
    _FRAME_CTRL02 = 0x4202
    # Control Masked Frame Number When both ON and OFF number set to 0x00,frame
    # control is in bypass mode
    # Bit[7:4]: Not used
    # BIT[3:0]: Frame OFF number

    # format control registers
    _FORMAT_CTRL00 = 0x4300

    _CLOCK_POL_CONTROL = 0x4740
    # Bit[5]: PCLK polarity 0: active low
    #          1: active high
    # Bit[3]: Gate PCLK under VSYNC
    # Bit[2]: Gate PCLK under HREF
    # Bit[1]: HREF polarity
    #          0: active low
    #          1: active high
    # Bit[0] VSYNC polarity
    #          0: active low
    #          1: active high

    _ISP_CONTROL_01 = 0x5001
    # Bit[5]: Scale enable
    #          0: Disable
    #          1: Enable

    # output format control registers
    _FORMAT_CTRL = 0x501F
    # Format select
    # Bit[2:0]:
    #  000: YUV422
    #  001: RGB
    #  010: Dither
    #  011: RAW after DPC
    #  101: RAW after CIP

    # ISP top control registers
    _PRE_ISP_TEST_SETTING_1 = 0x503D
    # Bit[7]: Test enable
    #         0: Test disable
    #         1: Color bar enable
    # Bit[6]: Rolling
    # Bit[5]: Transparent
    # Bit[4]: Square black and white
    # Bit[3:2]: Color bar style
    #         00: Standard 8 color bar
    #         01: Gradual change at vertical mode 1
    #         10: Gradual change at horizontal
    #         11: Gradual change at vertical mode 2
    # Bit[1:0]: Test select
    #         00: Color bar
    #         01: Random data
    #         10: Square data
    #         11: Black image

    # exposure = {0x3500[3:0], 0x3501[7:0], 0x3502[7:0]} / 16 × tROW

    _SCALE_CTRL_1 = 0x5601
    # Bit[6:4]: HDIV RW
    #          DCW scale times
    #          000: DCW 1 time
    #          001: DCW 2 times
    #          010: DCW 4 times
    #          100: DCW 8 times
    #          101: DCW 16 times
    #          Others: DCW 16 times
    # Bit[2:0]: VDIV RW
    #          DCW scale times
    #          000: DCW 1 time
    #          001: DCW 2 times
    #          010: DCW 4 times
    #          100: DCW 8 times
    #          101: DCW 16 times
    #          Others: DCW 16 times

    _SCALE_CTRL_2 = 0x5602
    # X_SCALE High Bits
    _SCALE_CTRL_3 = 0x5603
    # X_SCALE Low Bits
    _SCALE_CTRL_4 = 0x5604
    # Y_SCALE High Bits
    _SCALE_CTRL_5 = 0x5605
    # Y_SCALE Low Bits
    _SCALE_CTRL_6 = 0x5606
    # Bit[3:0]: V Offset

    _VFIFO_CTRL0C = 0x460C
    # Bit[1]: PCLK manual enable
    #          0: Auto
    #          1: Manual by PCLK_RATIO

    _VFIFO_X_SIZE_H = 0x4602
    _VFIFO_X_SIZE_L = 0x4603
    _VFIFO_Y_SIZE_H = 0x4604
    _VFIFO_Y_SIZE_L = 0x4605

    _COMPRESSION_CTRL00 = 0x4400
    _COMPRESSION_CTRL01 = 0x4401
    _COMPRESSION_CTRL02 = 0x4402
    _COMPRESSION_CTRL03 = 0x4403
    _COMPRESSION_CTRL04 = 0x4404
    _COMPRESSION_CTRL05 = 0x4405
    _COMPRESSION_CTRL06 = 0x4406
    _COMPRESSION_CTRL07 = 0x4407
    # Bit[5:0]: QS
    _COMPRESSION_ISI_CTRL = 0x4408
    _COMPRESSION_CTRL09 = 0x4409
    _COMPRESSION_CTRL0A = 0x440A
    _COMPRESSION_CTRL0B = 0x440B
    _COMPRESSION_CTRL0C = 0x440C
    _COMPRESSION_CTRL0D = 0x440D
    _COMPRESSION_CTRL0E = 0x440E

    _TEST_COLOR_BAR = 0xC0
    # Enable Color Bar roling Test

    _AEC_PK_MANUAL_AGC_MANUALEN = 0x02
    # Enable AGC Manual enable
    _AEC_PK_MANUAL_AEC_MANUALEN = 0x01
    # Enable AEC Manual enable

    _TIMING_TC_REG20_VFLIP = 0x06
    # Vertical flip enable
    _TIMING_TC_REG21_HMIRROR = 0x06
    # Horizontal mirror enable

    OV5640_SIZE_96X96 = 0  # 96x96
    OV5640_SIZE_QQVGA = 1  # 160x120
    OV5640_SIZE_QCIF = 2  # 176x144
    OV5640_SIZE_HQVGA = 3  # 240x176
    OV5640_SIZE_240X240 = 4  # 240x240
    OV5640_SIZE_QVGA = 5  # 320x240
    OV5640_SIZE_CIF = 6  # 400x296
    OV5640_SIZE_HVGA = 7  # 480x320
    OV5640_SIZE_VGA = 8  # 640x480
    OV5640_SIZE_SVGA = 9  # 800x600
    OV5640_SIZE_XGA = 10  # 1024x768
    OV5640_SIZE_HD = 11  # 1280x720
    OV5640_SIZE_SXGA = 12  # 1280x1024
    OV5640_SIZE_UXGA = 13  # 1600x1200
    OV5640_SIZE_QHDA = 14  # 2560x1440
    OV5640_SIZE_WQXGA = 15  # 2560x1600
    OV5640_SIZE_PFHD = 16  # 1088x1920
    OV5640_SIZE_QSXGA = 17  # 2560x1920

    _ASPECT_RATIO_4X3 = 0
    _ASPECT_RATIO_3X2 = 1
    _ASPECT_RATIO_16X10 = 2
    _ASPECT_RATIO_5X3 = 3
    _ASPECT_RATIO_16X9 = 4
    _ASPECT_RATIO_21X9 = 5
    _ASPECT_RATIO_5X4 = 6
    _ASPECT_RATIO_1X1 = 7
    _ASPECT_RATIO_9X16 = 8

    _resolution_info = [
        [96, 96, _ASPECT_RATIO_1X1],  # 96x96
        [160, 120, _ASPECT_RATIO_4X3],  # QQVGA
        [176, 144, _ASPECT_RATIO_5X4],  # QCIF
        [240, 176, _ASPECT_RATIO_4X3],  # HQVGA
        [240, 240, _ASPECT_RATIO_1X1],  # 240x240
        [320, 240, _ASPECT_RATIO_4X3],  # QVGA
        [400, 296, _ASPECT_RATIO_4X3],  # CIF
        [480, 320, _ASPECT_RATIO_3X2],  # HVGA
        [640, 480, _ASPECT_RATIO_4X3],  # VGA
        [800, 600, _ASPECT_RATIO_4X3],  # SVGA
        [1024, 768, _ASPECT_RATIO_4X3],  # XGA
        [1280, 720, _ASPECT_RATIO_16X9],  # HD
        [1280, 1024, _ASPECT_RATIO_5X4],  # SXGA
        [1600, 1200, _ASPECT_RATIO_4X3],  # UXGA
        [2560, 1440, _ASPECT_RATIO_16X9], # QHD
        [2560, 1600, _ASPECT_RATIO_16X10], # WQXGA
        [1088, 1920, _ASPECT_RATIO_9X16], # Portrait FHD
        [2560, 1920, _ASPECT_RATIO_4X3], # QSXGA

    ]


    _ratio_table = [
        #  mw,   mh,  sx,  sy,   ex,   ey, ox, oy,   tx,   ty
        [2560, 1920, 0, 0, 2623, 1951, 32, 16, 2844, 1968],  # 4x3
        [2560, 1704, 0, 110, 2623, 1843, 32, 16, 2844, 1752],  # 3x2
        [2560, 1600, 0, 160, 2623, 1791, 32, 16, 2844, 1648],  # 16x10
        [2560, 1536, 0, 192, 2623, 1759, 32, 16, 2844, 1584],  # 5x3
        [2560, 1440, 0, 240, 2623, 1711, 32, 16, 2844, 1488],  # 16x9
        [2560, 1080, 0, 420, 2623, 1531, 32, 16, 2844, 1128],  # 21x9
        [2400, 1920, 80, 0, 2543, 1951, 32, 16, 2684, 1968],  # 5x4
        [1920, 1920, 320, 0, 2543, 1951, 32, 16, 2684, 1968],  # 1x1
        [1088, 1920, 736, 0, 1887, 1951, 32, 16, 1884, 1968],  # 9x16
    ]

    _pll_pre_div2x_factors = [1, 1, 2, 3, 4, 1.5, 6, 2.5, 8]
    _pll_pclk_root_div_factors = [1,2,4,8]

    _REG_DLY = 0xFFFF
    _REGLIST_TAIL = 0x0000

    _OV5640_STAT_FIRMWAREBAD = 0x7F
    _OV5640_STAT_STARTUP = 0x7E
    _OV5640_STAT_IDLE = 0x70
    _OV5640_STAT_FOCUSING = 0x00
    _OV5640_STAT_FOCUSED = 0x10

    _OV5640_CMD_TRIGGER_AUTOFOCUS = 0x03
    _OV5640_CMD_AUTO_AUTOFOCUS = 0x04
    _OV5640_CMD_RELEASE_FOCUS = 0x08
    _OV5640_CMD_AF_SET_VCM_STEP = 0x1A
    _OV5640_CMD_AF_GET_VCM_STEP = 0x1B

    _OV5640_CMD_MAIN = 0x3022
    _OV5640_CMD_ACK = 0x3023
    _OV5640_CMD_PARA0 = 0x3024
    _OV5640_CMD_PARA1 = 0x3025
    _OV5640_CMD_PARA2 = 0x3026
    _OV5640_CMD_PARA3 = 0x3027
    _OV5640_CMD_PARA4 = 0x3028
    _OV5640_CMD_FW_STATUS = 0x3029


    _sensor_default_regs = [
        _SYSTEM_CTROL0, 0x82,  # software reset
        _REG_DLY, 10,  # delay 10ms
        _SYSTEM_CTROL0, 0x42,  # power down
        # enable pll
        0x3103, 0x13,
        # io direction
        0x3017, 0xFF,
        0x3018, 0xFF,
        _DRIVE_CAPABILITY, 0xC3,
        _CLOCK_POL_CONTROL, 0x21,
        0x4713, 0x02,  # jpg mode select
        _ISP_CONTROL_01, 0x83,  # turn color matrix, awb and SDE
        # sys reset
        _SYSTEM_RESET00, 0x00, # enable all blocks
        _SYSTEM_RESET02, 0x1C, # reset jfifo, sfifo, jpg, fmux, avg
        # clock enable
        0x3004, 0xFF,
        _CLOCK_ENABLE02, 0xC3,
        # isp control
        0x5000, 0xA7,
        _ISP_CONTROL_01, 0xA3,  # +scaling?
        0x5003, 0x08,  # special_effect
        # unknown
        0x370C, 0x02,  #!!IMPORTANT
        0x3634, 0x40,  #!!IMPORTANT
        # AEC/AGC
        0x3A02, 0x03,
        0x3A03, 0xD8,
        0x3A08, 0x01,
        0x3A09, 0x27,
        0x3A0A, 0x00,
        0x3A0B, 0xF6,
        0x3A0D, 0x04,
        0x3A0E, 0x03,
        0x3A0F, 0x30,  # ae_level
        0x3A10, 0x28,  # ae_level
        0x3A11, 0x60,  # ae_level
        0x3A13, 0x43,
        0x3A14, 0x03,
        0x3A15, 0xD8,
        0x3A18, 0x00,  # gainceiling
        0x3A19, 0xF8,  # gainceiling
        0x3A1B, 0x30,  # ae_level
        0x3A1E, 0x26,  # ae_level
        0x3A1F, 0x14,  # ae_level
        # vcm debug
        0x3600, 0x08,
        0x3601, 0x33,
        # 50/60Hz
        0x3C01, 0xA4,
        0x3C04, 0x28,
        0x3C05, 0x98,
        0x3C06, 0x00,
        0x3C07, 0x08,
        0x3C08, 0x00,
        0x3C09, 0x1C,
        0x3C0A, 0x9C,
        0x3C0B, 0x40,
        0x460C, 0x22,  # disable jpeg footer
        # BLC
        0x4001, 0x02,
        0x4004, 0x02,
        # AWB
        0x5180, 0xFF,
        0x5181, 0xF2,
        0x5182, 0x00,
        0x5183, 0x14,
        0x5184, 0x25,
        0x5185, 0x24,
        0x5186, 0x09,
        0x5187, 0x09,
        0x5188, 0x09,
        0x5189, 0x75,
        0x518A, 0x54,
        0x518B, 0xE0,
        0x518C, 0xB2,
        0x518D, 0x42,
        0x518E, 0x3D,
        0x518F, 0x56,
        0x5190, 0x46,
        0x5191, 0xF8,
        0x5192, 0x04,
        0x5193, 0x70,
        0x5194, 0xF0,
        0x5195, 0xF0,
        0x5196, 0x03,
        0x5197, 0x01,
        0x5198, 0x04,
        0x5199, 0x12,
        0x519A, 0x04,
        0x519B, 0x00,
        0x519C, 0x06,
        0x519D, 0x82,
        0x519E, 0x38,
        # color matrix (Saturation)
        0x5381, 0x1E,
        0x5382, 0x5B,
        0x5383, 0x08,
        0x5384, 0x0A,
        0x5385, 0x7E,
        0x5386, 0x88,
        0x5387, 0x7C,
        0x5388, 0x6C,
        0x5389, 0x10,
        0x538A, 0x01,
        0x538B, 0x98,
        # CIP control (Sharpness)
        0x5300, 0x10,  # sharpness
        0x5301, 0x10,  # sharpness
        0x5302, 0x18,  # sharpness
        0x5303, 0x19,  # sharpness
        0x5304, 0x10,
        0x5305, 0x10,
        0x5306, 0x08,  # denoise
        0x5307, 0x16,
        0x5308, 0x40,
        0x5309, 0x10,  # sharpness
        0x530A, 0x10,  # sharpness
        0x530B, 0x04,  # sharpness
        0x530C, 0x06,  # sharpness
        # GAMMA
        0x5480, 0x01,
        0x5481, 0x00,
        0x5482, 0x1E,
        0x5483, 0x3B,
        0x5484, 0x58,
        0x5485, 0x66,
        0x5486, 0x71,
        0x5487, 0x7D,
        0x5488, 0x83,
        0x5489, 0x8F,
        0x548A, 0x98,
        0x548B, 0xA6,
        0x548C, 0xB8,
        0x548D, 0xCA,
        0x548E, 0xD7,
        0x548F, 0xE3,
        0x5490, 0x1D,
        # Special Digital Effects (SDE) (UV adjust)
        0x5580, 0x06,  # enable brightness and contrast
        0x5583, 0x40,  # special_effect
        0x5584, 0x10,  # special_effect
        0x5586, 0x20,  # contrast
        0x5587, 0x00,  # brightness
        0x5588, 0x00,  # brightness
        0x5589, 0x10,
        0x558A, 0x00,
        0x558B, 0xF8,
        0x501D, 0x40,  # enable manual offset of contrast
        # power on
        0x3008, 0x02,
        # 50Hz
        0x3C00, 0x04,
        #_REG_DLY, 300,
    ]



    _reset_awb = [
        _ISP_CONTROL_01, 0x83,  # turn color matrix, awb and SDE
        # sys reset
        _SYSTEM_RESET00, 0x00, # enable all blocks
        _SYSTEM_RESET02, 0x1C, # reset jfifo, sfifo, jpg, fmux, avg
        # clock enable
        #0x3004, 0xFF,
        #_CLOCK_ENABLE02, 0xC3,
        # isp control
        0x5000, 0xA7,
        _ISP_CONTROL_01, 0xA3,  # +scaling?
        0x5003, 0x08,  # special_effect
        # unknown
        0x370C, 0x02,  #!!IMPORTANT
        0x3634, 0x40,  #!!IMPORTANT
        # AEC/AGC
        0x3A02, 0x03,
        0x3A03, 0xD8,
        0x3A08, 0x01,
        0x3A09, 0x27,
        0x3A0A, 0x00,
        0x3A0B, 0xF6,
        0x3A0D, 0x04,
        0x3A0E, 0x03,
        0x3A0F, 0x30,  # ae_level
        0x3A10, 0x28,  # ae_level
        0x3A11, 0x60,  # ae_level
        0x3A13, 0x43,
        0x3A14, 0x03,
        0x3A15, 0xD8,
        0x3A18, 0x00,  # gainceiling
        0x3A19, 0xF8,  # gainceiling
        0x3A1B, 0x30,  # ae_level
        0x3A1E, 0x26,  # ae_level
        0x3A1F, 0x14,  # ae_level
        # vcm debug
        0x3600, 0x08,
        0x3601, 0x33,
        # 50/60Hz
        0x3C01, 0xA4,
        0x3C04, 0x28,
        0x3C05, 0x98,
        0x3C06, 0x00,
        0x3C07, 0x08,
        0x3C08, 0x00,
        0x3C09, 0x1C,
        0x3C0A, 0x9C,
        0x3C0B, 0x40,
        0x460C, 0x22,  # disable jpeg footer
        # BLC
        0x4001, 0x02,
        0x4004, 0x02,
        # AWB
        0x5180, 0xFF,
        0x5181, 0xF2,
        0x5182, 0x00,
        0x5183, 0x14,
        0x5184, 0x25,
        0x5185, 0x24,
        0x5186, 0x09,
        0x5187, 0x09,
        0x5188, 0x09,
        0x5189, 0x75,
        0x518A, 0x54,
        0x518B, 0xE0,
        0x518C, 0xB2,
        0x518D, 0x42,
        0x518E, 0x3D,
        0x518F, 0x56,
        0x5190, 0x46,
        0x5191, 0xF8,
        0x5192, 0x04,
        0x5193, 0x70,
        0x5194, 0xF0,
        0x5195, 0xF0,
        0x5196, 0x03,
        0x5197, 0x01,
        0x5198, 0x04,
        0x5199, 0x12,
        0x519A, 0x04,
        0x519B, 0x00,
        0x519C, 0x06,
        0x519D, 0x82,
        0x519E, 0x38,
        # color matrix (Saturation)
        0x5381, 0x1E,
        0x5382, 0x5B,
        0x5383, 0x08,
        0x5384, 0x0A,
        0x5385, 0x7E,
        0x5386, 0x88,
        0x5387, 0x7C,
        0x5388, 0x6C,
        0x5389, 0x10,
        0x538A, 0x01,
        0x538B, 0x98,
        # CIP control (Sharpness)
        0x5300, 0x10,  # sharpness
        0x5301, 0x10,  # sharpness
        0x5302, 0x18,  # sharpness
        0x5303, 0x19,  # sharpness
        0x5304, 0x10,
        0x5305, 0x10,
        0x5306, 0x08,  # denoise
        0x5307, 0x16,
        0x5308, 0x40,
        0x5309, 0x10,  # sharpness
        0x530A, 0x10,  # sharpness
        0x530B, 0x04,  # sharpness
        0x530C, 0x06,  # sharpness
        # GAMMA
        0x5480, 0x01,
        0x5481, 0x00,
        0x5482, 0x1E,
        0x5483, 0x3B,
        0x5484, 0x58,
        0x5485, 0x66,
        0x5486, 0x71,
        0x5487, 0x7D,
        0x5488, 0x83,
        0x5489, 0x8F,
        0x548A, 0x98,
        0x548B, 0xA6,
        0x548C, 0xB8,
        0x548D, 0xCA,
        0x548E, 0xD7,
        0x548F, 0xE3,
        0x5490, 0x1D,
        # Special Digital Effects (SDE) (UV adjust)
        0x5580, 0x06,  # enable brightness and contrast
        0x5583, 0x40,  # special_effect
        0x5584, 0x10,  # special_effect
        0x5586, 0x20,  # contrast
        0x5587, 0x00,  # brightness
        0x5588, 0x00,  # brightness
        0x5589, 0x10,
        0x558A, 0x00,
        0x558B, 0xF8,
        0x501D, 0x40,  # enable manual offset of contrast
    ]
    _sensor_format_jpeg = [
        _FORMAT_CTRL, 0x00,  # YUV422
        _FORMAT_CTRL00, 0x30,  # YUYV
        _SYSTEM_RESET02, 0x00,  # enable everything
        _CLOCK_ENABLE02, 0xFF,  # enable all clocks
        0x471C, 0x50,  # 0xd0 to 0x50 !!!
    ]

    _sensor_format_raw = [
        _FORMAT_CTRL, 0x03,  # RAW (DPC)
        _FORMAT_CTRL00, 0x00,  # RAW
    ]

    _sensor_format_grayscale = [
        _FORMAT_CTRL, 0x00,  # YUV422
        _FORMAT_CTRL00, 0x10,  # Y8
    ]

    _sensor_format_yuv422 = [
        _FORMAT_CTRL, 0x00,  # YUV422
        _FORMAT_CTRL00, 0x30,  # YUYV
    ]

    _sensor_format_rgb565 = [
        _FORMAT_CTRL, 0x01,  # RGB
        _FORMAT_CTRL00, 0x61,  # RGB565 (BGR)
        _SYSTEM_RESET02, 0x1C, # reset jfifo, sfifo, jpg, fmux, avg
        _CLOCK_ENABLE02, 0xC3, # reset to how it was before (no jpg clock)

    ]

    _ov5640_color_settings = {
        OV5640_COLOR_RGB: _sensor_format_rgb565,
        OV5640_COLOR_YUV: _sensor_format_yuv422,
        OV5640_COLOR_GRAYSCALE: _sensor_format_grayscale,
        OV5640_COLOR_JPEG: _sensor_format_jpeg,
    }

    _contrast_settings = [
        [0x20, 0x00], #  0
        [0x24, 0x10], # +1
        [0x28, 0x18], # +2
        [0x2c, 0x1c], # +3
        [0x14, 0x14], # -3
        [0x18, 0x18], # -2
        [0x1c, 0x1c], # -1
    ]

    _sensor_saturation_levels = [
        [0x1D, 0x60, 0x03, 0x0C, 0x78, 0x84, 0x7D, 0x6B, 0x12, 0x01, 0x98],  # 0
        [0x1D, 0x60, 0x03, 0x0D, 0x84, 0x91, 0x8A, 0x76, 0x14, 0x01, 0x98],  # +1
        [0x1D, 0x60, 0x03, 0x0E, 0x90, 0x9E, 0x96, 0x80, 0x16, 0x01, 0x98],  # +2
        [0x1D, 0x60, 0x03, 0x10, 0x9C, 0xAC, 0xA2, 0x8B, 0x17, 0x01, 0x98],  # +3
        [0x1D, 0x60, 0x03, 0x11, 0xA8, 0xB9, 0xAF, 0x96, 0x19, 0x01, 0x98],  # +4
        [0x1D, 0x60, 0x03, 0x07, 0x48, 0x4F, 0x4B, 0x40, 0x0B, 0x01, 0x98],  # -4
        [0x1D, 0x60, 0x03, 0x08, 0x54, 0x5C, 0x58, 0x4B, 0x0D, 0x01, 0x98],  # -3
        [0x1D, 0x60, 0x03, 0x0A, 0x60, 0x6A, 0x64, 0x56, 0x0E, 0x01, 0x98],  # -2
        [0x1D, 0x60, 0x03, 0x0B, 0x6C, 0x77, 0x70, 0x60, 0x10, 0x01, 0x98],  # -1
    ]

    _sensor_ev_levels = [
        [0x38, 0x30, 0x61, 0x38, 0x30, 0x10], #  0
        [0x40, 0x38, 0x71, 0x40, 0x38, 0x10], # +1
        [0x50, 0x48, 0x90, 0x50, 0x48, 0x20], # +2
        [0x60, 0x58, 0xa0, 0x60, 0x58, 0x20], # +3
        [0x10, 0x08, 0x10, 0x08, 0x20, 0x10], # -3
        [0x20, 0x18, 0x41, 0x20, 0x18, 0x10], # -2
        [0x30, 0x28, 0x61, 0x30, 0x28, 0x10], # -1
    ]

    OV5640_WHITE_BALANCE_AUTO = 0
    OV5640_WHITE_BALANCE_SUNNY = 1
    OV5640_WHITE_BALANCE_FLUORESCENT = 2
    OV5640_WHITE_BALANCE_CLOUDY = 3
    OV5640_WHITE_BALANCE_INCANDESCENT = 4

    _light_registers = [0x3406, 0x3400, 0x3401, 0x3402, 0x3403, 0x3404, 0x3405]
    _light_modes = [
        [0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00], # auto
        [0x01, 0x06, 0x1c, 0x04, 0x00, 0x04, 0xf3], # sunny
        [0x01, 0x05, 0x48, 0x04, 0x00, 0x07, 0xcf], # office / fluorescent
        [0x01, 0x06, 0x48, 0x04, 0x00, 0x04, 0xd3], # cloudy
        [0x01, 0x04, 0x10, 0x04, 0x00, 0x08, 0x40], # home / incandescent

    ]

    OV5640_SPECIAL_EFFECT_NONE = 0
    OV5640_SPECIAL_EFFECT_NEGATIVE = 1
    OV5640_SPECIAL_EFFECT_GRAYSCALE = 2
    OV5640_SPECIAL_EFFECT_RED_TINT = 3
    OV5640_SPECIAL_EFFECT_GREEN_TINT = 4
    OV5640_SPECIAL_EFFECT_BLUE_TINT = 5
    OV5640_SPECIAL_EFFECT_SEPIA = 6

    _sensor_special_effects = [
        [0x06, 0x40, 0x10, 0x08],  # Normal
        [0x46, 0x40, 0x28, 0x08],  # Negative
        [0x1E, 0x80, 0x80, 0x08],  # Grayscale
        [0x1E, 0x80, 0xC0, 0x08],  # Red Tint
        [0x1E, 0x60, 0x60, 0x08],  # Green Tint
        [0x1E, 0xA0, 0x40, 0x08],  # Blue Tint
        [0x1E, 0x40, 0xA0, 0x08],  # Sepia
    ]

    _sensor_regs_gamma0 = [
        0x5480, 0x01,
        0x5481, 0x08,
        0x5482, 0x14,
        0x5483, 0x28,
        0x5484, 0x51,
        0x5485, 0x65,
        0x5486, 0x71,
        0x5487, 0x7D,
        0x5488, 0x87,
        0x5489, 0x91,
        0x548A, 0x9A,
        0x548B, 0xAA,
        0x548C, 0xB8,
        0x548D, 0xCD,
        0x548E, 0xDD,
        0x548F, 0xEA,
        0x5490, 0x1D,
    ]

    sensor_regs_gamma1 = [
        0x5480, 0x1,
        0x5481, 0x0,
        0x5482, 0x1E,
        0x5483, 0x3B,
        0x5484, 0x58,
        0x5485, 0x66,
        0x5486, 0x71,
        0x5487, 0x7D,
        0x5488, 0x83,
        0x5489, 0x8F,
        0x548A, 0x98,
        0x548B, 0xA6,
        0x548C, 0xB8,
        0x548D, 0xCA,
        0x548E, 0xD7,
        0x548F, 0xE3,
        0x5490, 0x1D,
    ]

    sensor_regs_awb0 = [
        0x5180, 0xFF,
        0x5181, 0xF2,
        0x5182, 0x00,
        0x5183, 0x14,
        0x5184, 0x25,
        0x5185, 0x24,
        0x5186, 0x09,
        0x5187, 0x09,
        0x5188, 0x09,
        0x5189, 0x75,
        0x518A, 0x54,
        0x518B, 0xE0,
        0x518C, 0xB2,
        0x518D, 0x42,
        0x518E, 0x3D,
        0x518F, 0x56,
        0x5190, 0x46,
        0x5191, 0xF8,
        0x5192, 0x04,
        0x5193, 0x70,
        0x5194, 0xF0,
        0x5195, 0xF0,
        0x5196, 0x03,
        0x5197, 0x01,
        0x5198, 0x04,
        0x5199, 0x12,
        0x519A, 0x04,
        0x519B, 0x00,
        0x519C, 0x06,
        0x519D, 0x82,
        0x519E, 0x38,
    ]
    # fmt: on

    def __init__(
        self,
        i2c,
        i2c_address = 0x3C,
        num_data_pins = 1
    ):
        super().__init__(i2c, i2c_address, (240, 320, 2))

        # self.soft_reset()
        # sleep_us(1_000_000)
        self.write_list(self._sensor_default_regs)

        self._colorspace = self.OV5640_COLOR_RGB
        self._flip_x = False
        self._flip_y = False
        self._w = None
        self._h = None
        self._size = self.OV5640_SIZE_QVGA
        self._test_pattern = False
        self._binning = False
        self._scale = False
        self._ev = 0
        self._white_balance = 0

        self._set_size_and_colorspace()
    
    def is_connected(self):
        try:
            # Try to read the chip ID
            # If it throws an I/O error - the device isn't connected
            id = self.getChipID()
            
            # Confirm the chip ID is correct
            if id == 0x5640:
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
        data = self.readRegister(self._CHIP_ID_HIGH, 2)
        return (data[0] << 8) | data[1]

    def soft_reset(self):
        """
        Performs a software reset of the HM01B0 sensor.
        This resets the sensor to its default state.
        """
        self.writeRegister(self._SYSTEM_CTROL0, 0x82)

    # def setMode(self, mode):
    #     """
    #     Sets the operating mode of the HM01B0 sensor.
    #     Args:
    #         mode (int): The mode to set, e.g., MODE_STREAMING.
    #     """
    #     self.writeRegister(self.MODE_SELECT, mode)

    # def trigger(self):
    #     self.writeRegister(self.MODE_SELECT, self.HIMAX_MODE_STREAMING_NFRAMES)

    # def set_n_frames(self, n_frames):
    #     self.writeRegister(self.PMU_AUTOSLEEP_FRAMECNT, n_frames)

    def write_list(self, data):
        """
        Initializes the HM01B0 sensor with default settings.
        This includes setting up exposure, gain, and frame timing.
        """
        for i in range(len(data) // 2):
            reg = data[i * 2]
            value = data[i * 2 + 1]
            print(i, reg, value)
            if reg == self._REG_DLY:
                sleep_us(value)
            else:
                self.writeRegister(reg, value)
                sleep_us(1000)
 
    def _set_size_and_colorspace(self) -> None:
        size = self._size
        width, height, ratio = self._resolution_info[size]
        self._w = width
        self._h = height
        (
            max_width,
            max_height,
            start_x,
            start_y,
            end_x,
            end_y,
            offset_x,
            offset_y,
            total_x,
            total_y,
        ) = self._ratio_table[ratio]

        self._binning = (width <= max_width // 2) and (height <= max_height // 2)
        self._scale = not (
            (width == max_width and height == max_height)
            or (width == max_width // 2 and height == max_height // 2)
        )

        self._write_addr_reg(self._X_ADDR_ST_H, start_x, start_y)
        self._write_addr_reg(self._X_ADDR_END_H, end_x, end_y)
        self._write_addr_reg(self._X_OUTPUT_SIZE_H, width, height)

        if not self._binning:
            self._write_addr_reg(self._X_TOTAL_SIZE_H, total_x, total_y)
            self._write_addr_reg(self._X_OFFSET_H, offset_x, offset_y)
        else:
            if width > 920:
                self._write_addr_reg(self._X_TOTAL_SIZE_H, total_x - 200, total_y // 2)
            else:
                self._write_addr_reg(self._X_TOTAL_SIZE_H, 2060, total_y // 2)
            self._write_addr_reg(self._X_OFFSET_H, offset_x // 2, offset_y // 2)

        self._write_reg_bits(self._ISP_CONTROL_01, 0x20, self._scale)

        self._set_image_options()

        # if self._colorspace == self.OV5640_COLOR_JPEG:
        #     sys_mul = 200
        #     if size < self.OV5640_SIZE_QVGA:
        #         sys_mul = 160
        #     if size < self.OV5640_SIZE_XGA:
        #         sys_mul = 180
        #     self._set_pll(False, sys_mul, 4, 2, False, 2, True, 4)
        # else:
        #     self._set_pll(False, 32, 1, 1, False, 1, True, 4)
        self._set_pll(False, 32, 1, 1, False, 1, True, 4)

        self._set_colorspace()

    def _set_pll(
        self,
        bypass: bool,
        multiplier: int,
        sys_div: int,
        pre_div: int,
        root_2x: bool,
        pclk_root_div: int,
        pclk_manual: bool,
        pclk_div: int,
    ) -> None:
        if (
            multiplier > 252
            or multiplier < 4
            or sys_div > 15
            or pre_div > 8
            or pclk_div > 31
            or pclk_root_div > 3
        ):
            raise ValueError("Invalid argument to internal function")

        self.writeRegister(0x3039, 0x80 if bypass else 0)
        self.writeRegister(0x3034, 0x1A)
        self.writeRegister(0x3035, 1 | ((sys_div & 0xF) << 4))
        self.writeRegister(0x3036, multiplier & 0xFF)
        self.writeRegister(0x3037, (pre_div & 0xF) | (0x10 if root_2x else 0))
        self.writeRegister(0x3108, (pclk_root_div & 3) << 4 | 0x06)
        self.writeRegister(0x3824, pclk_div & 0x1F)
        self.writeRegister(0x460C, 0x22 if pclk_manual else 0x22)
        self.writeRegister(0x3103, 0x13)

    def _set_colorspace(self) -> None:
        colorspace = self._colorspace
        settings = self._ov5640_color_settings[colorspace]

        self.write_list(settings)

    def _set_image_options(self) -> None:
        reg20 = reg21 = reg4514 = reg4514_test = 0
        if self._colorspace == self.OV5640_COLOR_JPEG:
            reg21 |= 0x20

        if self._binning:
            reg20 |= 1
            reg21 |= 1
            reg4514_test |= 4
        else:
            reg20 |= 0x40

        if self._flip_y:
            reg20 |= 0x06
            reg4514_test |= 1

        if self._flip_x:
            reg21 |= 0x06
            reg4514_test |= 2

        if reg4514_test == 0:
            reg4514 = 0x88
        elif reg4514_test == 1:
            reg4514 = 0x00
        elif reg4514_test == 2:
            reg4514 = 0xBB
        elif reg4514_test == 3:
            reg4514 = 0x00
        elif reg4514_test == 4:
            reg4514 = 0xAA
        elif reg4514_test == 5:
            reg4514 = 0xBB
        elif reg4514_test == 6:
            reg4514 = 0xBB
        elif reg4514_test == 7:
            reg4514 = 0xAA

        self.writeRegister(self._TIMING_TC_REG20, reg20)
        self.writeRegister(self._TIMING_TC_REG21, reg21)
        self.writeRegister(0x4514, reg4514)

        if self._binning:
            self.writeRegister(0x4520, 0x0B)
            self.writeRegister(self._X_INCREMENT, 0x31)
            self.writeRegister(self._Y_INCREMENT, 0x31)
        else:
            self.writeRegister(0x4520, 0x10)
            self.writeRegister(self._X_INCREMENT, 0x11)
            self.writeRegister(self._Y_INCREMENT, 0x11)

    def _write_addr_reg(self, reg: int, x_value: int, y_value: int) -> None:
        self.writeRegister(reg, [
            (x_value >> 8) & 0xFF,
            x_value & 0xFF,
            (y_value >> 8) & 0xFF,
            y_value & 0xFF,
        ])

    def _write_reg_bits(self, reg: int, mask: int, enable: bool) -> None:
        val = self.readRegister(reg)[0]
        if enable:
            val |= mask
        else:
            val &= ~mask
        self.writeRegister(reg, val)

    def read(self, image = None):
        """
        Reads a frame from the camera.
        Returns:
            tuple: (success, frame)
        """
        return (True, cv2.cvtColor(self.buffer, cv2.COLOR_BayerRG2BGR, image))

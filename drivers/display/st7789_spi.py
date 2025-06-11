# Modified from:
# https://github.com/easytarget/st7789-framebuffer/blob/main/st7789_purefb.py

import struct
from time import sleep_ms
from machine import Pin, SPI
from ulab import numpy as np
import cv2

# ST7789 commands
_ST7789_SWRESET = b"\x01"
_ST7789_SLPIN = b"\x10"
_ST7789_SLPOUT = b"\x11"
_ST7789_NORON = b"\x13"
_ST7789_INVOFF = b"\x20"
_ST7789_INVON = b"\x21"
_ST7789_DISPOFF = b"\x28"
_ST7789_DISPON = b"\x29"
_ST7789_CASET = b"\x2a"
_ST7789_RASET = b"\x2b"
_ST7789_RAMWR = b"\x2c"
_ST7789_VSCRDEF = b"\x33"
_ST7789_COLMOD = b"\x3a"
_ST7789_MADCTL = b"\x36"
_ST7789_VSCSAD = b"\x37"
_ST7789_RAMCTL = b"\xb0"

# MADCTL bits
_ST7789_MADCTL_MY = const(0x80)
_ST7789_MADCTL_MX = const(0x40)
_ST7789_MADCTL_MV = const(0x20)
_ST7789_MADCTL_ML = const(0x10)
_ST7789_MADCTL_BGR = const(0x08)
_ST7789_MADCTL_MH = const(0x04)
_ST7789_MADCTL_RGB = const(0x00)

RGB = 0x00
BGR = 0x08

# 8 basic color definitions
BLACK = const(0x0000)
BLUE = const(0x001F)
RED = const(0xF800)
GREEN = const(0x07E0)
CYAN = const(0x07FF)
MAGENTA = const(0xF81F)
YELLOW = const(0xFFE0)
WHITE = const(0xFFFF)

_ENCODE_POS = const(">HH")

# Rotation tables
#   (madctl, width, height, xstart, ystart)[rotation % 4]

_DISPLAY_240x320 = (
    (0x00, 240, 320, 0, 0),
    (0x60, 320, 240, 0, 0),
    (0xc0, 240, 320, 0, 0),
    (0xa0, 320, 240, 0, 0))

_DISPLAY_170x320 = (
    (0x00, 170, 320, 35, 0),
    (0x60, 320, 170, 0, 35),
    (0xc0, 170, 320, 35, 0),
    (0xa0, 320, 170, 0, 35))

_DISPLAY_240x240 = (
    (0x00, 240, 240,  0,  0),
    (0x60, 240, 240,  0,  0),
    (0xc0, 240, 240,  0, 80),
    (0xa0, 240, 240, 80,  0))

_DISPLAY_135x240 = (
    (0x00, 135, 240, 52, 40),
    (0x60, 240, 135, 40, 53),
    (0xc0, 135, 240, 53, 40),
    (0xa0, 240, 135, 40, 52))

_DISPLAY_128x128 = (
    (0x00, 128, 128, 2, 1),
    (0x60, 128, 128, 1, 2),
    (0xc0, 128, 128, 2, 1),
    (0xa0, 128, 128, 1, 2))

# Supported displays (physical width, physical height, rotation table)
_SUPPORTED_DISPLAYS = (
    (240, 320, _DISPLAY_240x320),
    (170, 320, _DISPLAY_170x320),
    (240, 240, _DISPLAY_240x240),
    (135, 240, _DISPLAY_135x240),
    (128, 128, _DISPLAY_128x128))

# init tuple format (b'command', b'data', delay_ms)
_ST7789_INIT_CMDS = (
    ( b'\x11', b'\x00', 120),               # Exit sleep mode
    ( b'\x13', b'\x00', 0),                 # Turn on the display
    ( b'\xb6', b'\x0a\x82', 0),             # Set display function control
    ( b'\x3a', b'\x55', 10),                # Set pixel format to 16 bits per pixel (RGB565)
    ( b'\xb2', b'\x0c\x0c\x00\x33\x33', 0), # Set porch control
    ( b'\xb7', b'\x35', 0),                 # Set gate control
    ( b'\xbb', b'\x28', 0),                 # Set VCOMS setting
    ( b'\xc0', b'\x0c', 0),                 # Set power control 1
    ( b'\xc2', b'\x01\xff', 0),             # Set power control 2
    ( b'\xc3', b'\x10', 0),                 # Set power control 3
    ( b'\xc4', b'\x20', 0),                 # Set power control 4
    ( b'\xc6', b'\x0f', 0),                 # Set VCOM control 1
    ( b'\xd0', b'\xa4\xa1', 0),             # Set power control A
                                            # Set gamma curve positive polarity
    ( b'\xe0', b'\xd0\x00\x02\x07\x0a\x28\x32\x44\x42\x06\x0e\x12\x14\x17', 0),
                                            # Set gamma curve negative polarity
    ( b'\xe1', b'\xd0\x00\x02\x07\x0a\x28\x31\x54\x47\x0e\x1c\x17\x1b\x1e', 0),
    ( b'\x21', b'\x00', 0),                 # Enable display inversion
    ( b'\x29', b'\x00', 120)                # Turn on the display
)

class ST7789_SPI():
    """
    OpenCV SPI driver for ST7789 displays

    Args:
        width (int): display width **Required**
        height (int): display height **Required**
        spi (SPI): SPI bus **Required**
        pin_dc (Pin): Data/Command pin number **Required**
        pin_cs (Pin): Chip Select pin number
        rotation (int): Orientation of display
          - 0-Portrait, default
          - 1-Landscape
          - 2-Inverted Portrait
          - 3-Inverted Landscape
        color_order (int):
          - RGB: Red, Green Blue, default
          - BGR: Blue, Green, Red
        reverse_bytes_in_word (bool):
          - Enable if the display uses LSB byte order for color words
    """
    def __init__(
        self,
        width,
        height,
        spi,
        pin_dc,
        pin_cs=None,
        rotation=0,
        color_order=BGR,
        reverse_bytes_in_word=True,
    ):
        # Store SPI arguments
        self.spi = spi
        self.dc = Pin(pin_dc) # Don't change mode/alt
        self.cs = Pin(pin_cs, Pin.OUT, value=1) if pin_cs else None
        # Initial dimensions and offsets; will be overridden when rotation applied
        self.width = width
        self.height = height
        self.xstart = 0
        self.ystart = 0
        # Check display is known and get rotation table
        self.rotations = self._find_rotations(width, height)
        if not self.rotations:
            supported_displays = ", ".join(
                [f"{display[0]}x{display[1]}" for display in _SUPPORTED_DISPLAYS])
            raise ValueError(
                f"Unsupported {width}x{height} display. Supported displays: {supported_displays}")
        # Colors
        self.color_order = color_order
        self.needs_swap = reverse_bytes_in_word
        # Reset the display
        self.soft_reset()
        # Yes, send init twice, once is not always enough
        self.send_init(_ST7789_INIT_CMDS)
        self.send_init(_ST7789_INIT_CMDS)
        # Initial rotation
        self._rotation = rotation % 4
        # Apply rotation
        self.rotation(self._rotation)
        # Create the framebuffer for the correct rotation
        self.buffer = np.zeros((self.height, self.width, 2), dtype=np.uint8)

    def send_init(self, commands):
        """
        Send initialisation commands to display.
        """
        for command, data, delay in commands:
            self._write(command, data)
            sleep_ms(delay)

    def soft_reset(self):
        """
        Soft reset display.
        """
        self._write(_ST7789_SWRESET)
        sleep_ms(150)

    def _find_rotations(self, width, height):
        """ Find the correct rotation for our display or return None """
        for display in _SUPPORTED_DISPLAYS:
            if display[0] == width and display[1] == height:
                return display[2]
        return None

    def rotation(self, rotation):
        """
        Set display rotation.

        Args:
            rotation (int):
                - 0-Portrait
                - 1-Landscape
                - 2-Inverted Portrait
                - 3-Inverted Landscape
        """
        if ((rotation % 2) != (self._rotation % 2)) and (self.width != self.height):
            # non-square displays can currently only be rotated by 180 degrees
            # TODO: can framebuffer of super class be destroyed and re-created
            #       to match the new dimensions? or it's width/height changed?
            return

        # find rotation parameters and send command
        rotation %= len(self.rotations)
        (   madctl,
            self.width,
            self.height,
            self.xstart,
            self.ystart, ) = self.rotations[rotation]
        if self.color_order == BGR:
            madctl |= _ST7789_MADCTL_BGR
        else:
            madctl &= ~_ST7789_MADCTL_BGR
        self._write(_ST7789_MADCTL, bytes([madctl]))
        # Set window for writing into
        self._write(_ST7789_CASET,
            struct.pack(_ENCODE_POS, self.xstart, self.width + self.xstart - 1))
        self._write(_ST7789_RASET,
            struct.pack(_ENCODE_POS, self.ystart, self.height + self.ystart - 1))
        self._write(_ST7789_RAMWR)
        # TODO: Can we swap (modify) framebuffer width/height in the super() class?
        self._rotation = rotation

    def _get_common_roi_with_buffer(self, image):
        """
        Get the common region of interest (ROI) between the image and the 
        display's internal buffer.

        Args:
            image (ndarray): Image to display
        
        Returns:
            tuple: (image_roi, buffer_roi)
        """
        # Ensure image is a NumPy ndarray
        if type(image) is not np.ndarray:
            raise TypeError("Image must be a NumPy ndarray")
        
        # Determing number of rows and columns in the image
        image_rows = image.shape[0]
        if image.ndim < 2:
            image_cols = 1
        else:
            image_cols = image.shape[1]
        
        # Get the common ROI between the image and the buffer
        row_max = min(image_rows, self.height)
        col_max = min(image_cols, self.width)
        img_roi = image[:row_max, :col_max]
        buffer_roi = self.buffer[:row_max, :col_max]
        return img_roi, buffer_roi

    def _convert_image_to_uint8(self, image):
        """
        Convert the image to uint8 format if necessary.

        Args:
            image (ndarray): Image to convert

        Returns:
            Image: Converted image
        """
        # Check if the image is already in uint8 format
        if image.dtype is np.uint8:
            return image
        
        # Convert to uint8 format. This unfortunately requires creating a new
        # buffer for the converted image, which takes more memory
        if image.dtype == np.int8:
            return cv2.convertScaleAbs(image, alpha=1, beta=127)
        elif image.dtype == np.int16:
            return cv2.convertScaleAbs(image, alpha=1/255, beta=127)
        elif image.dtype == np.uint16:
            return cv2.convertScaleAbs(image, alpha=1/255)
        elif image.dtype == np.float:
            # This implementation creates an additional buffer from np.clip()
            # TODO: Find another solution that avoids an additional buffer
            return cv2.convertScaleAbs(np.clip(image, 0, 1), alpha=255)
        else:
            raise ValueError(f"Unsupported image dtype: {image.dtype}")

    def _write_image_to_buffer_bgr565(self, image_roi, buffer_roi):
        """
        Convert the image ROI to BGR565 format and write it to the buffer ROI.

        Args:
            image_roi (ndarray): Image region of interest
            buffer_roi (ndarray): Buffer region of interest
        """
        # Determine the number of channels in the image
        if image_roi.ndim < 3:
            ch = 1
        else:
            ch = image_roi.shape[2]

        if ch == 1: # Grayscale
            buffer_roi = cv2.cvtColor(image_roi, cv2.COLOR_GRAY2BGR565, buffer_roi)
        elif ch == 2: # Already in BGR565 format
            buffer_roi[:] = image_roi
        elif ch == 3: # BGR
            buffer_roi = cv2.cvtColor(image_roi, cv2.COLOR_BGR2BGR565, buffer_roi)
        else:
            raise ValueError("Image must be 1, 2 or 3 channels (grayscale, BGR565, or BGR)")

    def imshow(self, image):
        """
        Display a NumPy image on the screen.

        Args:
            image (ndarray): Image to display
        """
        # Get the common ROI between the image and internal display buffer
        image_roi, buffer_roi = self._get_common_roi_with_buffer(image)

        # Ensure the image is in uint8 format
        image_roi = self._convert_image_to_uint8(image_roi)

        # Convert the image to BGR565 format and write it to the buffer
        self._write_image_to_buffer_bgr565(image_roi, buffer_roi)

        # Write buffer to display. Swap bytes if needed
        if self.needs_swap:
            self._write(None, self.buffer[:, :, ::-1])
        else:
            self._write(None, self.buffer)

    def clear(self):
        """
        Clear the display by filling it with black color.
        """
        # Clear the buffer by filling it with zeros (black)
        self.buffer[:] = 0
        # Write the buffer to the display
        self._write(None, self.buffer)

    def saveDcPin(self):
        """
        Saves the current `mode` and `alt` of the DC pin so it can be restored
        later. Mostly used to restore the SPI mode (MISO) of the DC pin after
        communication with the display in case another device is using the same
        SPI bus.

        Returns:
            tuple: (mode, alt)
        """
        # There's no way to get the mode and alt of a pin directly, so we
        # convert the pin to a string and parse it. Example format:
        # "Pin(GPIO16, mode=ALT, alt=SPI)"
        dcStr = str(self.dc)

        # Extract the "mode" parameter from the pin string
        if "mode=" in dcStr:
            # Split between "mode=" and the next comma or closing parenthesis
            modeStr = dcStr.split("mode=")[1].split(",")[0].split(")")[0]

            # Look up the mode in Pin class dictionary
            mode = Pin.__dict__[modeStr]
        else:
            # No mode specified, just set to None
            mode = None

        # Extrct the "alt" parameter from the pin string
        if "alt=" in dcStr:
            # Split between "alt=" and the next comma or closing parenthesis
            altStr = dcStr.split("alt=")[1].split(",")[0].split(")")[0]

            # Look up the alt in Pin class dictionary (with "ALT_" prefix)
            alt = Pin.__dict__["ALT_" + altStr]
        else:
            # No alt specified, just set to None
            alt = None

        # Return the mode and alt as a tuple
        return (mode, alt)

    def _write(self, command=None, data=None):
        """SPI write to the device: commands and data."""
        # Save the current mode and alt of the DC pin in case it's used by
        # another device on the same SPI bus
        mode, alt = self.saveDcPin()
        # Temporarily set the DC pin to output mode
        self.dc.init(mode=Pin.OUT)

        if self.cs:
            self.cs.off()
        if command is not None:
            self.dc.off()
            self.spi.write(command)
        if data is not None:
            self.dc.on()
            self.spi.write(data)
        if self.cs:
            self.cs.on()

        # Restore the DC pin to its original mode and alt
        self.dc.init(mode=mode, alt=alt)

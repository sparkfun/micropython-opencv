from .st7789 import ST7789
from machine import Pin

# Derived from:
# https://github.com/easytarget/st7789-framebuffer/blob/main/st7789_purefb.py
class ST7789_SPI(ST7789):
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
        color_order=ST7789.BGR,
        reverse_bytes_in_word=True,
    ):
        # Store SPI arguments
        self.spi = spi
        self.dc = Pin(pin_dc) # Don't change mode/alt
        self.cs = Pin(pin_cs, Pin.OUT, value=1) if pin_cs else None
        
        super().__init__(width, height, rotation, color_order, reverse_bytes_in_word)

    def _write(self, command=None, data=None):
        """SPI write to the device: commands and data."""
        # Save the current mode and alt of the DC pin in case it's used by
        # another device on the same SPI bus
        dcMode, dcAlt = self.savePinModeAlt(self.dc)

        # Temporarily set the DC pin to output mode
        self.dc.init(mode=Pin.OUT)

        # Write to the display
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
        self.dc.init(mode=dcMode, alt=dcAlt)

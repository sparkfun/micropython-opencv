from .st7789 import ST7789
from machine import Pin

# This class is derived from:
# https://github.com/easytarget/st7789-framebuffer/blob/main/st7789_purefb.py
# Released under the MIT license.
# Copyright (c) 2024 Owen Carter
# Copyright (c) 2024 Ethan Lacasse
# Copyright (c) 2020-2023 Russ Hughes
# Copyright (c) 2019 Ivan Belokobylskiy
class ST7789_SPI(ST7789):
    """
    OpenCV ST7789 display driver using a SPI interface.
    """
    def __init__(
        self,
        width,
        height,
        spi,
        pin_dc,
        pin_cs=None,
        rotation=0,
        bgr_order=True,
        reverse_bytes_in_word=True,
    ):
        """
        Initializes the ST7789 SPI display driver.

        Args:
            width (int): Display width in pixels
            height (int): Display height in pixels
            spi (SPI): SPI bus object
            pin_dc (int): Data/Command pin number
            pin_cs (int, optional): Chip Select pin number
            rotation (int, optional): Orientation of display
              - 0: Portrait (default)
              - 1: Landscape
              - 2: Inverted portrait
              - 3: Inverted landscape
            bgr_order (bool, optional): Color order
              - True: BGR (default)
              - False: RGB
            reverse_bytes_in_word (bool, optional):
              - Enable if the display uses LSB byte order for color words
        """
        # Store SPI arguments
        self._spi = spi
        self._dc = Pin(pin_dc) # Don't change mode/alt
        self._cs = Pin(pin_cs, Pin.OUT, value=1) if pin_cs else None

        super().__init__(width, height, rotation, bgr_order, reverse_bytes_in_word)

    def _write(self, command=None, data=None):
        """SPI write to the device: commands and data."""
        # Save the current mode and alt of the DC pin in case it's used by
        # another device on the same SPI bus
        dcMode, dcAlt = self._save_pin_mode_alt(self._dc)

        # Temporarily set the DC pin to output mode
        self._dc.init(mode=Pin.OUT)

        # Write to the display
        if self._cs:
            self._cs.off()
        if command is not None:
            self._dc.off()
            self._spi.write(command)
        if data is not None:
            self._dc.on()
            self._spi.write(data)
        if self._cs:
            self._cs.on()

        # Restore the DC pin to its original mode and alt
        self._dc.init(mode=dcMode, alt=dcAlt)

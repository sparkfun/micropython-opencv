#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# st7789_pio.py
#
# OpenCV ST7789 display driver using a PIO interface. Only available on 
# Raspberry Pi RP2 processors.
# 
# This class is derived from:
# https://github.com/raspberrypi/pico-examples/tree/master/pio/st7789_lcd
# Released under the MIT license.
# Copyright (c) 2024 Owen Carter
# Copyright (c) 2024 Ethan Lacasse
# Copyright (c) 2020-2023 Russ Hughes
# Copyright (c) 2019 Ivan Belokobylskiy
#-------------------------------------------------------------------------------

from .st7789 import ST7789
from machine import Pin
import rp2

class ST7789_PIO(ST7789):
    """
    OpenCV ST7789 display driver using a PIO interface. Only available on
    Raspberry Pi RP2 processors.
    """
    def __init__(
        self,
        width,
        height,
        sm_id,
        pin_clk,
        pin_tx,
        pin_dc,
        pin_cs=None,
        freq=-1,
        rotation=0,
        bgr_order=True,
        reverse_bytes_in_word=True,
    ):
        """
        Initializes the ST7789 PIO display driver.

        Args:
            width (int): Display width in pixels
            height (int): Display height in pixels
            sm_id (int): PIO state machine ID
            pin_clk (int): Clock pin number
            pin_tx (int): Data pin number
            pin_dc (int): Data/Command pin number
            pin_cs (int, optional): Chip Select pin number
            freq (int, optional): Frequency in Hz for the PIO state machine
                Default is -1, which uses the default frequency of 125MHz
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
        # Store PIO arguments
        self._sm_id = sm_id
        self._clk = Pin(pin_clk) # Don't change mode/alt
        self._tx = Pin(pin_tx) # Don't change mode/alt
        self._dc = Pin(pin_dc) # Don't change mode/alt
        self._cs = Pin(pin_cs, Pin.OUT, value=1) if pin_cs else None
        self._freq = freq

        # Start the PIO state machine and DMA with 1 bytes per transfer
        self._setup_sm_and_dma(1)

        # Call the parent class constructor
        super().__init__(width, height, rotation, bgr_order, reverse_bytes_in_word)

        # Change the transfer size to 2 bytes for faster throughput. Can't do 4
        # bytes, because then pairs of pixels get swapped
        self._setup_sm_and_dma(2)

    def _setup_sm_and_dma(self, bytes_per_transfer):
        """
        Sets up the PIO state machine and DMA for writing to the display.

        Args:
            bytes_per_transfer (int): Number of bytes to transfer in each write
        """
        # Store the bytes per transfer for later use
        self._bytes_per_transfer = bytes_per_transfer

        # Get the current mode and alt of the pins so they can be restored
        txMode, txAlt = self._save_pin_mode_alt(self._tx)
        clkMode, clkAlt = self._save_pin_mode_alt(self._clk)

        # Initialize the PIO state machine
        self._sm = rp2.StateMachine(
            self._sm_id,
            self._pio_write_spi,
            freq = self._freq,
            out_base = self._tx,
            sideset_base = self._clk,
            pull_thresh = bytes_per_transfer * 8
        )

        # The tx and clk pins just got their mode and alt set for PIO0 or PIO1.
        # We need to save them again to restore later when _write() is called,
        # if we haven't already
        if not hasattr(self, 'txMode'):
            self._txMode, self._txAlt = self._save_pin_mode_alt(self._tx)
            self._clkMode, self._clkAlt = self._save_pin_mode_alt(self._clk)

        # Now restore the original mode and alt of the pins
        self._tx.init(mode=txMode, alt=txAlt)
        self._clk.init(mode=clkMode, alt=clkAlt)

        # Instantiate a DMA controller if not already done
        if not hasattr(self, 'dma'):
            self._dma = rp2.DMA()

        # Configure up DMA to write to the PIO state machine
        req_num = ((self._sm_id // 4) << 3) + (self._sm_id % 4)
        dma_ctrl = self._dma.pack_ctrl(
            size = {1:0, 2:1, 4:2}[bytes_per_transfer], # 0 = 8-bit, 1 = 16-bit, 2 = 32-bit
            inc_write = False,
            treq_sel = req_num,
            bswap = False
        )
        self._dma.config(
            write = self._sm,
            ctrl = dma_ctrl
        )

    def _write(self, command=None, data=None):
        """
        Writes commands and data to the display.

        Args:
            command (bytes, optional): Command to send to the display
            data (bytes, optional): Data to send to the display
        """
        # Save the current mode and alt of the spi pins in case they're used by
        # another device on the same SPI bus
        dcMode, dcAlt = self._save_pin_mode_alt(self._dc)
        txMode, txAlt = self._save_pin_mode_alt(self._tx)
        clkMode, clkAlt = self._save_pin_mode_alt(self._clk)

        # Temporarily set the SPI pins to the correct mode and alt for PIO
        self._dc.init(mode=Pin.OUT)
        self._tx.init(mode=self._txMode, alt=self._txAlt)
        self._clk.init(mode=self._clkMode, alt=self._clkAlt)

        # Write to the display
        if self._cs:
            self._cs.off()
        if command is not None:
            self._dc.off()
            self._pio_write(command)
        if data is not None:
            self._dc.on()
            self._pio_write(data)
        if self._cs:
            self._cs.on()

        # Restore the SPI pins to their original mode and alt
        self._dc.init(mode=dcMode, alt=dcAlt)
        self._tx.init(mode=txMode, alt=txAlt)
        self._clk.init(mode=clkMode, alt=clkAlt)

    def _pio_write(self, data):
        """
        Writes data to the display using the PIO.

        Args:
            data (bytes, bytearray, or ndarray): Data to write to the display
        """
        # Configure the DMA transfer count and read address
        count = len(data) if isinstance(data, (bytes, bytearray)) else data.size
        self._dma.count = count // self._bytes_per_transfer
        self._dma.read = data
        
        # Start the state machine and DMA transfer, and wait for it to finish
        self._sm.active(1)
        self._dma.active(True)
        while self._dma.active():
            pass

        # Stop the state machine
        self._sm.active(0)

    @rp2.asm_pio(
            out_init = rp2.PIO.OUT_LOW,
            sideset_init = rp2.PIO.OUT_LOW,
            out_shiftdir = rp2.PIO.SHIFT_LEFT,
            autopull = True
        )
    def _pio_write_spi():
        """
        PIO program to write data to the display.
        """
        out(pins, 1).side(0)
        nop().side(1)

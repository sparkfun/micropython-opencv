from .st7789 import ST7789
from machine import Pin
import rp2

# Derived from:
# https://github.com/raspberrypi/pico-examples/tree/master/pio/st7789_lcd
class ST7789_PIO(ST7789):
    """
    OpenCV PIO driver for ST7789 displays

    Args:
        width (int): display width **Required**
        height (int): display height **Required**
        sm_id (int): State Machine ID for PIO **Required**
        pin_clk (Pin): Clock pin number **Required**
        pin_tx (Pin): Transmit pin number **Required**
        pin_dc (Pin): Data/Command pin number **Required**
        pin_cs (Pin): Chip Select pin number
        freq (int): State machine frequency in Hz, default -1 (system clock)
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
        sm_id,
        pin_clk,
        pin_tx,
        pin_dc,
        pin_cs=None,
        freq=-1,
        rotation=0,
        color_order=ST7789.BGR,
        reverse_bytes_in_word=True,
    ):
        # Store PIO arguments
        self.sm_id = sm_id
        self.clk = Pin(pin_clk) # Don't change mode/alt
        self.tx = Pin(pin_tx) # Don't change mode/alt
        self.dc = Pin(pin_dc) # Don't change mode/alt
        self.cs = Pin(pin_cs, Pin.OUT, value=1) if pin_cs else None
        self.freq = freq
        
        # Get the current mode and alt of the pins so they can be restored
        txMode, txAlt = self.savePinModeAlt(self.tx)
        clkMode, clkAlt = self.savePinModeAlt(self.clk)

        # Initialize the PIO state machine
        self.sm = rp2.StateMachine(
            self.sm_id,
            self._pio_write_spi,
            freq = self.freq,
            out_base = self.tx,
            sideset_base = self.clk,
        )

        # The tx and clk pins just got their mode and alt set for PIO0 or PIO1,
        # so we need to save them again to restore later when _write() is called
        self.txMode, self.txAlt = self.savePinModeAlt(self.tx)
        self.clkMode, self.clkAlt = self.savePinModeAlt(self.clk)
        
        # Now restore the original mode and alt of the pins
        self.tx.init(mode=txMode, alt=txAlt)
        self.clk.init(mode=clkMode, alt=clkAlt)
        
        # Call the parent class constructor
        super().__init__(width, height, rotation, color_order, reverse_bytes_in_word)

    def _write(self, command=None, data=None):
        """SPI write to the device: commands and data."""
        # Save the current mode and alt of the spi pins in case they're used by
        # another device on the same SPI bus
        dcMode, dcAlt = self.savePinModeAlt(self.dc)
        txMode, txAlt = self.savePinModeAlt(self.tx)
        clkMode, clkAlt = self.savePinModeAlt(self.clk)

        # Temporarily set the SPI pins to the correct mode and alt for PIO
        self.dc.init(mode=Pin.OUT)
        self.tx.init(mode=self.txMode, alt=self.txAlt)
        self.clk.init(mode=self.clkMode, alt=self.clkAlt)

        # Write to the display
        if self.cs:
            self.cs.off()
        if command is not None:
            self.dc.off()
            self._pio_write(command)
        if data is not None:
            self.dc.on()
            self._pio_write(data)
        if self.cs:
            self.cs.on()

        # Restore the SPI pins to their original mode and alt
        self.dc.init(mode=dcMode, alt=dcAlt)
        self.tx.init(mode=txMode, alt=txAlt)
        self.clk.init(mode=clkMode, alt=clkAlt)

    def _pio_write(self, data):
        """Write data to the display using PIO."""
        # Start the state machine
        self.sm.active(1)
        
        # Write data to the state machine
        self.sm.put(data, 24)

        # Stop the state machine
        self.sm.active(0)

    @rp2.asm_pio(
            out_init = rp2.PIO.OUT_LOW,
            sideset_init = rp2.PIO.OUT_LOW,
            out_shiftdir = rp2.PIO.SHIFT_LEFT,
            autopull = True,
            pull_thresh = 8,
        )
    def _pio_write_spi():
        out(pins, 1).side(0)
        nop().side(1)

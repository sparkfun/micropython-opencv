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
        rotation=0,
        color_order=ST7789.BGR,
        reverse_bytes_in_word=True,
    ):
        # Store PIO arguments
        self.sm_id = sm_id
        self.pin_clk = pin_clk
        self.pin_tx = pin_tx
        # self.pin_dc = pin_dc
        # self.pin_cs = pin_cs

        self.clk = Pin(pin_clk, Pin.OUT) # Don't change mode/alt
        self.tx = Pin(pin_tx, Pin.OUT) # Don't change mode/alt
        self.clk = Pin(pin_clk, Pin.ALT, alt=Pin.ALT_PIO0) # Don't change mode/alt
        self.tx = Pin(pin_tx, Pin.ALT, alt=Pin.ALT_PIO0) # Don't change mode/alt
        self.dc = Pin(pin_dc, Pin.OUT) # Don't change mode/alt
        self.cs = Pin(pin_cs, Pin.OUT, value=1) if pin_cs else None
        
        program = self._pio_write_spi
        # program[0][0]=0x6001
        # program[0][4]=0xb042
        print(program)

        self.sm = rp2.StateMachine(
            self.sm_id,
            program,
            out_base = self.pin_tx,
            sideset_base = self.pin_clk,
            # out_shiftdir = rp2.PIO.SHIFT_LEFT,
        )
        
        super().__init__(width, height, rotation, color_order, reverse_bytes_in_word)

    def _write(self, command=None, data=None):
        """SPI write to the device: commands and data."""
        # Save the current mode and alt of the DC pin in case it's used by
        # another device on the same SPI bus
        # dcMode, dcAlt = self.savePinModeAlt(self.dc)

        # Temporarily set the DC pin to output mode
        self.dc.init(mode=Pin.OUT)

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

        # Restore the DC pin to its original mode and alt
        # self.dc.init(mode=dcMode, alt=dcAlt)

    def _pio_write(self, data):
        """Write data to the display using PIO."""
        # Start the state machine
        self.sm.active(1)
        
        # Write data to the state machine
        self.sm.put(data)

        # Stop the state machine
        self.sm.active(0)

    @rp2.asm_pio(
            # fifo_join = rp2.PIO.JOIN_TX,
            out_init = rp2.PIO.OUT_LOW,
            sideset_init = rp2.PIO.OUT_LOW,
            out_shiftdir = rp2.PIO.SHIFT_LEFT,
            autopull = True,
            pull_thresh = 8,
        )
    def _pio_write_spi():
        out(pins, 1).side(0)
        nop()
        nop()
        nop()
        nop().side(1)
        nop()
        nop()
        nop()

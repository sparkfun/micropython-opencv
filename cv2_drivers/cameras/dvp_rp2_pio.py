import rp2
from machine import Pin, PWM

# This class is derived from:
# https://github.com/adafruit/Adafruit_ImageCapture/blob/main/src/arch/rp2040.cpp
# Released under the MIT license.
# Copyright (c) 2021 Adafruit Industries
class DVP_RP2_PIO():
    """
    This class implements a DVP (Digital Video Port) interface using the RP2 PIO
    (Programmable Input/Output) interface. This is only available on Raspberry
    Pi RP2 processors.
    """
    def __init__(
        self,
        pin_d0,
        pin_vsync,
        pin_hsync,
        pin_pclk,
        pin_xclk,
        xclk_freq,
        sm_id,
        num_data_pins,
        bytes_per_frame,
        byte_swap
    ):
        """
        Initializes the DVP interface with the specified parameters.

        Args:
            pin_d0 (int): Data 0 pin number for DVP interface
            pin_vsync (int): Vertical sync pin number
            pin_hsync (int): Horizontal sync pin number
            pin_pclk (int): Pixel clock pin number
            pin_xclk (int): External clock pin number
            xclk_freq (int): Frequency in Hz for the external clock
            sm_id (int): PIO state machine ID
            num_data_pins (int): Number of data pins used in DVP interface
            bytes_per_frame (int): Number of bytes per frame to capture
            byte_swap (bool): Whether to swap bytes in the captured data
        """
        self._pin_d0 = pin_d0
        self._pin_vsync = pin_vsync
        self._pin_hsync = pin_hsync
        self._pin_pclk = pin_pclk
        self._pin_xclk = pin_xclk
        self._sm_id = sm_id

        # Initialize DVP pins as inputs
        for i in range(num_data_pins):
            Pin(pin_d0+i, Pin.IN)
        Pin(pin_vsync, Pin.IN)
        Pin(pin_hsync, Pin.IN)
        Pin(pin_pclk, Pin.IN)

        # Set up XCLK pin if provided
        if self._pin_xclk is not None:
            self._xclk = PWM(Pin(pin_xclk))
            self._xclk.freq(xclk_freq)
            self._xclk.duty_u16(32768) # 50% duty cycle

        # Copy the PIO program
        program = self._pio_read_dvp

        # Mask in the GPIO pins
        program[0][0] |= self._pin_hsync & 0x1F
        program[0][1] |= self._pin_pclk & 0x1F
        program[0][3] |= self._pin_pclk & 0x1F

        # Mask in the number of data pins
        program[0][2] &= 0xFFFFFFE0
        program[0][2] |= num_data_pins

        # Create PIO state machine to capture DVP data
        self._sm = rp2.StateMachine(
            self._sm_id,
            program,
            in_base = pin_d0
        )

        # Create DMA controller to transfer data from PIO to buffer
        self._dma = rp2.DMA()
        req_num = ((self._sm_id // 4) << 3) + (self._sm_id % 4) + 4
        bytes_per_transfer = 4
        dma_ctrl = self._dma.pack_ctrl(
            # 0 = 1 byte, 1 = 2 bytes, 2 = 4 bytes
            size = {1:0, 2:1, 4:2}[bytes_per_transfer],
            inc_read = False,
            treq_sel = req_num,
            bswap = byte_swap
        )
        self._dma.config(
            read = self._sm,
            count = bytes_per_frame // bytes_per_transfer,
            ctrl = dma_ctrl
        )

    def _active(self, active=None):
        """
        Sets or gets the active state of the DVP interface.

        Args:
            active (bool, optional):
                - True: Activate the DVP interface
                - False: Deactivate the DVP interface
                - None: Get the current active state
        """
        # If no argument is provided, return the current active state
        if active == None:
            return self._sm.active()
        
        # Disable the DMA, the VSYNC handler will re-enable it when needed
        self._dma.active(False)

        # Set the active state of the state machine
        self._sm.active(active)

        # If active, set up the VSYNC interrupt handler
        if active:
            Pin(self._pin_vsync).irq(
                trigger = Pin.IRQ_FALLING,
                handler = lambda pin: self._vsync_handler()
            )
        # If not active, disable the VSYNC interrupt handler
        else:
            Pin(self._pin_vsync).irq(
                handler = None
            )

    def _vsync_handler(self):
        """
        Handles the VSYNC interrupt to capture a frame of data.
        """
        # Disable DMA before reconfiguring it
        self._dma.active(False)

        # Reset state machine to ensure ISR is cleared
        self._sm.restart()

        # Ensure PIO RX FIFO is empty (it's not emptied by `sm.restart()`)
        while self._sm.rx_fifo() > 0:
            self._sm.get()

        # Reset the DMA write address
        self._dma.write = self._buffer

        # Start the DMA
        self._dma.active(True)

    # Here is the PIO program, which is configurable to mask in the GPIO pins
    # and the number of data pins. It must be configured before the state
    # machine is created
    @rp2.asm_pio(
            in_shiftdir = rp2.PIO.SHIFT_LEFT,
            push_thresh = 32,
            autopush = True,
            fifo_join = rp2.PIO.JOIN_RX
        )
    def _pio_read_dvp():
        """
        PIO program to read DVP data from the GPIO pins.
        """
        wait(1, gpio, 0) # Mask in HSYNC pin
        wait(1, gpio, 0) # Mask in PCLK pin
        in_(pins, 1)     # Mask in number of pins
        wait(0, gpio, 0) # Mask in PCLK pin

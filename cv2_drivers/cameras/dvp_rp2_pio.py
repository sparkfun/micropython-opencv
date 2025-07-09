import rp2
from machine import Pin, PWM

class DVP_RP2_PIO():
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
        self.pin_d0 = pin_d0
        self.pin_vsync = pin_vsync
        self.pin_hsync = pin_hsync
        self.pin_pclk = pin_pclk
        self.pin_xclk = pin_xclk
        self.sm_id = sm_id

        # Initialize DVP pins as inputs
        for i in range(num_data_pins):
            Pin(pin_d0+i, Pin.IN)
        Pin(pin_vsync, Pin.IN)
        Pin(pin_hsync, Pin.IN)
        Pin(pin_pclk, Pin.IN)

        # Set up XCLK pin if provided
        if self.pin_xclk is not None:
            self.xclk = PWM(Pin(pin_xclk))
            self.xclk.freq(xclk_freq)
            self.xclk.duty_u16(32768) # 50% duty cycle

        # Copy the PIO program
        program = self._pio_read_dvp

        # Mask in the GPIO pins
        program[0][0] |= self.pin_hsync & 0x1F
        program[0][1] |= self.pin_pclk & 0x1F
        program[0][3] |= self.pin_pclk & 0x1F

        # Mask in the number of data pins
        program[0][2] &= 0xFFFFFFE0
        program[0][2] |= num_data_pins

        # Create PIO state machine to capture DVP data
        self.sm = rp2.StateMachine(
            self.sm_id,
            program,
            in_base = pin_d0
        )

        # Create DMA controller to transfer data from PIO to buffer
        self.dma = rp2.DMA()
        req_num = ((self.sm_id // 4) << 3) + (self.sm_id % 4) + 4
        bytes_per_transfer = 4
        dma_ctrl = self.dma.pack_ctrl(
            # 0 = 1 byte, 1 = 2 bytes, 2 = 4 bytes
            size = {1:0, 2:1, 4:2}[bytes_per_transfer],
            inc_read = False,
            treq_sel = req_num,
            bswap = byte_swap
        )
        self.dma.config(
            read = self.sm,
            count = bytes_per_frame // bytes_per_transfer,
            ctrl = dma_ctrl
        )

    def active(self, active = None):
        # If no argument is provided, return the current active state
        if active == None:
            return self.sm.active()
        
        # Disable the DMA, the VSYNC handler will re-enable it when needed
        self.dma.active(False)

        # Set the active state of the state machine
        self.sm.active(active)

        # If active, set up the VSYNC interrupt handler
        if active:
            Pin(self.pin_vsync).irq(
                trigger = Pin.IRQ_FALLING,
                handler = lambda pin: self._vsync_handler()
            )
        # If not active, disable the VSYNC interrupt handler
        else:
            Pin(self.pin_vsync).irq(
                handler = None
            )

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
        wait(1, gpio, 0) # Mask in HSYNC pin
        wait(1, gpio, 0) # Mask in PCLK pin
        in_(pins, 1)     # Mask in number of pins
        wait(0, gpio, 0) # Mask in PCLK pin

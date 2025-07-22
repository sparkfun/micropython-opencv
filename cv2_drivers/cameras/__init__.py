# Import sys module to check platform
import sys

# Import RP2 drivers
if 'rp2' in sys.platform:
    from . import hm01b0_pio
    from . import ov5640_pio

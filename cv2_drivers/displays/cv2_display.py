#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# cv2_display.py
# 
# Base class for OpenCV display drivers.
#-------------------------------------------------------------------------------

import cv2
from ulab import numpy as np
from machine import Pin

class CV2_Display():
    """
    Base class for OpenCV display drivers.
    """
    def __init__(self, buffer_shape):
        """
        Initializes the display.

        Args:
            buffer_shape (tuple): Shape of the buffer as (rows, cols, channels)
        """
        # Create the frame buffer
        self._buffer = np.zeros(buffer_shape, dtype=np.uint8)

    def imshow(self, image):
        """
        Shows a NumPy image on the display.

        Args:
            image (ndarray): Image to show
        """
        raise NotImplementedError("imshow() must be implemented by driver")

    def clear(self):
        """
        Clears the display by filling it with black color.
        """
        raise NotImplementedError("clear() must be implemented by driver")

    def _get_common_roi_with_buffer(self, image):
        """
        Gets the common region of interest (ROI) between the image and the 
        display's internal buffer.

        Args:
            image (ndarray): Image to display
        
        Returns:
            tuple: (image_roi, buffer_roi)
                - image_roi (ndarray): ROI of the image
                - buffer_roi (ndarray): ROI of the display's buffer
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
        row_max = min(image_rows, self._buffer.shape[0])
        col_max = min(image_cols, self._buffer.shape[1])
        img_roi = image[:row_max, :col_max]
        buffer_roi = self._buffer[:row_max, :col_max]
        return img_roi, buffer_roi

    def _convert_to_uint8(self, image):
        """
        Converts the image to uint8 format if necessary.

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

    def _convert_to_bgr565(self, src, dst):
        """
        Converts an image to BGR565 format.

        Args:
            src (ndarray): Input image
            dst (ndarray): Output BGR565 buffer
        """
        # Determine the number of channels in the image
        if src.ndim < 3:
            ch = 1
        else:
            ch = src.shape[2]

        # Convert the image to BGR565 format based on the number of channels
        if ch == 1: # Grayscale
            dst = cv2.cvtColor(src, cv2.COLOR_GRAY2BGR565, dst)
        elif ch == 2: # Already in BGR565 format
            # For some reason, this is relatively slow and creates a new buffer:
            # https://github.com/v923z/micropython-ulab/issues/726
            dst[:] = src
        elif ch == 3: # BGR
            dst = cv2.cvtColor(src, cv2.COLOR_BGR2BGR565, dst)
        else:
            raise ValueError("Image must be 1, 2 or 3 channels (grayscale, BGR565, or BGR)")

    def _save_pin_mode_alt(self, pin):
        """
        Saves the current `mode` and `alt` of the pin so it can be restored
        later. Mostly used for SPI displays on a shared SPI bus with a driver
        that needs non-SPI pin modes, such as the RP2 PIO driver. This allows
        other devices on the bus to continue using the SPI interface after the
        display driver finishes communicating with the display.

        Returns:
            tuple: (mode, alt)
        """
        # See: https://github.com/micropython/micropython/issues/17515
        # There's no way to get the mode and alt of a pin directly, so we
        # convert the pin to a string and parse it. Example formats:
        # "Pin(GPIO16, mode=OUT)"
        # "Pin(GPIO16, mode=ALT, alt=SPI)"
        pin_str = str(pin)

        # Extract the "mode" parameter from the pin string
        try:
            # Split between "mode=" and the next comma or closing parenthesis
            mode_str = pin_str[pin_str.index("mode=") + 5:].partition(",")[0].partition(")")[0]

            # Look up the mode in Pin class dictionary
            mode = Pin.__dict__[mode_str]
        except (ValueError, KeyError):
            # No mode specified, just set to -1 (default)
            mode = -1

        # Extrct the "alt" parameter from the pin string
        try:
            # Split between "alt=" and the next comma or closing parenthesis
            alt_str = pin_str[pin_str.index("alt=") + 4:].partition(",")[0].partition(")")[0]

            # Sometimes the value comes back as a number instead of a valid
            # "ALT_xyz" string, so we need to check it
            if "ALT_" + alt_str in Pin.__dict__:
                # Look up the alt in Pin class dictionary (with "ALT_" prefix)
                alt = Pin.__dict__["ALT_" + alt_str]
            else:
                # Convert the altStr to an integer
                alt = int(alt_str)
        except (ValueError, KeyError):
            # No alt specified, just set to -1 (default)
            alt = -1

        # Return the mode and alt as a tuple
        return (mode, alt)

    def splash(self, filename="splash.png"):
        """
        Shows a splash image on the display if one is available, otherwise
        clears the display of any previous content.

        Args:
            filename (str, optional): Path to a splash image file. Defaults to
                            "splash.png"
        """
        try:
            # Attempt to load and show the splash image
            self.imshow(cv2.imread(filename))
        except Exception:
            # Couldn't load the image, just clear the display as a fallback
            self.clear()

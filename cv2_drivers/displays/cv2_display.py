import cv2
from ulab import numpy as np
from machine import Pin

class CV2_Display():
    def __init__(self, buffer_size):
        # Create the frame buffer
        self.buffer = np.zeros(buffer_size, dtype=np.uint8)

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

    def savePinModeAlt(self, pin):
        """
        Saves the current `mode` and `alt` of the pin so it can be restored
        later. Mostly used to restore the SPI mode (MISO) of the DC pin after
        communication with the display in case another device is using the same
        SPI bus.

        Returns:
            tuple: (mode, alt)
        """
        # See: https://github.com/micropython/micropython/issues/17515
        # There's no way to get the mode and alt of a pin directly, so we
        # convert the pin to a string and parse it. Example formats:
        # "Pin(GPIO16, mode=OUT)"
        # "Pin(GPIO16, mode=ALT, alt=SPI)"
        pinStr = str(pin)

        # Extract the "mode" parameter from the pin string
        if "mode=" in pinStr:
            # Split between "mode=" and the next comma or closing parenthesis
            modeStr = pinStr.split("mode=")[1].split(",")[0].split(")")[0]

            # Look up the mode in Pin class dictionary
            mode = Pin.__dict__[modeStr]
        else:
            # No mode specified, just set to -1 (default)
            mode = -1

        # Extrct the "alt" parameter from the pin string
        if "alt=" in pinStr:
            # Split between "alt=" and the next comma or closing parenthesis
            altStr = pinStr.split("alt=")[1].split(",")[0].split(")")[0]

            # Sometimes the value comes back as a number instead of a valid
            # "ALT_xyz" string, so we need to check it
            if "ALT_" + altStr in Pin.__dict__:
                # Look up the alt in Pin class dictionary (with "ALT_" prefix)
                alt = Pin.__dict__["ALT_" + altStr]
            else:
                # Convert the altStr to an integer
                alt = int(altStr)
        else:
            # No alt specified, just set to -1 (default)
            alt = -1

        # Return the mode and alt as a tuple
        return (mode, alt)

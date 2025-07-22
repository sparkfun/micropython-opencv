class CV2_Camera():
    """
    Base class for OpenCV camera drivers.
    """
    def __init__(self):
        """
        Initializes the camera.
        """
        pass

    def open(self):
        """
        Opens the camera and prepares it for capturing images.
        """
        raise NotImplementedError("open() must be implemented by driver")

    def release(self):
        """
        Releases the camera and frees any resources.
        """
        raise NotImplementedError("release() must be implemented by driver")

    def read(self, image=None):
        """
        Reads an image from the camera.

        Args:
            image (ndarray, optional): Image to read into

        Returns:
            tuple: (success, image)
                - success (bool): True if the image was read, otherwise False
                - image (ndarray): The captured image, or None if reading failed
        """
        raise NotImplementedError("read() must be implemented by driver")

import cv2
from ulab import numpy as np
from st7789_spi import ST7789_SPI
import time

display = ST7789_SPI(width=240,
                    height=320,
                    spi_id=0,
                    pin_cs=17,
                    pin_dc=16,
                    rotation=1)

x = 0
y = 0
vx = 10
vy = 10

while True:
    img = np.zeros((240, 320, 3), dtype=np.uint8)
    img[0:50, :] = (255, 0, 0)
    img = cv2.ellipse(img, (160, 120), (100, 50), 0, 0, 360, (0, 255, 0), -1)
    img = cv2.putText(img, "Hello OpenCV!", (50, 200), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    cv2.imshow(display, img)
    key = cv2.waitKey(1000)
    
    edge_img = cv2.Canny(img, 100, 200)
    cv2.imshow(display, edge_img)
    key = cv2.waitKey(2000)

    img = np.zeros((240, 320, 3), dtype=np.uint8)
    t0 = time.time()
    while time.time() - t0 < 5:
        # Update position
        x += vx
        y += vy
        if x <= 0 or x >= 320 - 50:
            vx = -vx  # Reverse direction on x-axis
        if y <= 0 or y >= 240 - 50:
            vy = -vy # Reverse direction on y-axis

        # Draw a square
        img[y:y+50, x:x+50] = (0, 0, 255)

        cv2.imshow(display, img)

        # Clear the square area for the next frame
        img[y:y+50, x:x+50] = (0, 0, 0)
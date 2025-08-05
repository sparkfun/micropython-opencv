# SparkFun MicroPython-OpenCV

Welcome to SparkFun's MicroPython port of OpenCV! This is the first known MicroPython port of OpenCV, which opens up a whole new world of vision processing abilities on embedded devices in a Python environment!

As the first port, there may be incomplete or missing features, and some rough edges. For example, we have only implemented support for the Raspberry Pi RP2350 so far, and some of the build procedures are hard-coded for that. We'd be happy to work with the community to create an official port in the future, but until then, this repo is available and fully open-source for anyone to use!

# Example Snippets

Below are example code snippets of features avaiable in this port of OpenCV. We've done our best to make it as similar as possible to standard OpenCV, but there are some necessary API changes due to the limitations of MicroPython.

```python
# Import OpenCV, just like any other Python environment!
import cv2 as cv

# Import ulab NumPy and initialize an image, almost like any other Python
# environment!
from ulab import numpy as np
img = np.zeros((240, 320, 3), dtype=np.uint8)

# Call OpenCV functions, just like standard OpenCV!
img = cv.putText(img, "Hello OpenCV!", (50, 200), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
img = cv.Canny(img, 100, 200)

# Call `cv.imshow()`, almost like standard OpenCV! Instead of passing a window
# name string, you pass a display driver that implements an `imshow()` method
# that takes a NumPy array as input.
cv.imshow(display, img)

# Call `cv.waitKey()`, just like standard OpenCV! Unlike standard OpenCV, this
# waits for a key press on the REPL instead of a window, and it is not necessary
# to call after `cv.imshow()` because display drivers show images immediately.
key = cv.waitKey(0)

# Use a camera, similar to standard OpenCV! `cv.VideoCapture()` is not used in
# MicroPython-OpenCV, because a separate camera driver that implements the same
# methods as the OpenCV `VideoCapture` class must be initialized separately.
camera.open()
success, frame = camera.read()
camera.release()

# Call `cv.imread()` and `cv.imwrite()` to read and write images to and from
# the MicroPython filesystem, just like standard OpenCV! It can also point to an
# SD card if one is mounted for extra storage space.
img = cv.imread("path/to/image.png")
success = cv.imwrite("path/to/image.png", img)
```

For full example, see our [Red Vision repo](https://github.com/sparkfun/red_vision).

# Performance

Limit your expectations. OpenCV typically runs on full desktop systems containing processors running at GHz speeds with dozens of cores optimized for computing speed and GB of RAM. In contrast, microcontrollers processors typically run at a few hundred MHz with 1 or 2 cores optimized for low power consumtion with a few MB of RAM. Exact performance depends on many things, including the processor, vision pipeline, image resolution, colorspaces used, RAM available, etc.

If you want best performance, keep in mind is that MicroPython uses a garbage collector for memory management. If images are repeatedly created in a vision pipeline, RAM will be consumed until the garbage collector runs. The collection process takes longer with more RAM, so this can result in noticable delays during collection (typically a few hundred milliseconds). To mitigate this, it's best to pre-allocate arrays and utilize the optional `dst` argument of OpenCV functions so memory consumption is minimized. Pre-allocation also helps improve performance, because allocating memory takes time.

Below are some typical execution times for various OpenCV functions. All were tested on a Raspberry Pi RP2350 with a 320x240 test image.

| Function | Execution Time |
| --- | --- |
| `dst = cv.blur(src, (5, 5))` | 115ms |
| `dst = cv.blur(src, (5, 5), dst)` | 87ms |
| `retval, dst = cv.threshold(src, 127, 255, cv.THRESH_BINARY)` | 76ms |
| `retval, dst = cv.threshold(src, 127, 255, cv.THRESH_BINARY, dst)` | 46ms |
| `dst = cv.cvtColor(src, cv.COLOR_BGR2HSV)` | 114ms |
| `dst = cv.cvtColor(src, cv.COLOR_BGR2HSV, dst)` | 84ms |
| `dst = cv.Canny(src, 100, 200)` | 504ms |
| `dst = cv.Canny(src, 100, 200, dst)` | 482ms |

# Included OpenCV Functions

Below is a list of all OpenCV functions included in the MicroPython port of OpenCV. This section follows OpenCV's module structure.

Only the most useful OpenCV functions are included. The MicroPython environment is *extremely* limited, so many functions are omitted due to prohibitively high RAM and firmware size requirements. Other less useful functions have been omitted to reduce firmware size. If there are additional functions you'd like to be included, see [#Contributing](#Contributing).

If you need help understanding how to use these functions, see the documentation link for each function. You can also check out [OpenCV's Python Tutorials](https://docs.opencv.org/4.11.0/d6/d00/tutorial_py_root.html) and other tutorials online for more educational experience. This repository is simply a port of OpenCV, so we do not document these functions or how to use them, except for deviations from standard OpenCV.

## [`core`](https://docs.opencv.org/4.11.0/d0/de1/group__core.html)

> [!NOTE]
> The `core` module includes many functions for basic operations on arrays. Most of these can be performed by `numpy` operations, so they have been omitted to reduce firmware size.

### [Operations on arrays](https://docs.opencv.org/4.11.0/d2/de8/group__core__array.html)

| Function | Notes |
| --- | --- |
| `cv.convertScaleAbs(src[, dst[, alpha[, beta]]]) -> dst`<br>Scales, calculates absolute values, and converts the result to 8-bit.<br>[Documentation](https://docs.opencv.org/4.11.0/d2/de8/group__core__array.html#ga3460e9c9f37b563ab9dd550c4d8c4e7d) | |
| `cv.inRange(src, lowerb, upperb[, dst]) -> dst`<br>Checks if array elements lie between the elements of two other arrays.<br>[Documentation](https://docs.opencv.org/4.11.0/d2/de8/group__core__array.html#ga48af0ab51e36436c5d04340e036ce981) | |
| `cv.minMaxLoc(src[, mask]) -> minVal, maxVal, minLoc, maxLoc`<br>Finds the global minimum and maximum in an array.<br>[Documentation](https://docs.opencv.org/4.11.0/d2/de8/group__core__array.html#gab473bf2eb6d14ff97e89b355dac20707) | |

## [`imgproc`](https://docs.opencv.org/4.11.0/d7/dbd/group__imgproc.html)

### [Image Filtering](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html)

| Function | Notes |
| --- | --- |
| `cv.bilateralFilter(src, d, sigmaColor, sigmaSpace[, dst[, borderType]]) -> dst`<br>Applies the bilateral filter to an image.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga9d7064d478c95d60003cf839430737ed) | |
| `cv.blur(src, ksize[, dst[, anchor[, borderType]]]) -> dst`<br>Blurs an image using the normalized box filter.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga8c45db9afe636703801b0b2e440fce37) | |
| `cv.boxFilter(src, ddepth, ksize[, dst[, anchor[, normalize[, borderType]]]]) -> dst`<br>Blurs an image using the box filter.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gad533230ebf2d42509547d514f7d3fbc3) | |
| `cv.dilate(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst`<br>Dilates an image by using a specific structuring element.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga4ff0f3318642c4f469d0e11f242f3b6c) | |
| `cv.erode(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst`<br>Erodes an image by using a specific structuring element.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gaeb1e0c1033e3f6b891a25d0511362aeb) | |
| `cv.filter2D(src, ddepth, kernel[, dst[, anchor[, delta[, borderType]]]]) -> dst`<br>Convolves an image with the kernel.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga27c049795ce870216ddfb366086b5a04) | |
| `cv.GaussianBlur(src, ksize, sigmaX[, dst[, sigmaY[, borderType[, hint]]]]) -> dst`<br>Blurs an image using a Gaussian filter.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gae8bdcd9154ed5ca3cbc1766d960f45c1) | |
| `cv.getStructuringElement(shape, ksize[, anchor]) -> retval`<br>Returns a structuring element of the specified size and shape for morphological operations.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gac342a1bb6eabf6f55c803b09268e36dc) | |
| `cv.Laplacian(src, ddepth[, dst[, ksize[, scale[, delta[, borderType]]]]]) -> dst`<br>Calculates the Laplacian of an image.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gad78703e4c8fe703d479c1860d76429e6) | |
| `cv.medianBlur(src, ksize[, dst]) -> dst`<br>Blurs an image using the median filter.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga564869aa33e58769b4469101aac458f9) | |
| `cv.morphologyEx(src, op, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst`<br>Performs advanced morphological transformations.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga67493776e3ad1a3df63883829375201f) | |
| `cv.Scharr(src, ddepth, dx, dy[, dst[, scale[, delta[, borderType]]]]) -> dst`<br>Calculates the first x- or y- image derivative using Scharr operator.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gaa13106761eedf14798f37aa2d60404c9) | |
| `cv.Sobel(src, ddepth, dx, dy[, dst[, ksize[, scale[, delta[, borderType]]]]]) -> dst`<br>Calculates the first, second, third, or mixed image derivatives using an extended Sobel operator.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#gacea54f142e81b6758cb6f375ce782c8d) | |
| `cv.spatialGradient(src[, dx[, dy[, ksize[, borderType]]]]) -> dx, dy`<br>Calculates the first order image derivative in both x and y using a Sobel operator.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/d86/group__imgproc__filter.html#ga405d03b20c782b65a4daf54d233239a2) | |

### [Miscellaneous Image Transformations](https://docs.opencv.org/4.11.0/d7/d1b/group__imgproc__misc.html)

| Function | Notes |
| --- | --- |
| `cv.adaptiveThreshold(src, maxValue, adaptiveMethod, thresholdType, blockSize, C[, dst]) -> dst`<br>Applies an adaptive threshold to an array.<br>[Documentation](https://docs.opencv.org/4.11.0/d7/d1b/group__imgproc__misc.html#ga72b913f352e4a1b1b397736707afcde3) | |
| `cv.threshold(src, thresh, maxval, type[, dst]) -> retval, dst`<br>Applies a fixed-level threshold to each array element.<br>[Documentation](https://docs.opencv.org/4.11.0/d7/d1b/group__imgproc__misc.html#gae8a4a146d1ca78c626a53577199e9c57) | |

### [Drawing Functions](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html)

| Function | Notes |
| --- | --- |
| `cv.arrowedLine(img, pt1, pt2, color[, thickness[, line_type[, shift[, tipLength]]]]) -> img`<br>Draws an arrow segment pointing from the first point to the second one.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga0a165a3ca093fd488ac709fdf10c05b2) | |
| `cv.circle(img, center, radius, color[, thickness[, lineType[, shift]]]) -> img`<br>Draws a circle.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#gaf10604b069374903dbd0f0488cb43670) | |
| `cv.drawContours(image, contours, contourIdx, color[, thickness[, lineType[, hierarchy[, maxLevel[, offset]]]]]) -> image`<br>Draws contours outlines or filled contours.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga746c0625f1781f1ffc9056259103edbc) | |
| `cv.drawMarker(img, position, color[, markerType[, markerSize[, thickness[, line_type]]]]) -> img`<br>Draws a marker on a predefined position in an image.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga644c4a170d4799a56b29f864ce984b7e) | |
| `cv.ellipse(img, center, axes, angle, startAngle, endAngle, color[, thickness[, lineType[, shift]]]) -> img`<br>Draws a simple or thick elliptic arc or fills an ellipse sector.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga28b2267d35786f5f890ca167236cbc69) | |
| `cv.fillConvexPoly(img, points, color[, lineType[, shift]]) -> img`<br>Fills a convex polygon.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga9bb982be9d641dc51edd5e8ae3624e1f) | |
| `cv.fillPoly(img, pts, color[, lineType[, shift[, offset]]]) -> img`<br>Fills the area bounded by one or more polygons.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga311160e71d37e3b795324d097cb3a7dc) | |
| `cv.line(img, pt1, pt2, color[, thickness[, lineType[, shift]]]) -> img`<br>Draws a line segment connecting two points.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga7078a9fae8c7e7d13d24dac2520ae4a2) | |
| `cv.putText(img, text, org, fontFace, fontScale, color[, thickness[, lineType[, bottomLeftOrigin]]]) -> img`<br>Draws a text string.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga5126f47f883d730f633d74f07456c576) | |
| `cv.rectangle(img, pt1, pt2, color[, thickness[, lineType[, shift]]]) -> img`<br>Draws a simple, thick, or filled up-right rectangle.<br>[Documentation](https://docs.opencv.org/4.11.0/d6/d6e/group__imgproc__draw.html#ga07d2f74cadcf8e305e810ce8eed13bc9) | |

### [Color Space Conversions](https://docs.opencv.org/4.11.0/d8/d01/group__imgproc__color__conversions.html)

| Function | Notes |
| --- | --- |
| `cv.cvtColor(src, code[, dst[, dstCn[, hint]]]) -> dst`<br>Converts an image from one color space to another.<br>[Documentation](https://docs.opencv.org/4.11.0/d8/d01/group__imgproc__color__conversions.html#gaf86c09fe702ed037c03c2bc603ceab14) | |

### [Structural Analysis and Shape Descriptors](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html)

| Function | Notes |
| --- | --- |
| `cv.approxPolyDP(curve, epsilon, closed[, approxCurve]) -> approxCurve`<br>Approximates a polygonal curve(s) with the specified precision.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga0012a5fdaea70b8a9970165d98722b4c) | |
| `cv.approxPolyN(curve, nsides[, approxCurve[, epsilon_percentage[, ensure_convex]]]) -> approxCurve`<br>Approximates a polygon with a convex hull with a specified accuracy and number of sides.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga88981607a2d61b95074688aac55625cc) | |
| `cv.arcLength(curve, closed) -> retval`<br>Calculates a contour perimeter or a curve length.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga8d26483c636be6b35c3ec6335798a47c) | |
| `cv.boundingRect(array) -> retval`<br>Calculates the up-right bounding rectangle of a point set or non-zero pixels of gray-scale image.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga103fcbda2f540f3ef1c042d6a9b35ac7) | |
| `cv.boxPoints(box[, points]) -> points`<br>Finds the four vertices of a rotated rect. Useful to draw the rotated rectangle.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gaf78d467e024b4d7936cf9397185d2f5c) | |
| `cv.connectedComponents(image[, labels[, connectivity[, ltype]]]) -> retval, labels`<br>computes the connected components labeled image of boolean image<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gaedef8c7340499ca391d459122e51bef5) | `ltype` defaults to `CV_16U` instead of `CV_32S` due to ulab not supporting 32-bit integers. See: https://github.com/v923z/micropython-ulab/issues/719 |
| `cv.connectedComponentsWithStats(image[, labels[, stats[, centroids[, connectivity[, ltype]]]]]) -> retval, labels, stats, centroids`<br>computes the connected components labeled image of boolean image and also produces a statistics output for each label<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga107a78bf7cd25dec05fb4dfc5c9e765f) | `labels`, `stats`, and `centroids` are returned with `dtype=np.float` instead of `np.int32` due to ulab not supporting 32-bit integers. See: https://github.com/v923z/micropython-ulab/issues/719 |
| `cv.contourArea(contour[, oriented]) -> retval`<br>Calculates a contour area.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga2c759ed9f497d4a618048a2f56dc97f1) | |
| `cv.convexHull(points[, hull[, clockwise[, returnPoints]]]) -> hull`<br>Finds the convex hull of a point set.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga014b28e56cb8854c0de4a211cb2be656) | `hull` is returned with `dtype=np.float` instead of `np.int32` due to ulab not supporting 32-bit integers. See: https://github.com/v923z/micropython-ulab/issues/719 |
| `cv.convexityDefects(contour, convexhull[, convexityDefects]) -> convexityDefects`<br>Finds the convexity defects of a contour.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gada4437098113fd8683c932e0567f47ba) | `convexityDefects` is returned with `dtype=np.float` instead of `np.int32` due to ulab not supporting 32-bit integers. See: https://github.com/v923z/micropython-ulab/issues/719 |
| `cv.findContours(image, mode, method[, contours[, hierarchy[, offset]]]) -> contours, hierarchy`<br>Finds contours in a binary image.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gadf1ad6a0b82947fa1fe3c3d497f260e0) | `contours` and `hierarchy` are returned with `dtype=np.float` and `dtype=np.int16` respectively instead of `np.int32` due to ulab not supporting 32-bit integers. See: https://github.com/v923z/micropython-ulab/issues/719 |
| `cv.fitEllipse(points) -> retval`<br>Fits an ellipse around a set of 2D points.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gaf259efaad93098103d6c27b9e4900ffa) | |
| `cv.fitLine(points, distType, param, reps, aeps[, line]) -> line`<br>Fits a line to a 2D or 3D point set.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gaf849da1fdafa67ee84b1e9a23b93f91f) | |
| `cv.isContourConvex(contour) -> retval`<br>Tests a contour convexity.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga8abf8010377b58cbc16db6734d92941b) | |
| `cv.matchShapes(contour1, contour2, method, parameter) -> retval`<br>Compares two shapes.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#gaadc90cb16e2362c9bd6e7363e6e4c317) | |
| `cv.minAreaRect(points) -> retval`<br>Finds a rotated rectangle of the minimum area enclosing the input 2D point set.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga3d476a3417130ae5154aea421ca7ead9) | |
| `cv.minEnclosingCircle(points) -> center, radius`<br>Finds a circle of the minimum area enclosing a 2D point set.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga8ce13c24081bbc7151e9326f412190f1) | |
| `cv.minEnclosingTriangle(points[, triangle]) -> retval, triangle`<br>Finds a triangle of minimum area enclosing a 2D point set and returns its area.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga1513e72f6bbdfc370563664f71e0542f) | |
| `cv.moments(array[, binaryImage]) -> retval`<br>Calculates all of the moments up to the third order of a polygon or rasterized shape.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga556a180f43cab22649c23ada36a8a139) | |
| `cv.pointPolygonTest(contour, pt, measureDist) -> retval`<br>Performs a point-in-contour test.<br>[Documentation](https://docs.opencv.org/4.11.0/d3/dc0/group__imgproc__shape.html#ga1a539e8db2135af2566103705d7a5722) | |

### [Feature Detection](https://docs.opencv.org/4.11.0/dd/d1a/group__imgproc__feature.html)

| Function | Notes |
| --- | --- |
| `cv.Canny(image, threshold1, threshold2[, edges[, apertureSize[, L2gradient]]]) -> edges`<br>Finds edges in an image using the Canny algorithm.<br>[Documentation](https://docs.opencv.org/4.11.0/dd/d1a/group__imgproc__feature.html#ga04723e007ed888ddf11d9ba04e2232de) | |
| `cv.HoughCircles(image, method, dp, minDist[, circles[, param1[, param2[, minRadius[, maxRadius]]]]]) -> circles`<br>Finds circles in a grayscale image using the Hough transform.<br>[Documentation](https://docs.opencv.org/4.11.0/dd/d1a/group__imgproc__feature.html#ga47849c3be0d0406ad3ca45db65a25d2d) | |
| `cv.HoughCirclesWithAccumulator(image, method, dp, minDist[, circles[, param1[, param2[, minRadius[, maxRadius]]]]]) -> circles`<br>Finds circles in a grayscale image using the Hough transform and get accumulator.<br>[Documentation](https://docs.opencv.org/4.x/d2/d75/namespacecv.html#aed6d238ceede74293152437228c603ce) | |
| `cv.HoughLines(image, rho, theta, threshold[, lines[, srn[, stn[, min_theta[, max_theta[, use_edgeval]]]]]]) -> lines`<br>Finds lines in a binary image using the standard Hough transform.<br>[Documentation](https://docs.opencv.org/4.11.0/dd/d1a/group__imgproc__feature.html#ga0b7ee275f89bb1a799ab70a42131f39d) | |
| `cv.HoughLinesP(image, rho, theta, threshold[, lines[, minLineLength[, maxLineGap]]]) -> lines`<br>Finds line segments in a binary image using the probabilistic Hough transform.<br>[Documentation](https://docs.opencv.org/4.11.0/dd/d1a/group__imgproc__feature.html#ga8618180a5948286384e3b7ca02f6feeb) | `lines` is returned with `dtype=np.float` instead of `np.int32` due to ulab not supporting 32-bit integers. See: https://github.com/v923z/micropython-ulab/issues/719 |
| `cv.HoughLinesWithAccumulator(image, rho, theta, threshold[, lines[, srn[, stn[, min_theta[, max_theta[, use_edgeval]]]]]]) -> lines`<br>Finds lines in a binary image using the standard Hough transform and get accumulator.<br>[Documentation](https://docs.opencv.org/4.x/d2/d75/namespacecv.html#ad5e6dca5163cd4bd0135cb808f1ddfe8) | |

### [Object Detection](https://docs.opencv.org/4.11.0/df/dfb/group__imgproc__object.html)

| Function | Notes |
| --- | --- |
| `cv.matchTemplate(image, templ, method[, result[, mask]]) -> result`<br>Compares a template against overlapped image regions.<br>[Documentation](https://docs.opencv.org/4.11.0/df/dfb/group__imgproc__object.html#ga586ebfb0a7fb604b35a23d85391329be) | |

## [`imgcodecs`](https://docs.opencv.org/4.11.0/d4/da8/group__imgcodecs.html)

| Function | Notes |
| --- | --- |
| `cv.imread(filename[, flags]) -> retval`<br>Loads an image from a file.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/da8/group__imgcodecs.html#gacbaa02cffc4ec2422dfa2e24412a99e2) | `filename` can be anywhere in the full MicroPython filesystem, including SD cards if mounted.<br>Only BMP and PNG formats are currently supported. |
| `cv.imwrite(filename, img[, params]) -> retval`<br>Saves an image to a specified file.<br>[Documentation](https://docs.opencv.org/4.11.0/d4/da8/group__imgcodecs.html#ga8ac397bd09e48851665edbe12aa28f25) | `filename` can be anywhere in the full MicroPython filesystem, including SD cards if mounted.<br>Only BMP and PNG formats are currently supported. |

## [`highgui`](https://docs.opencv.org/4.11.0/d7/dfc/group__highgui.html)

| Function | Notes |
| --- | --- |
| `cv.imshow(winname, mat) -> None`<br>Displays an image in the specified window.<br>[Documentation](https://docs.opencv.org/4.11.0/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563) | `winname` must actually be a display driver object that implements an `imshow()` method that takes a NumPy array as input. |
| `cv.waitKey([, delay]) -> retval`<br>Waits for a pressed key.<br>[Documentation](https://docs.opencv.org/4.11.0/d7/dfc/group__highgui.html#ga5628525ad33f52eab17feebcfba38bd7) | Input is taken from `sys.stdin`, which is typically the REPL. |
| `cv.waitKeyEx([, delay]) -> retval`<br>Similar to waitKey, but returns full key code.<br>[Documentation](https://docs.opencv.org/4.11.0/d7/dfc/group__highgui.html#ga5628525ad33f52eab17feebcfba38bd7) | Input is taken from `sys.stdin`, which is typically the REPL.<br>Full key code is implementation specific, so special key codes in MicroPython will not match other Python environments. |

# Hardware Drivers

Standard OpenCV leverages the host operating system to access hardware, like creating windows and accessing cameras. MicroPython does not have that luxury, so instead, drivers must be implemented for these hardware devices. Take a look at our [Red Vision repo](https://github.com/sparkfun/red_vision) for examples. This leads to necessary API changes for functions like `cv.imshow()`.

# MicroPython Board Requirements

As of writing, the OpenCV firmware adds over 3MiB on top of the standard MicroPython firmware, which itself be up to 1MiB in size (depending on platform and board). You'll also want some storage space, so a board with at least 8MB of flash is recommended.

PSRAM is basically a requirement to do anything useful with OpenCV. A single 320x240 RGB888 frame buffer requires 225KiB of RAM; most microcontrollers only have a few hundred KiB of SRAM. Several frame buffers can be needed for even simple vision pipelines, so you really need at least a few MiB of RAM available. The more the merrier!

# Building

Below are instructions to build the MicroPython-OpenCV firmware from scratch. Instructions are only provided for Linux systems.

> [!NOTE]
> This build process does not include any hardware drivers, see our [Red Vision repo](https://github.com/sparkfun/red_vision) for example drivers.

> [!NOTE]
> Because OpenCV dramatically increases the firmware size, it may be necessary to define board variants that reduce the storage size to avoid it overlapping with the firmware. See [#Adding New Boards](#Adding-New-Boards).

1. Clone this repo and MicroPython
    * ```
      cd ~
      git clone https://github.com/sparkfun/micropython-opencv.git
      git clone https://github.com/micropython/micropython.git
      ```
2. Build the MicroPython cross-compiler
    * ```
      make -C micropython/mpy-cross -j4
      ```
3. Clone MicroPython submodules for your board
    * ```
      make -C micropython/ports/rp2 BOARD=SPARKFUN_XRP_CONTROLLER submodules
      ```
    * Replace `rp2` and `SPARKFUN_XRP_CONTROLLER` with your platform and board name respectively
4. Set environment variables (if needed)
    * Some platforms require environment variables to be set. Example:
    * ```
      export PICO_SDK_PATH=~/micropython/lib/pico-sdk
      ```
5. Build OpenCV for your platform
    * ```
      make -C micropython-opencv PLATFORM=rp2350 --no-print-directory -j4
      ```
    * Replace `rp2350` with your board's platform
6. Build MicroPython-OpenCV firmware for your board
    * ```
      export CMAKE_ARGS="-DSKIP_PICO_MALLOC=1 -DPICO_CXX_ENABLE_EXCEPTIONS=1" && make -C micropython/ports/rp2 BOARD=SPARKFUN_XRP_CONTROLLER USER_C_MODULES=~/micropython-opencv/micropython_opencv.cmake -j4
      ```
    * Replace `rp2` and `SPARKFUN_XRP_CONTROLLER` with your platform and board name respectively
    * Replace the `CMAKE_ARGS` contents with whatever is required for your board's platform
    * Your firmware file(s) will be located in `~/micropython/ports/<port-name>/build-<board-name>/`

# Adding New Boards

Because OpenCV dramatically increases the firmware size, it may be necessary to define board variants that reduce the storage size to avoid it overlapping with the firmware. It is also beneficial to adjust the board name to include `OpenCV` or similar to help people identify that the MicroPython-OpenCV firmware is flashed to the board instead of standard MicroPython.

Below is the variant for the XRP Controller as an example. The variant is defined by creating a file called [`micropython/ports/rp2/boards/SPARKFUN_XRP_CONTROLLER/mpconfigvariant_RED_VISION.cmake`](https://github.com/sparkfun/micropython/blob/7e728e8c6aad74ca244183f3e0705db6f332abd9/ports/rp2/boards/SPARKFUN_XRP_CONTROLLER/mpconfigvariant_LARGE_BINARY.cmake) with contents:

```
list(APPEND MICROPY_DEF_BOARD
    # Board name
    "MICROPY_HW_BOARD_NAME=\"SparkFun XRP Controller (Red Vision)\""
    # 8MB (8 * 1024 * 1024)
    "MICROPY_HW_FLASH_STORAGE_BYTES=8388608"
)
```

Some board definitions do not have `#ifndef` wrappers in `mpconfigboard.h` for `MICROPY_HW_BOARD_NAME` and `MICROPY_HW_FLASH_STORAGE_BYTES`. They should be added if needed so the variant can build properly.

Then, the firmware can be built by adding `BOARD_VARIANT=<variant-name>` to the `make` command when building the MicroPython-OpenCV firmware.

# Adding New Platforms

Only support for the Raspberry Pi RP2350 has been figured out, so the all requirements for adding new platforms is not fully known yet. However, it should be along the lines of:

1. Create a valid toolchain file for the platform
    * See [rp2350.toolchain.cmake](src/opencv/platforms/rp2350.toolchain.cmake) for reference
    * This loosely follow's [OpenCV's platform definitions](https://github.com/opencv/opencv/tree/4.x/platforms)
2. Ensure OpenCV builds correctly
    * ```
      make -C micropython-opencv/src/opencv PLATFORM=<new-platform> --no-print-directory -j4
      ```
3. Create new board(s) for that platform
    * See [#Adding New Boards](#Adding-New-Boards)
4. Build MicroPython-OpenCV firmware for that board
    * ```
      make -C micropython/ports/rp2 BOARD=<board-name> USER_C_MODULES=micropython-opencv/src/micropython_opencv.cmake -j4
      ```

# Contributing

Found a bug? Is there a discrepancy between standard OpenCV and MicroPython-OpenCV? Have a feature request?

First, please see if there is an [existing issue](https://github.com/sparkfun/micropython-opencv/issues). If not, then please [open a new issue](https://github.com/sparkfun/micropython-opencv/issues/new) so we can discuss the topic!

Pull requests are welcome! Please keep the scope of your pull request focused (make separate ones if needed), and keep file changes limited to the scope of your pull request.

// C++ headers
#include "opencv2/core.hpp"

// C headers
extern "C" {
#include "py/runtime.h"
#include "ulab/code/ndarray.h"
} // extern "C"

using namespace cv;

// Conversion functions between Mat depth and ndarray_obj_t type
uint8_t mat_depth_to_ndarray_type(int depth);
int ndarray_type_to_mat_depth(uint8_t type);

// Converstion functions between Mat and ndarray_obj_t
ndarray_obj_t *mat_to_ndarray(Mat &mat);
Mat ndarray_to_mat(ndarray_obj_t *ndarray);

// Conversion functions between Mat and mp_obj_t. Abstracts away intermediate
// conversions to ndarray_obj_t
mp_obj_t mat_to_mp_obj(Mat &mat);
Mat mp_obj_to_mat(mp_obj_t obj);

// Conversion functions between Size and mp_obj_t
Size mp_obj_to_size(mp_obj_t obj);
Size2f mp_obj_to_size2f(mp_obj_t obj);

// Conversion functions between Point and mp_obj_t
Point mp_obj_to_point(mp_obj_t obj);
Point2f mp_obj_to_point2f(mp_obj_t obj);

// Conversion functions between Scalar and mp_obj_t
Scalar mp_obj_to_scalar(mp_obj_t obj);

// Conversion functions between contours (vector of vector of Point) and mp_obj_t
std::vector<std::vector<Point>> mp_obj_to_contours(mp_obj_t obj);

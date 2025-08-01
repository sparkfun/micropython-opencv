/*
 *------------------------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * 
 * Copyright (c) 2025 SparkFun Electronics
 *------------------------------------------------------------------------------
 * core.cpp
 * 
 * MicroPython wrappers for functions from the OpenCV core module.
 *------------------------------------------------------------------------------
 */

// C++ headers
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "convert.h"
#include "numpy.h"

// C headers
extern "C" {
#include "core.h"
#include "ndarray.h"
} // extern "C"

using namespace cv;

// The function below is a workaround for memory management issues between
// OpenCV and the MicroPython GC. OpenCV allocates some objects on the heap,
// whenever the first function that needs the objects happen to be called. That
// only happens from the user's code after the GC has been initialized, meaning
// they get allocated on the GC heap (see `__wrap_malloc()`). If a soft reset
// occurs, the GC gets reset and the memory locations get overwritten, but the
// same memory locations are still referenced for the objects, resulting in bad
// values and problems (crashes and freezes, `CV_Assert()` calls fail, etc.).
// 
// The solution here is to ensure those objects are allocated in the C heap
// instead of the GC heap. The function below calls various OpenCV functions
// that subsequently allocate the problematic objects. To ensure they are
// allocated on the C heap, this needs to happen before the GC is initialized
// (before `main()` is called), so __wrap_malloc() will use __real_malloc()
// instead of the GC.
// 
// The function below returns a dummy value that we use to initialize a global
// variable, ensuring it gets run before `main()` gets called. This also means
// it can be used as a general boot function for anything else that needs to
// happen before `main()` is called, such as setting the default Mat allocator.
bool upyOpenCVBoot() {
    try {
        // Initializes `CoreTLSData` on the C heap, see:
        // https://github.com/sparkfun/micropython-opencv/issues/13
        theRNG();

        // Initializes all image codecs on the C heap, see:
        // https://github.com/sparkfun/micropython-opencv/issues/17
        haveImageWriter(".bmp");

        // Initializes `StdMatAllocator` on the C heap, see:
        // https://github.com/sparkfun/micropython-opencv/issues/17
        // 
        // Alternatively, we could set the NumpyAllocator as the default Mat
        // allocator with `Mat::setDefaultAllocator(&GetNumpyAllocator())`,
        // however that actually causes some issues. For example, Canny()
        // creates a temporary 64-bit float Mat, which is not supported by ulab
        // NumPy and therefore fails with the NumpyAllocator. StdMatAllocator is
        // fine to use, because it calls `malloc()`, which we catch with
        // `__wrap_malloc()` to ensure the data is allocated on the GC heap
        Mat::getDefaultAllocator();

        return true;
    } catch (const Exception& e) {
        return false;
    }
}
volatile bool bootSuccess = upyOpenCVBoot();

mp_obj_t cv2_core_convertScaleAbs(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_dst, ARG_alpha, ARG_beta };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_alpha, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_beta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    mp_float_t alpha = args[ARG_alpha].u_obj == mp_const_none ? 1.0 : mp_obj_get_float(args[ARG_alpha].u_obj);
    mp_float_t beta = args[ARG_beta].u_obj == mp_const_none ? 0.0 : mp_obj_get_float(args[ARG_beta].u_obj);

    // Call the corresponding OpenCV function
    try {
        convertScaleAbs(src, dst, alpha, beta);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_core_inRange(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_lower, ARG_upper, ARG_dst };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_lower, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_upper, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Mat lower = mp_obj_to_mat(args[ARG_lower].u_obj);
    Mat upper = mp_obj_to_mat(args[ARG_upper].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);

    // Call the corresponding OpenCV function
    try {
        inRange(src, lower, upper, dst);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_core_minMaxLoc(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_mask };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_mask, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Mat mask = mp_obj_to_mat(args[ARG_mask].u_obj);

    double minVal, maxVal;
    Point minLoc, maxLoc;

    // Call the corresponding OpenCV function
    try {
        minMaxLoc(src, &minVal, &maxVal, &minLoc, &maxLoc, mask);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    mp_obj_t min_loc_tuple[2] = {
        mp_obj_new_float(minLoc.x),
        mp_obj_new_float(minLoc.y)
    };
    mp_obj_t max_loc_tuple[2] = {
        mp_obj_new_float(maxLoc.x),
        mp_obj_new_float(maxLoc.y)
    };
    mp_obj_t result_tuple[4] = {
        mp_obj_new_float(minVal),
        mp_obj_new_float(maxVal),
        mp_obj_new_tuple(2, min_loc_tuple),
        mp_obj_new_tuple(2, max_loc_tuple)
    };
    return mp_obj_new_tuple(4, result_tuple);
}

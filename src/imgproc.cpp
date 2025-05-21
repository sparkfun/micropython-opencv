// C++ headers
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "convert.h"
#include "numpy.h"

// C headers
extern "C" {
#include "imgproc.h"
#include "ulab/code/ndarray.h"
} // extern "C"

using namespace cv;

mp_obj_t cv2_imgproc_cvtColor(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_code, ARG_dst };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_code, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int code = args[ARG_code].u_int;
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);

    // Call the corresponding OpenCV function
    try {
        cvtColor(src, dst, code);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_dilate(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_kernel, ARG_dst, ARG_anchor, ARG_iterations, ARG_borderType, ARG_borderValue };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_kernel, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_iterations, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_CONSTANT } },
        { MP_QSTR_borderValue, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Mat kernel = mp_obj_to_mat(args[ARG_kernel].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    Point anchor;
    if(args[ARG_anchor].u_obj == mp_const_none)
        anchor = Point(-1, -1); // Default value
    else
        anchor = mp_obj_to_point(args[ARG_anchor].u_obj);
    int iterations = args[ARG_iterations].u_int;
    int borderType = args[ARG_borderType].u_int;
    Scalar borderValue;
    if(args[ARG_borderValue].u_obj == mp_const_none)
        borderValue = morphologyDefaultBorderValue(); // Default value
    else
        borderValue = mp_obj_to_scalar(args[ARG_borderValue].u_obj);

    // Call the corresponding OpenCV function
    try {
        dilate(src, dst, kernel, anchor, iterations, borderType, borderValue);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_erode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_kernel, ARG_dst, ARG_anchor, ARG_iterations, ARG_borderType, ARG_borderValue };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_kernel, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_iterations, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_CONSTANT } },
        { MP_QSTR_borderValue, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Mat kernel = mp_obj_to_mat(args[ARG_kernel].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    Point anchor;
    if(args[ARG_anchor].u_obj == mp_const_none)
        anchor = Point(-1, -1); // Default value
    else
        anchor = mp_obj_to_point(args[ARG_anchor].u_obj);
    int iterations = args[ARG_iterations].u_int;
    int borderType = args[ARG_borderType].u_int;
    Scalar borderValue;
    if(args[ARG_borderValue].u_obj == mp_const_none)
        borderValue = morphologyDefaultBorderValue(); // Default value
    else
        borderValue = mp_obj_to_scalar(args[ARG_borderValue].u_obj);

    // Call the corresponding OpenCV function
    try {
        erode(src, dst, kernel, anchor, iterations, borderType, borderValue);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_getStructuringElement(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_shape, ARG_ksize, ARG_anchor };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_shape, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_ksize, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    int shape = args[ARG_shape].u_int;
    Size ksize = mp_obj_to_size(args[ARG_ksize].u_obj);
    Point anchor;
    if(args[ARG_anchor].u_obj == mp_const_none)
        anchor = Point(-1, -1); // Default value
    else
        anchor = mp_obj_to_point(args[ARG_anchor].u_obj);

    // Instantiate result
    Mat kernel;

    // Call the corresponding OpenCV function
    try {
        kernel = getStructuringElement(shape, ksize, anchor);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(kernel);
}

mp_obj_t cv2_imgproc_morphologyEx(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_op, ARG_kernel, ARG_dst, ARG_anchor, ARG_iterations, ARG_borderType, ARG_borderValue };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_op, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_kernel, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_iterations, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_CONSTANT } },
        { MP_QSTR_borderValue, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int op = args[ARG_op].u_int;
    Mat kernel = mp_obj_to_mat(args[ARG_kernel].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    Point anchor;
    if(args[ARG_anchor].u_obj == mp_const_none)
        anchor = Point(-1, -1); // Default value
    else
        anchor = mp_obj_to_point(args[ARG_anchor].u_obj);
    int iterations = args[ARG_iterations].u_int;
    int borderType = args[ARG_borderType].u_int;
    Scalar borderValue;
    if(args[ARG_borderValue].u_obj == mp_const_none)
        borderValue = morphologyDefaultBorderValue(); // Default value
    else
        borderValue = mp_obj_to_scalar(args[ARG_borderValue].u_obj);

    // Call the corresponding OpenCV function
    try {
        morphologyEx(src, dst, op, kernel, anchor, iterations, borderType, borderValue);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

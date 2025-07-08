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

mp_obj_t cv2_imgproc_adaptiveThreshold(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_maxValue, ARG_adaptiveMethod, ARG_thresholdType, ARG_blockSize, ARG_C, ARG_dst };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_maxValue, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_adaptiveMethod, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_thresholdType, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_blockSize, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_C, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    mp_float_t maxValue = mp_obj_get_float(args[ARG_maxValue].u_obj);
    int adaptiveMethod = args[ARG_adaptiveMethod].u_int;
    int thresholdType = args[ARG_thresholdType].u_int;
    int blockSize = args[ARG_blockSize].u_int;
    mp_float_t C = mp_obj_get_float(args[ARG_C].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);

    // Call the corresponding OpenCV function
    try {
        adaptiveThreshold(src, dst, maxValue, adaptiveMethod, thresholdType, blockSize, C);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_arcLength(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_curve, ARG_closed };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_curve, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_closed, MP_ARG_REQUIRED | MP_ARG_BOOL, { .u_bool = false } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat curve = mp_obj_to_mat(args[ARG_curve].u_obj);
    bool closed = args[ARG_closed].u_bool;

    mp_float_t retval;

    // Call the corresponding OpenCV function
    try {
        retval = arcLength(curve, closed);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mp_obj_new_float(retval);
}

mp_obj_t cv2_imgproc_arrowedLine(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_pt1, ARG_pt2, ARG_color, ARG_thickness, ARG_line_type, ARG_shift, ARG_tipLength };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt1, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt2, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_line_type, MP_ARG_INT, { .u_int = 8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_tipLength, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Point pt1 = mp_obj_to_point(args[ARG_pt1].u_obj);
    Point pt2 = mp_obj_to_point(args[ARG_pt2].u_obj);
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int line_type = args[ARG_line_type].u_int;
    int shift = args[ARG_shift].u_int;
    mp_float_t tipLength;
    if(args[ARG_tipLength].u_obj == mp_const_none)
        tipLength = 0.1; // Default value
    else
        tipLength = mp_obj_get_float(args[ARG_tipLength].u_obj);

    // Call the corresponding OpenCV function
    try {
        arrowedLine(img, pt1, pt2, color, thickness, line_type, shift, tipLength);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_approxPolyDP(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_curve, ARG_epsilon, ARG_closed, ARG_approxCurve };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_curve, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_epsilon, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_closed, MP_ARG_BOOL, { .u_bool = false } },
        { MP_QSTR_approxCurve, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat curve = mp_obj_to_mat(args[ARG_curve].u_obj);
    double epsilon = mp_obj_get_float(args[ARG_epsilon].u_obj);
    bool closed = args[ARG_closed].u_bool;
    Mat approxCurve = mp_obj_to_mat(args[ARG_approxCurve].u_obj);

    // Call the corresponding OpenCV function
    try {
        approxPolyDP(curve, approxCurve, epsilon, closed);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(approxCurve);
}

mp_obj_t cv2_imgproc_approxPolyN(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_curve, ARG_nsides, ARG_approxCurve, ARG_epsilon_percentage, ARG_ensure_convex };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_curve, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_nsides, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_approxCurve, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_epsilon_percentage, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_ensure_convex, MP_ARG_BOOL, { .u_bool = true } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat curve = mp_obj_to_mat(args[ARG_curve].u_obj);
    int nsides = args[ARG_nsides].u_int;
    Mat approxCurve = mp_obj_to_mat(args[ARG_approxCurve].u_obj);
    mp_float_t epsilon_percentage = args[ARG_epsilon_percentage].u_obj == mp_const_none ? -1.0 : mp_obj_get_float(args[ARG_epsilon_percentage].u_obj);
    bool ensure_convex = args[ARG_ensure_convex].u_bool;

    // Call the corresponding OpenCV function
    try {
        approxPolyN(curve, approxCurve, nsides, epsilon_percentage, ensure_convex);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(approxCurve);
}

mp_obj_t cv2_imgproc_bilateralFilter(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_d, ARG_sigmaColor, ARG_sigmaSpace, ARG_dst, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_d, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_sigmaColor, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_sigmaSpace, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int d = args[ARG_d].u_int;
    mp_float_t sigmaColor = mp_obj_get_float(args[ARG_sigmaColor].u_obj);
    mp_float_t sigmaSpace = mp_obj_get_float(args[ARG_sigmaSpace].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        bilateralFilter(src, dst, d, sigmaColor, sigmaSpace, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_blur(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ksize, ARG_dst, ARG_anchor, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ksize, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Size ksize = mp_obj_to_size(args[ARG_ksize].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    Point anchor = args[ARG_anchor].u_obj == mp_const_none ? Point(-1,-1) : mp_obj_to_point(args[ARG_anchor].u_obj);
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        blur(src, dst, ksize, anchor, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_boundingRect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_array };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_array, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat array = mp_obj_to_mat(args[ARG_array].u_obj);

    Rect retval;

    // Call the corresponding OpenCV function
    try {
        retval = boundingRect(array);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result as a tuple
    mp_obj_t retval_tuple[4];
    retval_tuple[0] = mp_obj_new_int(retval.x);
    retval_tuple[1] = mp_obj_new_int(retval.y);
    retval_tuple[2] = mp_obj_new_int(retval.width);
    retval_tuple[3] = mp_obj_new_int(retval.height);
    mp_obj_t result = mp_obj_new_tuple(4, retval_tuple);
    return result;
}

mp_obj_t cv2_imgproc_boxFilter(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ddepth, ARG_ksize, ARG_dst, ARG_anchor, ARG_normalize, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ddepth, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_ksize, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_normalize, MP_ARG_BOOL, { .u_bool = true } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int ddepth = args[ARG_ddepth].u_int;
    Size ksize = mp_obj_to_size(args[ARG_ksize].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    Point anchor = args[ARG_anchor].u_obj == mp_const_none ? Point(-1,-1) : mp_obj_to_point(args[ARG_anchor].u_obj);
    bool normalize = args[ARG_normalize].u_bool;
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        boxFilter(src, dst, ddepth, ksize, anchor, normalize, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_boxPoints(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_box, ARG_points };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_box, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_points, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    mp_obj_tuple_t *box_tuple = (mp_obj_tuple_t*) args[ARG_box].u_obj;
    RotatedRect box;
    box.center = mp_obj_to_point2f(box_tuple->items[0]);
    box.size = mp_obj_to_size2f(box_tuple->items[1]);
    box.angle = mp_obj_get_float(box_tuple->items[2]);
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);

    // Call the corresponding OpenCV function
    try {
        boxPoints(box, points);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(points);
}

mp_obj_t cv2_imgproc_Canny(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_threshold1, ARG_threshold2, ARG_edges, ARG_apertureSize, ARG_L2gradient };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_threshold1, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_threshold2, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_edges, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_apertureSize, MP_ARG_INT, { .u_int = 3 } },
        { MP_QSTR_L2gradient, MP_ARG_BOOL, { .u_bool = false } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    int threshold1 = args[ARG_threshold1].u_int;
    int threshold2 = args[ARG_threshold2].u_int;
    Mat edges = mp_obj_to_mat(args[ARG_edges].u_obj);
    int apertureSize = args[ARG_apertureSize].u_int;
    bool L2gradient = args[ARG_L2gradient].u_bool;

    // Call the corresponding OpenCV function
    try {
        Canny(image, edges, threshold1, threshold2, apertureSize, L2gradient);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(edges);
}

mp_obj_t cv2_imgproc_connectedComponents(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_labels, ARG_connectivity, ARG_ltype };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_labels, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_connectivity, MP_ARG_INT, { .u_int = 8 } },
        { MP_QSTR_ltype, MP_ARG_INT, { .u_int = CV_16U } }, // Normally CV_32S, but ulab doesn't support 32-bit integers
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    Mat labels = mp_obj_to_mat(args[ARG_labels].u_obj);
    int connectivity = args[ARG_connectivity].u_int;
    int ltype = args[ARG_ltype].u_int;

    // Return value
    int retval = 0;

    // Call the corresponding OpenCV function
    try {
        retval = connectedComponents(image, labels, connectivity, ltype);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    mp_obj_t result[2];
    result[0] = mp_obj_new_int(retval);
    result[1] = mat_to_mp_obj(labels);
    return mp_obj_new_tuple(2, result);
}

// mp_obj_t cv2_imgproc_connectedComponentsWithStats(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
//     // Define the arguments
//     enum { ARG_image, ARG_labels, ARG_stats, ARG_centroids, ARG_connectivity, ARG_ltype };
//     static const mp_arg_t allowed_args[] = {
//         { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
//         { MP_QSTR_labels, MP_ARG_OBJ, { .u_obj = mp_const_none } },
//         { MP_QSTR_stats, MP_ARG_OBJ, { .u_obj = mp_const_none } },
//         { MP_QSTR_centroids, MP_ARG_OBJ, { .u_obj = mp_const_none } },
//         { MP_QSTR_connectivity, MP_ARG_INT, { .u_int = 8 } },
//         { MP_QSTR_ltype, MP_ARG_INT, { .u_int = CV_16U } }, // Normally CV_32S, but ulab doesn't support 32-bit integers
//     };

//     // Parse the arguments
//     mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
//     mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

//     // Convert arguments to required types
//     Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
//     Mat labels = mp_obj_to_mat(args[ARG_labels].u_obj);
//     Mat stats = mp_obj_to_mat(args[ARG_stats].u_obj);
//     Mat centroids = mp_obj_to_mat(args[ARG_centroids].u_obj);
//     int connectivity = args[ARG_connectivity].u_int;
//     int ltype = args[ARG_ltype].u_int;

//     // Return value
//     int retval = 0;

//     // Call the corresponding OpenCV function
//     try {
//         retval = connectedComponentsWithStats(image, labels, stats, centroids, connectivity, ltype);
//     } catch(Exception& e) {
//         mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
//     }

//     // Return the result
//     mp_obj_t result[4];
//     result[0] = mp_obj_new_int(retval);
//     result[1] = mat_to_mp_obj(labels);
//     result[2] = mat_to_mp_obj(stats);
//     result[3] = mat_to_mp_obj(centroids);
//     return mp_obj_new_tuple(4, result);
// }

mp_obj_t cv2_imgproc_contourArea(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_contour, ARG_oriented };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_contour, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_oriented, MP_ARG_BOOL, { .u_bool = false } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat contour = mp_obj_to_mat(args[ARG_contour].u_obj);
    bool oriented = args[ARG_oriented].u_bool;

    mp_float_t retval;

    // Call the corresponding OpenCV function
    try {
        retval = contourArea(contour, oriented);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mp_obj_new_float(retval);
}

mp_obj_t cv2_imgproc_convexHull(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_points, ARG_hull, ARG_clockwise, ARG_returnPoints };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_hull, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_clockwise, MP_ARG_BOOL, { .u_bool = false } },
        { MP_QSTR_returnPoints, MP_ARG_BOOL, { .u_bool = true } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);
    Mat hull; // TODO: Allow user input
    bool clockwise = args[ARG_clockwise].u_bool;
    bool returnPoints = args[ARG_returnPoints].u_bool;

    // Call the corresponding OpenCV function
    try {
        convexHull(points, hull, clockwise, returnPoints);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // If hull is 32S, convert it to float
    if (hull.type() == CV_32S) {
        Mat hullFloat;
        hull.convertTo(hullFloat, CV_32F);
        hull = hullFloat;
    }

    // Return the result
    return mat_to_mp_obj(hull);
}

mp_obj_t cv2_imgproc_convexityDefects(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_contour, ARG_convexhull, ARG_convexityDefects };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_contour, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_convexhull, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_convexityDefects, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat contour = mp_obj_to_mat(args[ARG_contour].u_obj);
    Mat convexhull = mp_obj_to_mat(args[ARG_convexhull].u_obj);
    Mat convexityDefects32S; // TODO: Allow user input

    // contour must be of type CV_32S
    Mat contour32S;
    contour.convertTo(contour32S, CV_32S);

    // convexhull must be of type CV_32S
    Mat convexhull32S;
    convexhull.convertTo(convexhull32S, CV_32S);

    // Call the corresponding OpenCV function
    try {
        cv::convexityDefects(contour32S, convexhull32S, convexityDefects32S);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Convert the convexityDefects32S to float
    Mat convexityDefects;
    convexityDefects.allocator = &GetNumpyAllocator();
    convexityDefects32S.convertTo(convexityDefects, CV_32F);

    // Return the result
    return mat_to_mp_obj(convexityDefects);
}

mp_obj_t cv2_imgproc_circle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_center, ARG_radius, ARG_color, ARG_thickness, ARG_lineType, ARG_shift };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_center, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_radius, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Point center = mp_obj_to_point(args[ARG_center].u_obj);
    int radius = args[ARG_radius].u_int;
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int lineType = args[ARG_lineType].u_int;
    int shift = args[ARG_shift].u_int;

    // Call the corresponding OpenCV function
    try {
        circle(img, center, radius, color, thickness, lineType, shift);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

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

mp_obj_t cv2_imgproc_drawContours(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_contours, ARG_contourIdx, ARG_color, ARG_thickness, ARG_lineType, ARG_hierarchy, ARG_maxLevel, ARG_offset };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_contours, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_contourIdx, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_hierarchy, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_maxLevel, MP_ARG_INT, { .u_int = INT_MAX } },
        { MP_QSTR_offset, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    std::vector<std::vector<Point>> contours = mp_obj_to_contours(args[ARG_contours].u_obj);
    int contourIdx = args[ARG_contourIdx].u_int;
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int lineType = args[ARG_lineType].u_int;
    Mat hierarchy = args[ARG_hierarchy].u_obj != mp_const_none ? mp_obj_to_mat(args[ARG_hierarchy].u_obj) : Mat();
    int maxLevel = args[ARG_maxLevel].u_int;
    Point offset = args[ARG_offset].u_obj != mp_const_none ? mp_obj_to_point(args[ARG_offset].u_obj) : Point();

    // Call the corresponding OpenCV function
    try {
        drawContours(image, contours, contourIdx, color, thickness, lineType, hierarchy, maxLevel, offset);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(image);
}

mp_obj_t cv2_imgproc_drawMarker(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_position, ARG_color, ARG_markerType, ARG_markerSize, ARG_thickness, ARG_line_type };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_position, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_markerType, MP_ARG_INT, { .u_int = MARKER_CROSS } },
        { MP_QSTR_markerSize, MP_ARG_INT, { .u_int = 20 } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_line_type, MP_ARG_INT, { .u_int = 8 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Point position = mp_obj_to_point(args[ARG_position].u_obj);
    int markerType = args[ARG_markerType].u_int;
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int markerSize = args[ARG_markerSize].u_int;
    int thickness = args[ARG_thickness].u_int;
    int line_type = args[ARG_line_type].u_int;

    // Call the corresponding OpenCV function
    try {
        drawMarker(img, position, color, markerType, markerSize, thickness, line_type);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_ellipse(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_center, ARG_axes, ARG_angle, ARG_startAngle, ARG_endAngle, ARG_color, ARG_thickness, ARG_lineType, ARG_shift };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_center, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_axes, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_angle, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_startAngle, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_endAngle, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Point center = mp_obj_to_point(args[ARG_center].u_obj);
    Size axes = mp_obj_to_size(args[ARG_axes].u_obj);
    int angle = args[ARG_angle].u_int;
    int startAngle = args[ARG_startAngle].u_int;
    int endAngle = args[ARG_endAngle].u_int;
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int lineType = args[ARG_lineType].u_int;
    int shift = args[ARG_shift].u_int;

    // Call the corresponding OpenCV function
    try {
        ellipse(img, center, axes, angle, startAngle, endAngle, color, thickness, lineType, shift);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
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

mp_obj_t cv2_imgproc_fillConvexPoly(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_points, ARG_color, ARG_lineType, ARG_shift };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int lineType = args[ARG_lineType].u_int;
    int shift = args[ARG_shift].u_int;

    // points must be of type CV_32S
    Mat points_32S;
    points.allocator = &GetNumpyAllocator();
    points.convertTo(points_32S, CV_32S);

    // Call the corresponding OpenCV function
    try {
        fillConvexPoly(img, points_32S, color, lineType, shift);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_fillPoly(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_pts, ARG_color, ARG_lineType, ARG_shift, ARG_offset };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pts, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_offset, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Mat pts = mp_obj_to_mat(args[ARG_pts].u_obj);
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int lineType = args[ARG_lineType].u_int;
    int shift = args[ARG_shift].u_int;
    Point offset;
    if(args[ARG_offset].u_obj == mp_const_none)
        offset = Point(); // Default value
    else
        offset = mp_obj_to_point(args[ARG_offset].u_obj);

    // points must be of type CV_32S
    Mat pts_32S;
    pts.allocator = &GetNumpyAllocator();
    pts.convertTo(pts_32S, CV_32S);

    // Call the corresponding OpenCV function
    try {
        fillPoly(img, pts_32S, color, lineType, shift, offset);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_filter2D(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ddepth, ARG_kernel, ARG_dst, ARG_anchor, ARG_delta, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ddepth, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_kernel, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_anchor, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_delta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int ddepth = args[ARG_ddepth].u_int;
    Mat kernel = mp_obj_to_mat(args[ARG_kernel].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    Point anchor;
    if(args[ARG_anchor].u_obj == mp_const_none)
        anchor = Point(-1,-1); // Default value
    else
        anchor = mp_obj_to_point(args[ARG_anchor].u_obj);
    mp_float_t delta = args[ARG_delta].u_obj == mp_const_none ? 0.0 : mp_obj_get_float(args[ARG_delta].u_obj);
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        filter2D(src, dst, ddepth, kernel, anchor, delta, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_findContours(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_mode, ARG_method, ARG_contours, ARG_hierarchy, ARG_offset };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_method, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_contours, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_hierarchy, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_offset, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    int mode = args[ARG_mode].u_int;
    int method = args[ARG_method].u_int;
    std::vector<std::vector<Point>> contours; // TODO: Allow user input
    std::vector<Vec4i> hierarchy; // TODO: Allow user input
    Point offset = args[ARG_offset].u_obj == mp_const_none ? Point() : mp_obj_to_point(args[ARG_offset].u_obj);

    // Call the corresponding OpenCV function
    try {
        findContours(image, contours, hierarchy, mode, method, offset);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Convert contours to a tuple of ndarray objects
    mp_obj_t contours_obj = mp_obj_new_tuple(contours.size(), NULL);
    mp_obj_tuple_t *contours_tuple = (mp_obj_tuple_t*) MP_OBJ_TO_PTR(contours_obj);
    
    for(size_t i = 0; i < contours.size(); i++) {
        Mat mat_contour(contours[i]);
        Mat mat_f32;
        mat_contour.convertTo(mat_f32, CV_32F);
        contours_tuple->items[i] = mat_to_mp_obj(mat_f32);
    }

    // Convert hierarchy to a Mat object
    Mat mat_hierarchy(hierarchy);

    // Return the result
    mp_obj_t result_tuple[2];
    result_tuple[0] = contours_tuple;
    Mat mat_16s;
    mat_hierarchy.convertTo(mat_16s, CV_16S);
    result_tuple[1] = mat_to_mp_obj(mat_16s);
    return mp_obj_new_tuple(2, result_tuple);
}

mp_obj_t cv2_imgproc_fitEllipse(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_points };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);

    RotatedRect ellipse;

    // Call the corresponding OpenCV function
    try {
        ellipse = fitEllipse(points);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Convert the result to a tuple
    mp_obj_t center[2];
    center[0] = mp_obj_new_float(ellipse.center.x);
    center[1] = mp_obj_new_float(ellipse.center.y);
    mp_obj_t size[2];
    size[0] = mp_obj_new_float(ellipse.size.width);
    size[1] = mp_obj_new_float(ellipse.size.height);
    mp_obj_t result[3];
    result[0] = mp_obj_new_tuple(2, center);
    result[1] = mp_obj_new_tuple(2, size);
    result[2] = mp_obj_new_float(ellipse.angle);
    return mp_obj_new_tuple(3, result);
}

mp_obj_t cv2_imgproc_fitLine(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_points, ARG_distType, ARG_param, ARG_reps, ARG_aeps, ARG_line };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_distType, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_param, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_reps, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_aeps, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_line, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);
    int distType = args[ARG_distType].u_int;
    mp_float_t param = mp_obj_get_float(args[ARG_param].u_obj);
    mp_float_t reps = mp_obj_get_float(args[ARG_reps].u_obj);
    mp_float_t aeps = mp_obj_get_float(args[ARG_aeps].u_obj);
    Mat line = mp_obj_to_mat(args[ARG_line].u_obj);

    // Call the corresponding OpenCV function
    try {
        fitLine(points, line, distType, param, reps, aeps);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(line);
}

mp_obj_t cv2_imgproc_GaussianBlur(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ksize, ARG_sigmaX, ARG_dst, ARG_sigmaY, ARG_borderType, ARG_hint };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ksize, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_sigmaX, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_sigmaY, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
        { MP_QSTR_hint, MP_ARG_INT, { .u_int = ALGO_HINT_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Size ksize = mp_obj_to_size(args[ARG_ksize].u_obj);
    mp_float_t sigmaX = mp_obj_get_float(args[ARG_sigmaX].u_obj);
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    mp_float_t sigmaY = args[ARG_sigmaY].u_obj == mp_const_none ? sigmaX : mp_obj_get_float(args[ARG_sigmaY].u_obj);
    int borderType = args[ARG_borderType].u_int;
    AlgorithmHint hint = (AlgorithmHint) args[ARG_hint].u_int;

    // Call the corresponding OpenCV function
    try {
        GaussianBlur(src, dst, ksize, sigmaX, sigmaY, borderType, hint);
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

mp_obj_t cv2_imgproc_HoughCircles(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_method, ARG_dp, ARG_minDist, ARG_circles, ARG_param1, ARG_param2, ARG_minRadius, ARG_maxRadius };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_method, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dp, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_minDist, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_circles, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_param1, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_param2, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_minRadius, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_maxRadius, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    int method = args[ARG_method].u_int;
    mp_float_t dp = mp_obj_get_float(args[ARG_dp].u_obj);
    mp_float_t minDist = mp_obj_get_float(args[ARG_minDist].u_obj);
    Mat circles = mp_obj_to_mat(args[ARG_circles].u_obj);
    float param1;
    if(args[ARG_param1].u_obj == mp_const_none)
        param1 = 100; // Default value
    else
        param1 = mp_obj_get_float(args[ARG_param1].u_obj);
    float param2;
    if(args[ARG_param2].u_obj == mp_const_none)
        param2 = 100; // Default value
    else
        param2 = mp_obj_get_float(args[ARG_param2].u_obj);
    int minRadius = args[ARG_minRadius].u_int;
    int maxRadius = args[ARG_maxRadius].u_int;

    // Call the corresponding OpenCV function
    try {
        HoughCircles(image, circles, method, dp, minDist, param1, param2, minRadius, maxRadius);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(circles);
}

mp_obj_t cv2_imgproc_HoughCirclesWithAccumulator(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_method, ARG_dp, ARG_minDist, ARG_circles, ARG_param1, ARG_param2, ARG_minRadius, ARG_maxRadius };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_method, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dp, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_minDist, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_circles, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_param1, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_param2, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_minRadius, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_maxRadius, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    int method = args[ARG_method].u_int;
    mp_float_t dp = mp_obj_get_float(args[ARG_dp].u_obj);
    mp_float_t minDist = mp_obj_get_float(args[ARG_minDist].u_obj);
    Mat circles = mp_obj_to_mat(args[ARG_circles].u_obj);
    float param1;
    if(args[ARG_param1].u_obj == mp_const_none)
        param1 = 100; // Default value
    else
        param1 = mp_obj_get_float(args[ARG_param1].u_obj);
    float param2;
    if(args[ARG_param2].u_obj == mp_const_none)
        param2 = 100; // Default value
    else
        param2 = mp_obj_get_float(args[ARG_param2].u_obj);
    int minRadius = args[ARG_minRadius].u_int;
    int maxRadius = args[ARG_maxRadius].u_int;

    // Vector to hold the circles and votes
    std::vector<Vec4f> circles_acc;

    // Call the corresponding OpenCV function
    try {
        HoughCircles(image, circles_acc, method, dp, minDist, param1, param2, minRadius, maxRadius);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Copy the vector of circles and votes to output circles object
    Mat(circles_acc).copyTo(circles);

    // Return the result
    return mat_to_mp_obj(circles);
}

mp_obj_t cv2_imgproc_HoughLines(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_rho, ARG_theta, ARG_threshold, ARG_lines, ARG_srn, ARG_stn, ARG_min_theta, ARG_max_theta, ARG_use_edgeval };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_rho, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_theta, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_threshold, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 100 } },
        { MP_QSTR_lines, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_srn, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_stn, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_min_theta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_max_theta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_use_edgeval, MP_ARG_BOOL, { .u_bool = false } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    mp_float_t rho;
    if(args[ARG_rho].u_obj == mp_const_none)
        rho = 1; // Default value
    else
        rho = mp_obj_get_float(args[ARG_rho].u_obj);
    mp_float_t theta;
    if(args[ARG_theta].u_obj == mp_const_none)
        theta = CV_PI / 180; // Default value
    else
        theta = mp_obj_get_float(args[ARG_theta].u_obj);
    int threshold = args[ARG_threshold].u_int;
    Mat lines = mp_obj_to_mat(args[ARG_lines].u_obj);
    mp_float_t srn;
    if(args[ARG_srn].u_obj == mp_const_none)
        srn = 0; // Default value
    else
        srn = mp_obj_get_float(args[ARG_srn].u_obj);
    mp_float_t stn;
    if(args[ARG_stn].u_obj == mp_const_none)
        stn = 0; // Default value
    else
        stn = mp_obj_get_float(args[ARG_stn].u_obj);
    mp_float_t min_theta;
    if(args[ARG_min_theta].u_obj == mp_const_none)
        min_theta = 0; // Default value
    else
        min_theta = mp_obj_get_float(args[ARG_min_theta].u_obj);
    mp_float_t max_theta;
    if(args[ARG_max_theta].u_obj == mp_const_none)
        max_theta = CV_PI; // Default value
    else
        max_theta = mp_obj_get_float(args[ARG_max_theta].u_obj);
    bool use_edgeval = args[ARG_use_edgeval].u_bool;

    // Call the corresponding OpenCV function
    try {
        HoughLines(image, lines, rho, theta, threshold, srn, stn, min_theta, max_theta, use_edgeval);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(lines);
}

mp_obj_t cv2_imgproc_HoughLinesP(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_rho, ARG_theta, ARG_threshold, ARG_lines, ARG_minLineLength, ARG_maxLineGap };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_rho, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_theta, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_threshold, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 100 } },
        { MP_QSTR_lines, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_minLineLength, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_maxLineGap, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    mp_float_t rho = mp_obj_get_float(args[ARG_rho].u_obj);
    mp_float_t theta = mp_obj_get_float(args[ARG_theta].u_obj);
    int threshold = args[ARG_threshold].u_int;
    Mat lines32S; // TODO: Allow user input
    mp_float_t minLineLength;
    if(args[ARG_minLineLength].u_obj == mp_const_none)
        minLineLength = 0; // Default value
    else
        minLineLength = mp_obj_get_float(args[ARG_minLineLength].u_obj);
    mp_float_t maxLineGap;
    if(args[ARG_maxLineGap].u_obj == mp_const_none)
        maxLineGap = 0; // Default value
    else
        maxLineGap = mp_obj_get_float(args[ARG_maxLineGap].u_obj);  

    // Call the corresponding OpenCV function
    try {
        HoughLinesP(image, lines32S, rho, theta, threshold, minLineLength, maxLineGap);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Convert lines to float
    Mat lines;
    lines.allocator = &GetNumpyAllocator();
    lines32S.convertTo(lines, CV_32F);

    // Return the result
    return mat_to_mp_obj(lines);
}

mp_obj_t cv2_imgproc_HoughLinesWithAccumulator(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_image, ARG_rho, ARG_theta, ARG_threshold, ARG_lines, ARG_srn, ARG_stn, ARG_min_theta, ARG_max_theta };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_image, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_rho, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_theta, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_threshold, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 100 } },
        { MP_QSTR_lines, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_srn, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_stn, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_min_theta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_max_theta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat image = mp_obj_to_mat(args[ARG_image].u_obj);
    mp_float_t rho;
    if(args[ARG_rho].u_obj == mp_const_none)
        rho = 1; // Default value
    else
        rho = mp_obj_get_float(args[ARG_rho].u_obj);
    mp_float_t theta;
    if(args[ARG_theta].u_obj == mp_const_none)
        theta = CV_PI / 180; // Default value
    else
        theta = mp_obj_get_float(args[ARG_theta].u_obj);
    int threshold = args[ARG_threshold].u_int;
    Mat lines = mp_obj_to_mat(args[ARG_lines].u_obj);
    mp_float_t srn;
    if(args[ARG_srn].u_obj == mp_const_none)
        srn = 0; // Default value
    else
        srn = mp_obj_get_float(args[ARG_srn].u_obj);
    mp_float_t stn;
    if(args[ARG_stn].u_obj == mp_const_none)
        stn = 0; // Default value
    else
        stn = mp_obj_get_float(args[ARG_stn].u_obj);
    mp_float_t min_theta;
    if(args[ARG_min_theta].u_obj == mp_const_none)
        min_theta = 0; // Default value
    else
        min_theta = mp_obj_get_float(args[ARG_min_theta].u_obj);
    mp_float_t max_theta;
    if(args[ARG_max_theta].u_obj == mp_const_none)
        max_theta = CV_PI; // Default value
    else
        max_theta = mp_obj_get_float(args[ARG_max_theta].u_obj);

    // Vector to hold the lines and votes
    std::vector<Vec3f> lines_acc;

    // Call the corresponding OpenCV function
    try {
        HoughLines(image, lines_acc, rho, theta, threshold, srn, stn, min_theta, max_theta);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Copy the vector of lines and votes to output lines object
    Mat(lines_acc).copyTo(lines);

    // Return the result
    return mat_to_mp_obj(lines);
}

mp_obj_t cv2_imgproc_isContourConvex(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_contour };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_contour, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat contour = mp_obj_to_mat(args[ARG_contour].u_obj);

    // Call the corresponding OpenCV function
    bool isConvex;
    try {
        isConvex = isContourConvex(contour);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mp_obj_new_bool(isConvex);
}

mp_obj_t cv2_imgproc_Laplacian(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ddepth, ARG_dst, ARG_ksize, ARG_scale, ARG_delta, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ddepth, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_ksize, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_scale, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_delta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int ddepth = args[ARG_ddepth].u_int;
    int ksize = args[ARG_ksize].u_int;
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    mp_float_t scale = args[ARG_scale].u_obj == mp_const_none ? 1.0 : mp_obj_get_float(args[ARG_scale].u_obj);
    mp_float_t delta = args[ARG_delta].u_obj == mp_const_none ? 0.0 : mp_obj_get_float(args[ARG_delta].u_obj);
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        Laplacian(src, dst, ddepth, ksize, scale, delta, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_line(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_pt1, ARG_pt2, ARG_color, ARG_thickness, ARG_lineType, ARG_shift };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt1, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt2, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Point pt1 = mp_obj_to_point(args[ARG_pt1].u_obj);
    Point pt2 = mp_obj_to_point(args[ARG_pt2].u_obj);
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int lineType = args[ARG_lineType].u_int;
    int shift = args[ARG_shift].u_int;

    // Call the corresponding OpenCV function
    try {
        line(img, pt1, pt2, color, thickness, lineType, shift);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_matchShapes(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_contour1, ARG_contour2, ARG_method, ARG_parameter };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_contour1, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_contour2, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_method, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_parameter, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat contour1 = mp_obj_to_mat(args[ARG_contour1].u_obj);
    Mat contour2 = mp_obj_to_mat(args[ARG_contour2].u_obj);
    int method = args[ARG_method].u_int;
    mp_float_t parameter = mp_obj_get_float(args[ARG_parameter].u_obj);

    mp_float_t retval;

    // Call the corresponding OpenCV function
    try {
        retval = matchShapes(contour1, contour2, method, parameter);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mp_obj_new_float(retval);
}

mp_obj_t cv2_imgproc_matchTemplate(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_templ, ARG_method, ARG_result, ARG_mask };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_templ, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_method, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = TM_CCOEFF_NORMED } },
        { MP_QSTR_result, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_mask, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Mat templ = mp_obj_to_mat(args[ARG_templ].u_obj);
    int method = args[ARG_method].u_int;
    Mat result = mp_obj_to_mat(args[ARG_result].u_obj);
    Mat mask = mp_obj_to_mat(args[ARG_mask].u_obj);

    // Call the corresponding OpenCV function
    try {
        matchTemplate(img, templ, result, method, mask);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(result);
}

mp_obj_t cv2_imgproc_medianBlur(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ksize, ARG_dst };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ksize, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int ksize = args[ARG_ksize].u_int;
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);

    // Call the corresponding OpenCV function
    try {
        medianBlur(src, dst, ksize);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_minAreaRect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_points };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);

    RotatedRect retval;

    // Call the corresponding OpenCV function
    try {
        retval = minAreaRect(points);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result as a tuple
    mp_obj_t center_tuple[2];
    center_tuple[0] = mp_obj_new_float(retval.center.x);
    center_tuple[1] = mp_obj_new_float(retval.center.y);
    mp_obj_t size_tuple[2];
    size_tuple[0] = mp_obj_new_float(retval.size.width);
    size_tuple[1] = mp_obj_new_float(retval.size.height);
    mp_obj_t result_tuple[3];
    result_tuple[0] = mp_obj_new_tuple(2, center_tuple);
    result_tuple[1] = mp_obj_new_tuple(2, size_tuple);
    result_tuple[2] = mp_obj_new_float(retval.angle);
    return mp_obj_new_tuple(3, result_tuple);
}

mp_obj_t cv2_imgproc_minEnclosingCircle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_points };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);

    Point2f center;
    float radius;

    // Call the corresponding OpenCV function
    try {
        minEnclosingCircle(points, center, radius);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result as a tuple
    mp_obj_t center_tuple[2];
    center_tuple[0] = mp_obj_new_float(center.x);
    center_tuple[1] = mp_obj_new_float(center.y);
    mp_obj_t result_tuple[3];
    result_tuple[0] = mp_obj_new_tuple(2, center_tuple);
    result_tuple[1] = mp_obj_new_float(radius);
    return mp_obj_new_tuple(2, result_tuple);
}

mp_obj_t cv2_imgproc_minEnclosingTriangle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_points, ARG_triangle };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_points, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_triangle, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat points = mp_obj_to_mat(args[ARG_points].u_obj);
    Mat triangle = mp_obj_to_mat(args[ARG_triangle].u_obj);

    mp_float_t retval;

    // Call the corresponding OpenCV function
    try {
        retval = minEnclosingTriangle(points, triangle);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result as a tuple
    mp_obj_t result_tuple[2];
    result_tuple[0] = mp_obj_new_float(retval);
    result_tuple[1] = mat_to_mp_obj(triangle);
    return mp_obj_new_tuple(2, result_tuple);
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

mp_obj_t cv2_imgproc_moments(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_binary };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_binary, MP_ARG_BOOL, { .u_bool = false } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    bool binary = args[ARG_binary].u_bool;
    Moments moments;

    // Call the corresponding OpenCV function
    try {
        moments = cv::moments(src, binary);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }
    
    // Create a dictionary to hold the moments
    mp_obj_t moments_dict = mp_obj_new_dict(0);
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m00), mp_obj_new_float(moments.m00));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m10), mp_obj_new_float(moments.m10));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m01), mp_obj_new_float(moments.m01));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m20), mp_obj_new_float(moments.m20));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m11), mp_obj_new_float(moments.m11));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m02), mp_obj_new_float(moments.m02));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m30), mp_obj_new_float(moments.m30));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m21), mp_obj_new_float(moments.m21));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m12), mp_obj_new_float(moments.m12));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_m03), mp_obj_new_float(moments.m03));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu20), mp_obj_new_float(moments.mu20));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu11), mp_obj_new_float(moments.mu11));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu02), mp_obj_new_float(moments.mu02));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu30), mp_obj_new_float(moments.mu30));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu21), mp_obj_new_float(moments.mu21));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu12), mp_obj_new_float(moments.mu12));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_mu03), mp_obj_new_float(moments.mu03));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu20), mp_obj_new_float(moments.nu20));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu11), mp_obj_new_float(moments.nu11));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu02), mp_obj_new_float(moments.nu02));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu30), mp_obj_new_float(moments.nu30));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu21), mp_obj_new_float(moments.nu21));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu12), mp_obj_new_float(moments.nu12));
    mp_obj_dict_store(moments_dict, MP_OBJ_NEW_QSTR(MP_QSTR_nu03), mp_obj_new_float(moments.nu03));

    // Return the moments dictionary
    return moments_dict;
}

mp_obj_t cv2_imgproc_pointPolygonTest(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_contour, ARG_pt, ARG_measureDist };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_contour, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_measureDist, MP_ARG_REQUIRED | MP_ARG_BOOL, { .u_bool = false } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat contour = mp_obj_to_mat(args[ARG_contour].u_obj);
    Point pt = mp_obj_to_point(args[ARG_pt].u_obj);
    bool measureDist = args[ARG_measureDist].u_bool;

    mp_float_t retval;

    // Call the corresponding OpenCV function
    try {
        retval = pointPolygonTest(contour, pt, measureDist);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mp_obj_new_float(retval);
}

mp_obj_t cv2_imgproc_putText(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_text, ARG_org, ARG_fontFace, ARG_fontScale, ARG_color, ARG_thickness, ARG_lineType, ARG_bottomLeftOrigin };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_text, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_org, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_fontFace, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = FONT_HERSHEY_SIMPLEX } },
        { MP_QSTR_fontScale, MP_ARG_REQUIRED, { .u_obj = mp_const_none } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_bottomLeftOrigin, MP_ARG_BOOL, { .u_bool = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    size_t len;
    const char *text_str = mp_obj_str_get_data(args[ARG_text].u_obj, &len);
    String text(text_str, len);
    Point org = mp_obj_to_point(args[ARG_org].u_obj);
    int fontFace = args[ARG_fontFace].u_int;
    mp_float_t fontScale = mp_obj_get_float(args[ARG_fontScale].u_obj);
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int lineType = args[ARG_lineType].u_int;
    bool bottomLeftOrigin = args[ARG_bottomLeftOrigin].u_bool;

    // Call the corresponding OpenCV function
    try {
        putText(img, text, org, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_rectangle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_img, ARG_pt1, ARG_pt2, ARG_color, ARG_thickness, ARG_lineType, ARG_shift };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt1, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_pt2, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
        { MP_QSTR_shift, MP_ARG_INT, { .u_int = 0 } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    Point pt1 = mp_obj_to_point(args[ARG_pt1].u_obj);
    Point pt2 = mp_obj_to_point(args[ARG_pt2].u_obj);
    Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
    int thickness = args[ARG_thickness].u_int;
    int lineType = args[ARG_lineType].u_int;
    int shift = args[ARG_shift].u_int;

    // Call the corresponding OpenCV function
    try {
        rectangle(img, pt1, pt2, color, thickness, lineType, shift);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgproc_Scharr(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ddepth, ARG_dx, ARG_dy, ARG_dst, ARG_scale, ARG_delta, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ddepth, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dx, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dy, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_scale, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_delta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int ddepth = args[ARG_ddepth].u_int;
    int dx = args[ARG_dx].u_int;
    int dy = args[ARG_dy].u_int;
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    mp_float_t scale = args[ARG_scale].u_obj == mp_const_none ? 1.0 : mp_obj_get_float(args[ARG_scale].u_obj);
    mp_float_t delta = args[ARG_delta].u_obj == mp_const_none ? 0.0 : mp_obj_get_float(args[ARG_delta].u_obj);
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        Scharr(src, dst, ddepth, dx, dy, scale, delta, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_Sobel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_ddepth, ARG_dx, ARG_dy, ARG_dst, ARG_ksize, ARG_scale, ARG_delta, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_ddepth, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dx, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dy, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_ksize, MP_ARG_INT, { .u_int = 3 } },
        { MP_QSTR_scale, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_delta, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    int ddepth = args[ARG_ddepth].u_int;
    int dx = args[ARG_dx].u_int;
    int dy = args[ARG_dy].u_int;
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);
    int ksize = args[ARG_ksize].u_int;
    mp_float_t scale = args[ARG_scale].u_obj == mp_const_none ? 1.0 : mp_obj_get_float(args[ARG_scale].u_obj);
    mp_float_t delta = args[ARG_delta].u_obj == mp_const_none ? 0.0 : mp_obj_get_float(args[ARG_delta].u_obj);
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        Sobel(src, dst, ddepth, dx, dy, ksize, scale, delta, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

mp_obj_t cv2_imgproc_spatialGradient(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_dx, ARG_dy, ARG_ksize, ARG_borderType };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_dx, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_dy, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_ksize, MP_ARG_INT, { .u_int = 3 } },
        { MP_QSTR_borderType, MP_ARG_INT, { .u_int = BORDER_DEFAULT } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    Mat dx = mp_obj_to_mat(args[ARG_dx].u_obj);
    Mat dy = mp_obj_to_mat(args[ARG_dy].u_obj);
    int ksize = args[ARG_ksize].u_int;
    int borderType = args[ARG_borderType].u_int;

    // Call the corresponding OpenCV function
    try {
        spatialGradient(src, dx, dy, ksize, borderType);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    mp_obj_t result[2];
    result[0] = mat_to_mp_obj(dx);
    result[1] = mat_to_mp_obj(dy);
    return mp_obj_new_tuple(2, result);
}

mp_obj_t cv2_imgproc_threshold(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_src, ARG_thresh, ARG_maxval, ARG_type, ARG_dst };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_thresh, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_maxval, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_type, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = THRESH_BINARY } },
        { MP_QSTR_dst, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    Mat src = mp_obj_to_mat(args[ARG_src].u_obj);
    mp_float_t thresh = mp_obj_get_float(args[ARG_thresh].u_obj);
    mp_float_t maxval = mp_obj_get_float(args[ARG_maxval].u_obj);
    int type = args[ARG_type].u_int;
    Mat dst = mp_obj_to_mat(args[ARG_dst].u_obj);

    // Call the corresponding OpenCV function
    try {
        threshold(src, dst, thresh, maxval, type);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(dst);
}

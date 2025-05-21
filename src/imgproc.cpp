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

// mp_obj_t cv2_imgproc_putText(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
//     // Define the arguments
//     enum { ARG_img, ARG_text, ARG_org, ARG_fontFace, ARG_fontScale, ARG_color, ARG_thickness, ARG_lineType, ARG_bottomLeftOrigin };
//     static const mp_arg_t allowed_args[] = {
//         { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
//         { MP_QSTR_text, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
//         { MP_QSTR_org, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
//         { MP_QSTR_fontFace, MP_ARG_REQUIRED | MP_ARG_INT, { .u_int = FONT_HERSHEY_SIMPLEX } },
//         { MP_QSTR_fontScale, MP_ARG_REQUIRED, { .u_obj = mp_const_none } },
//         { MP_QSTR_color, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
//         { MP_QSTR_thickness, MP_ARG_INT, { .u_int = 1 } },
//         { MP_QSTR_lineType, MP_ARG_INT, { .u_int = LINE_8 } },
//         { MP_QSTR_bottomLeftOrigin, MP_ARG_BOOL, { .u_bool = 0 } },
//     };

//     // Parse the arguments
//     mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
//     mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

//     // Convert arguments to required types
//     Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
//     size_t len;
//     const char *text_str = mp_obj_str_get_data(args[ARG_text].u_obj, &len);
//     String text(text_str, len);
//     Point org = mp_obj_to_point(args[ARG_org].u_obj);
//     int fontFace = args[ARG_fontFace].u_int;
//     mp_float_t fontScale = mp_obj_get_float(args[ARG_fontScale].u_obj);
//     Scalar color = mp_obj_to_scalar(args[ARG_color].u_obj);
//     int thickness = args[ARG_thickness].u_int;
//     int lineType = args[ARG_lineType].u_int;
//     bool bottomLeftOrigin = args[ARG_bottomLeftOrigin].u_bool;

//     // Call the corresponding OpenCV function
//     try {
//         putText(img, text, org, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);
//     } catch(Exception& e) {
//         mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
//     }

//     // Return the result
//     return mat_to_mp_obj(img);
// }

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

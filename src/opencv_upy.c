#include "core.h"
#include "imgproc.h"

////////////////////////////////////////////////////////////////////////////////
// Python references to OpenCV functions
////////////////////////////////////////////////////////////////////////////////

// OpenCV core module
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_core_inRange_obj, 3, cv2_core_inRange);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_core_max_obj, 2, cv2_core_max);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_core_min_obj, 2, cv2_core_min);

// OpenCV imgproc module
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_arrowedLine_obj, 4, cv2_imgproc_arrowedLine);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_circle_obj, 4, cv2_imgproc_circle);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_cvtColor_obj, 2, cv2_imgproc_cvtColor);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_dilate_obj, 2, cv2_imgproc_dilate);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_drawMarker_obj, 3, cv2_imgproc_drawMarker);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_ellipse_obj, 7, cv2_imgproc_ellipse);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_erode_obj, 2, cv2_imgproc_erode);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_fillConvexPoly_obj, 3, cv2_imgproc_fillConvexPoly);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_fillPoly_obj, 3, cv2_imgproc_fillPoly);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_getStructuringElement_obj, 2, cv2_imgproc_getStructuringElement);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_line_obj, 4, cv2_imgproc_line);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_morphologyEx_obj, 3, cv2_imgproc_morphologyEx);
// static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_putText_obj, 6, cv2_imgproc_putText);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_rectangle_obj, 4, cv2_imgproc_rectangle);

////////////////////////////////////////////////////////////////////////////////
// Module attributes
////////////////////////////////////////////////////////////////////////////////
static const mp_rom_map_elem_t cv2_module_globals_table[] = {
    ////////////////////////////////////////////////////////////////////////////
    // Module name
    ////////////////////////////////////////////////////////////////////////////
    
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_cv2) },

    ////////////////////////////////////////////////////////////////////////////
    // Constants
    ////////////////////////////////////////////////////////////////////////////
    
    // These constants are defined by in OpenCV's header files, however we can't
    // include them here because it's C++ and this is C, so we have to redefine
    // them here. Only a subset of the most common conversions are included.

    // Border types, from opencv2/core/base.hpp
    { MP_ROM_QSTR(MP_QSTR_BORDER_CONSTANT), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_REPLICATE), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_REFLECT), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_WRAP), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_REFLECT_101), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_REFLECT101), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_DEFAULT), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_TRANSPARENT), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_BORDER_ISOLATED), MP_ROM_INT(16) },

    // Morphology operation types, from opencv2/imgproc.hpp
    { MP_ROM_QSTR(MP_QSTR_MORPH_ERODE), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_DILATE), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_OPEN), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_CLOSE), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_GRADIENT), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_TOPHAT), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_BLACKHAT), MP_ROM_INT(6) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_HITMISS), MP_ROM_INT(7) },

    // Morphology shapes, from opencv2/imgproc.hpp
    { MP_ROM_QSTR(MP_QSTR_MORPH_RECT), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_CROSS), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_MORPH_ELLIPSE), MP_ROM_INT(2) },

    // Color conversion codes, from opencv2/imgproc.hpp
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2BGRA), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2RGBA), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2BGR), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2RGB), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2RGBA), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2BGRA), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2BGR), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2RGB), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2RGB), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2BGR), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2RGBA), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2BGRA), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2GRAY), MP_ROM_INT(6) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2GRAY), MP_ROM_INT(7) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2BGR), MP_ROM_INT(8) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2RGB), MP_ROM_INT(8) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2BGRA), MP_ROM_INT(9) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2RGBA), MP_ROM_INT(9) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2GRAY), MP_ROM_INT(10) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2GRAY), MP_ROM_INT(11) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2BGR565), MP_ROM_INT(12) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2BGR565), MP_ROM_INT(13) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652BGR), MP_ROM_INT(14) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652RGB), MP_ROM_INT(15) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2BGR565), MP_ROM_INT(16) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2BGR565), MP_ROM_INT(17) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652BGRA), MP_ROM_INT(18) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652RGBA), MP_ROM_INT(19) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2BGR565), MP_ROM_INT(20) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652GRAY), MP_ROM_INT(21) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2YCrCb), MP_ROM_INT(36) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2YCrCb), MP_ROM_INT(37) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_YCrCb2BGR), MP_ROM_INT(38) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_YCrCb2RGB), MP_ROM_INT(39) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2HSV), MP_ROM_INT(40) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2HSV), MP_ROM_INT(41) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_HSV2BGR), MP_ROM_INT(54) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_HSV2RGB), MP_ROM_INT(55) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerBG2BGR), MP_ROM_INT(46) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGB2BGR), MP_ROM_INT(47) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerRG2BGR), MP_ROM_INT(48) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGR2BGR), MP_ROM_INT(49) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerRG2RGB), MP_ROM_INT(46) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGR2RGB), MP_ROM_INT(47) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerBG2RGB), MP_ROM_INT(48) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGB2RGB), MP_ROM_INT(49) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerBG2GRAY), MP_ROM_INT(86) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGB2GRAY), MP_ROM_INT(87) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerRG2GRAY), MP_ROM_INT(88) },
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGR2GRAY), MP_ROM_INT(89) },

    // Line types, from opencv2/imgproc.hpp
    { MP_ROM_QSTR(MP_QSTR_FILLED), MP_ROM_INT(-1) },
    { MP_ROM_QSTR(MP_QSTR_LINE_4), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_LINE_8), MP_ROM_INT(8) },
    { MP_ROM_QSTR(MP_QSTR_LINE_AA), MP_ROM_INT(16) },

    // Fonts, from opencv2/imgproc.hpp
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_SIMPLEX), MP_ROM_INT(0) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_PLAIN), MP_ROM_INT(1) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_DUPLEX), MP_ROM_INT(2) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_COMPLEX), MP_ROM_INT(3) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_TRIPLEX), MP_ROM_INT(4) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_COMPLEX_SMALL), MP_ROM_INT(5) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_SCRIPT_SIMPLEX), MP_ROM_INT(6) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_SCRIPT_COMPLEX), MP_ROM_INT(7) },
    // { MP_ROM_QSTR(MP_QSTR_FONT_ITALIC), MP_ROM_INT(16) },

    // Marker types, from opencv2/imgproc.hpp
    { MP_ROM_QSTR(MP_QSTR_MARKER_CROSS), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_MARKER_TILTED_CROSS), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_MARKER_STAR), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_MARKER_DIAMOND), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_MARKER_SQUARE), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_MARKER_TRIANGLE_UP), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_MARKER_TRIANGLE_DOWN), MP_ROM_INT(6) },
    
    ////////////////////////////////////////////////////////////////////////////
    // OpenCV core functions
    ////////////////////////////////////////////////////////////////////////////
    
    { MP_ROM_QSTR(MP_QSTR_inRange), MP_ROM_PTR(&cv2_core_inRange_obj) },
    { MP_ROM_QSTR(MP_QSTR_max), MP_ROM_PTR(&cv2_core_max_obj) },
    { MP_ROM_QSTR(MP_QSTR_min), MP_ROM_PTR(&cv2_core_min_obj) },
    
    ////////////////////////////////////////////////////////////////////////////
    // OpenCV imgproc functions
    ////////////////////////////////////////////////////////////////////////////
    
    { MP_ROM_QSTR(MP_QSTR_arrowedLine), MP_ROM_PTR(&cv2_imgproc_arrowedLine_obj) },
    { MP_ROM_QSTR(MP_QSTR_circle), MP_ROM_PTR(&cv2_imgproc_circle_obj) },
    { MP_ROM_QSTR(MP_QSTR_cvtColor), MP_ROM_PTR(&cv2_imgproc_cvtColor_obj) },
    { MP_ROM_QSTR(MP_QSTR_dilate), MP_ROM_PTR(&cv2_imgproc_dilate_obj) },
    { MP_ROM_QSTR(MP_QSTR_drawMarker), MP_ROM_PTR(&cv2_imgproc_drawMarker_obj) },
    { MP_ROM_QSTR(MP_QSTR_ellipse), MP_ROM_PTR(&cv2_imgproc_ellipse_obj) },
    { MP_ROM_QSTR(MP_QSTR_erode), MP_ROM_PTR(&cv2_imgproc_erode_obj) },
    { MP_ROM_QSTR(MP_QSTR_fillConvexPoly), MP_ROM_PTR(&cv2_imgproc_fillConvexPoly_obj) },
    { MP_ROM_QSTR(MP_QSTR_fillPoly), MP_ROM_PTR(&cv2_imgproc_fillPoly_obj) },
    { MP_ROM_QSTR(MP_QSTR_getStructuringElement), MP_ROM_PTR(&cv2_imgproc_getStructuringElement_obj) },
    { MP_ROM_QSTR(MP_QSTR_line), MP_ROM_PTR(&cv2_imgproc_line_obj) },
    { MP_ROM_QSTR(MP_QSTR_morphologyEx), MP_ROM_PTR(&cv2_imgproc_morphologyEx_obj) },
    // { MP_ROM_QSTR(MP_QSTR_putText), MP_ROM_PTR(&cv2_imgproc_putText_obj) },
    { MP_ROM_QSTR(MP_QSTR_rectangle), MP_ROM_PTR(&cv2_imgproc_rectangle_obj) },
};
static MP_DEFINE_CONST_DICT(cv2_module_globals, cv2_module_globals_table);

// Define module object.
const mp_obj_module_t cv2_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&cv2_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_cv2, cv2_user_cmodule);

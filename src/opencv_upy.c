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
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_cvtColor_obj, 2, cv2_imgproc_cvtColor);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_dilate_obj, 2, cv2_imgproc_dilate);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_erode_obj, 2, cv2_imgproc_erode);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_getStructuringElement_obj, 2, cv2_imgproc_getStructuringElement);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_morphologyEx_obj, 3, cv2_imgproc_morphologyEx);

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
    
    ////////////////////////////////////////////////////////////////////////////
    // OpenCV core functions
    ////////////////////////////////////////////////////////////////////////////
    
    { MP_ROM_QSTR(MP_QSTR_inRange), MP_ROM_PTR(&cv2_core_inRange_obj) },
    { MP_ROM_QSTR(MP_QSTR_max), MP_ROM_PTR(&cv2_core_max_obj) },
    { MP_ROM_QSTR(MP_QSTR_min), MP_ROM_PTR(&cv2_core_min_obj) },
    
    ////////////////////////////////////////////////////////////////////////////
    // OpenCV imgproc functions
    ////////////////////////////////////////////////////////////////////////////
    
    { MP_ROM_QSTR(MP_QSTR_cvtColor), MP_ROM_PTR(&cv2_imgproc_cvtColor_obj) },
    { MP_ROM_QSTR(MP_QSTR_dilate), MP_ROM_PTR(&cv2_imgproc_dilate_obj) },
    { MP_ROM_QSTR(MP_QSTR_erode), MP_ROM_PTR(&cv2_imgproc_erode_obj) },
    { MP_ROM_QSTR(MP_QSTR_getStructuringElement), MP_ROM_PTR(&cv2_imgproc_getStructuringElement_obj) },
    { MP_ROM_QSTR(MP_QSTR_morphologyEx), MP_ROM_PTR(&cv2_imgproc_morphologyEx_obj) },
};
static MP_DEFINE_CONST_DICT(cv2_module_globals, cv2_module_globals_table);

// Define module object.
const mp_obj_module_t cv2_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&cv2_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_cv2, cv2_user_cmodule);

// C headers
#include "py/runtime.h"

// Function declarations
extern mp_obj_t cv2_core_convertScaleAbs(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_core_inRange(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_core_minMaxLoc(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

// Python references to the functions
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_core_convertScaleAbs_obj, 1, cv2_core_convertScaleAbs);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_core_inRange_obj, 3, cv2_core_inRange);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_core_minMaxLoc_obj, 1, cv2_core_minMaxLoc);

// Global definitions for functions and constants
#define OPENCV_CORE_GLOBALS \
    /* Functions */ \
    { MP_ROM_QSTR(MP_QSTR_convertScaleAbs), MP_ROM_PTR(&cv2_core_convertScaleAbs_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_inRange), MP_ROM_PTR(&cv2_core_inRange_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_minMaxLoc), MP_ROM_PTR(&cv2_core_minMaxLoc_obj) }, \
    \
    /* OpenCV data types, from opencv2/core/hal/interface.h */ \
    /* Other types are currently not supported by ulab */ \
    { MP_ROM_QSTR(MP_QSTR_CV_8U), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_CV_8S), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_CV_16U), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_CV_16S), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_CV_32F), MP_ROM_INT(4) }, \
    \
    /* Border types, from opencv2/core/base.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_BORDER_CONSTANT), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_REPLICATE), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_REFLECT), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_WRAP), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_REFLECT_101), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_TRANSPARENT), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_REFLECT101), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_DEFAULT), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_BORDER_ISOLATED), MP_ROM_INT(16) }

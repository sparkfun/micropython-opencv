// C headers
#include "py/runtime.h"

// Function declarations
extern mp_obj_t cv2_highgui_imshow(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_highgui_waitKey(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

// Python references to the functions
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_highgui_imshow_obj, 2, cv2_highgui_imshow);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_highgui_waitKey_obj, 0, cv2_highgui_waitKey);

// Global definitions for functions and constants
#define OPENCV_HIGHGUI_GLOBALS \
    /* Functions */ \
    { MP_ROM_QSTR(MP_QSTR_imshow), MP_ROM_PTR(&cv2_highgui_imshow_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_waitKey), MP_ROM_PTR(&cv2_highgui_waitKey_obj) }

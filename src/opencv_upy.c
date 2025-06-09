#include "core.h"
#include "highgui.h"
#include "imgcodecs.h"
#include "imgproc.h"

// Python module globals dictionary
static const mp_rom_map_elem_t cv2_module_globals_table[] = {
    // Python module name
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_cv2) },

    // Inlude globals from each OpenCV module
    OPENCV_CORE_GLOBALS,
    OPENCV_HIGHGUI_GLOBALS,
    OPENCV_IMGCODECS_GLOBALS,
    OPENCV_IMGPROC_GLOBALS,
};
static MP_DEFINE_CONST_DICT(cv2_module_globals, cv2_module_globals_table);

// Define module object.
const mp_obj_module_t cv2_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&cv2_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_cv2, cv2_user_cmodule);

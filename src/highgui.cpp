// C++ headers
#include "opencv2/core.hpp"
#include "convert.h"
#include "numpy.h"

// C headers
extern "C" {
#include "highgui.h"
#include "ulab/code/ndarray.h"
#include "py/mphal.h"
} // extern "C"

extern const mp_obj_type_t cv2_display_type;

using namespace cv;

mp_obj_t cv2_highgui_imshow(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_display, ARG_img };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_display, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Assume the display object has an `imshow` method and load it. The method
    // array should be loaded with method[0] as the method object and method[1]
    // as the self object.
    mp_obj_t method[3];
    mp_load_method_maybe(args[ARG_display].u_obj, MP_QSTR_imshow, method);

    // Check if the method was found
    if(method[0] == MP_OBJ_NULL) {
        // Method not found, raise an AttributeError
        mp_raise_msg(&mp_type_AttributeError, MP_ERROR_TEXT("`cv2.imshow()` requires a display object with its own 'imshow()' method, not a window name string."));
    }

    // Add the image object to the method arguments
    method[2] = args[ARG_img].u_obj;

    // Call the method with one positional argument (the image we just added)
    return mp_call_method_n_kw(1, 0, method);
}

mp_obj_t cv2_highgui_waitKey(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { Arg_delay };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_delay, MP_ARG_INT, {.u_int = 0} },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    int delay = args[Arg_delay].u_int;

    // Because we have no way to get user input in this environment, we'll just
    // delay for the specified time and return a dummy value. Normally, passing
    // a delay of 0 would wait infinitely until a keyPress, but since that will
    // never happen here, we will just return immediately after the delay.
    if(delay > 0)
        mp_hal_delay_ms(delay);
    
    // Return a dummy value to indicate no key was pressed
    return MP_OBJ_NEW_SMALL_INT(-1);
}

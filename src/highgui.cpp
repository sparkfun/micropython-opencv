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

    // Derived from:
    // https://github.com/orgs/micropython/discussions/11448
    
    // Import `sys` and `select` modules
    mp_obj_t sys_module = mp_import_name(MP_QSTR_sys, mp_const_none, MP_OBJ_NEW_SMALL_INT(0));
    mp_obj_t select_module = mp_import_name(MP_QSTR_select, mp_const_none, MP_OBJ_NEW_SMALL_INT(0));

    // Get the `sys.stdin` object
    mp_obj_t stdin_obj = mp_load_attr(sys_module, MP_QSTR_stdin);

    // Get the `select.POLLIN` constant
    mp_obj_t pollin_obj = mp_load_attr(select_module, MP_QSTR_POLLIN);

    // Call `select.poll()` function to create a poll object
    mp_obj_t select_poll_method[2];
    mp_load_method(select_module, MP_QSTR_poll, select_poll_method);
    mp_obj_t poll_obj = mp_call_method_n_kw(0, 0, select_poll_method);

    // Call `poll.register(sys.stdin, select.POLLIN)`
    mp_obj_t poll_register_method[4];
    mp_load_method(poll_obj, MP_QSTR_register, poll_register_method);
    poll_register_method[2] = stdin_obj;
    poll_register_method[3] = pollin_obj;
    mp_call_method_n_kw(2, 0, poll_register_method);

    // Create timeout integer object for next method call. OpenCV uses a delay
    // of 0 to wait indefinitely, whereas `select.poll` uses -1
    mp_obj_t timeout = MP_OBJ_NEW_SMALL_INT(delay <= 0 ? -1 : delay);

    // Call `poll.poll(timeout)`
    mp_obj_t poll_poll_method[3];
    mp_load_method(poll_obj, MP_QSTR_poll, poll_poll_method);
    poll_poll_method[2] = timeout;
    mp_obj_t result = mp_call_method_n_kw(1, 0, poll_poll_method);

    // Extract the items from the result list
    mp_obj_t *items;
    size_t len;
    mp_obj_list_get(result, &len, &items);

    // Check if any items were returned
    if(len == 0) {
        // If no items were returned, return -1 to indicate no key was pressed
        return MP_OBJ_NEW_SMALL_INT(-1);
    }

    // Since something was returned, a key was pressed. We need to extract it
    // with `sys.stdin.read(1)`
    mp_obj_t read_method[3];
    mp_load_method(stdin_obj, MP_QSTR_read, read_method);
    read_method[2] = MP_OBJ_NEW_SMALL_INT(1);
    return mp_call_method_n_kw(1, 0, read_method);
}

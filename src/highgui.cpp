/*
 *------------------------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * 
 * Copyright (c) 2025 SparkFun Electronics
 *------------------------------------------------------------------------------
 * highgui.cpp
 * 
 * MicroPython wrappers for functions from the OpenCV highgui module.
 *------------------------------------------------------------------------------
 */

// C++ headers
#include "opencv2/core.hpp"
#include "convert.h"
#include "numpy.h"

// C headers
extern "C" {
#include "highgui.h"
#include "ndarray.h"
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
    // Call waitKeyEx to do the heavy lifting
    mp_obj_t key = cv2_highgui_waitKeyEx(n_args, pos_args, kw_args);

    // Get the key code as an integer
    int32_t key_code = mp_obj_get_int(key);

    // If the key code is -1, it means no key was pressed
    if (key_code == -1) {
        // Return the original key object
        return key;
    } else {
        // Return the last byte of the key code
        return MP_OBJ_NEW_SMALL_INT(key_code & 0xFF);
    }
}

mp_obj_t cv2_highgui_waitKeyEx(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
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

    // Load the `poll.poll()` method to check for key presses
    mp_obj_t poll_poll_method[3];
    mp_load_method(poll_obj, MP_QSTR_poll, poll_poll_method);

    // Load the `sys.stdin.read(1)` method to read a single character
    mp_obj_t read_method[3];
    mp_load_method(stdin_obj, MP_QSTR_read, read_method);
    read_method[2] = MP_OBJ_NEW_SMALL_INT(1);

    // Initialize key code to -1, which indicates no key was pressed
    int32_t key_code = -1;

    // Some key presses return multiple bytes (eg. up arrow key returns 3 bytes:
    // `\x1b[A`). To handle this, we will loop until no more bytes are available
    for (int i = 0; true; i++) {
        // Call `poll.poll(timeout)` if this is the first iteration, otherwise
        // call `poll.poll(1)` to quickly check for any remaining bytes. Can't
        // wait 0ms, because it takes a moment for all bytes to arrive
        poll_poll_method[2] = i == 0 ? timeout : MP_OBJ_NEW_SMALL_INT(1);
        mp_obj_t result = mp_call_method_n_kw(1, 0, poll_poll_method);

        // Extract the items from the result list
        mp_obj_t *items;
        size_t len;
        mp_obj_list_get(result, &len, &items);

        // Check if any items were returned
        if(len == 0) {
            // No more bytes available, so we're done. If multiple bytes were
            // read, we want the last byte to be 0 so it doesn't get confused
            // in `waitKey()` with a normal key press. So we can simply shift
            // the key code left by 8 bits again
            if (i > 1) {
                key_code <<= 8;
            }
            break;
        }

        // Since something was returned, a byte is available. We need to
        // extract it by calling `sys.stdin.read(1)`
        mp_obj_t byte_str = mp_call_method_n_kw(1, 0, read_method);

        // Convert the byte object to an actual byte
        uint8_t byte_val = mp_obj_str_get_str(byte_str)[0];

        // Check which iteration this is
        if(i == 0) {
            // This is the first iteration, set the key code to this byte
            key_code = byte_val;
            
            // Special keys always start with an escape character (0x1b). If
            // this is not the escape character, we can assume it's a normal key
            // press and break immediately. This helps mitigate the problem of
            // interpreting 2 key simultaneous key presses as 1 special key
            if (byte_val != 0x1b) {
                break;
            }
        } else if (i == 1) {
            // This is the second iteration, meaning the first byte was the
            // escape character. We don't want that to be part of the key code
            // (special keys will be indicated by having multiple bytes, and the
            // last byte being zero), so we'll just overwrite the key code with
            // the second byte
            key_code = byte_val;
        } else {
            // This is a subsequent iteration, meaning we have already read the
            // escape character and the second byte. For all subsequent bytes,
            // we will shift the key code left by 8 bits and add the new byte to
            // it to create a multi-byte key
            key_code = (key_code << 8) | byte_val;
        }
    }

    // Return the final key code
    return MP_OBJ_NEW_SMALL_INT(key_code);
}

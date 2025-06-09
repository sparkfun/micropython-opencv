// C++ headers
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "convert.h"
#include "numpy.h"

// C headers
extern "C" {
#include "imgcodecs.h"
#include "ulab/code/ndarray.h"
#include "py/builtin.h"
} // extern "C"

using namespace cv;

// Helper macro to create an empty mp_map_t, derived from MP_DEFINE_CONST_MAP.
// Primarily used for function calls with no keyword arguments, since we can't
// just pass `NULL` or mp_const_none (crash occurs otherwise)
#define MP_EMPTY_MAP() { \
        .all_keys_are_qstrs = 0, \
        .is_fixed = 1, \
        .is_ordered = 0, \
        .used = 0, \
        .alloc = 0, \
        .table = (mp_map_elem_t *)(mp_rom_map_elem_t *)mp_const_none, \
    }

mp_obj_t cv2_imgcodecs_imread(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_filename, ARG_flags };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_filename, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_flags, MP_ARG_INT, { .u_int = IMREAD_COLOR_BGR } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    mp_obj_t filename = args[ARG_filename].u_obj;
    int flags = args[ARG_flags].u_int;

    // Call MicroPython's `open()` function to read the image file
    mp_obj_t open_args[2];
    open_args[0] = filename;
    open_args[1] = mp_obj_new_str("rb", 2); // Open in binary read mode
    mp_map_t open_kw_args = MP_EMPTY_MAP(); // No keyword arguments
    mp_obj_t file_obj = mp_builtin_open(2, open_args, &open_kw_args);

    // Call the `read()` method on the file object to get the image data
    mp_obj_t read_method[2];
    mp_load_method(file_obj, MP_QSTR_read, read_method);
    mp_obj_t bytes_obj = mp_call_method_n_kw(0, 0, read_method);

    // Close the file object
    mp_obj_t close_method[2];
    mp_load_method(file_obj, MP_QSTR_close, close_method);
    mp_call_method_n_kw(0, 0, close_method);

    // Convert bytes_obj to vector of uint8_t for decoding
    size_t len;
    const char *buf_data = mp_obj_str_get_data(bytes_obj, &len);
    std::vector<uint8_t> buf(buf_data, buf_data + len);

    // Decode the image from the buffer
    Mat img;
    try {
        img = imdecode(buf, flags);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the image
    return mat_to_mp_obj(img);
}

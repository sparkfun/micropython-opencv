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
    img.allocator = &GetNumpyAllocator();
    try {
        img = imdecode(buf, flags);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Return the result
    return mat_to_mp_obj(img);
}

mp_obj_t cv2_imgcodecs_imwrite(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // Define the arguments
    enum { ARG_filename, ARG_img, ARG_params };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_filename, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_img, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_params, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Convert arguments to required types
    mp_obj_t filename = args[ARG_filename].u_obj;
    Mat img = mp_obj_to_mat(args[ARG_img].u_obj);
    ndarray_obj_t *params;
    if (args[ARG_params].u_obj == mp_const_none) {
        // If no parameters are provided, use an empty ndarray
        params = ndarray_new_linear_array(0, NDARRAY_INT16);
    } else {
        params = ndarray_from_mp_obj(args[ARG_params].u_obj, 0);
    }

    // Convert the filename to a std::string
    size_t filename_len;
    const char *filename_chr = mp_obj_str_get_data(filename, &filename_len);
    std::string filename_str(filename_chr, filename_len);

    // Create vector of uint8_t for encoding
    std::vector<uint8_t> buf;

    // Convert the parameters to a vector of int
    std::vector<int> params_vec;
    if (params->len > 0) {
        params_vec.reserve(params->len);
        for (size_t i = 0; i < params->len; ++i) {
            mp_obj_t val = (mp_obj_t*) mp_binary_get_val_array(params->dtype, params->array, i);
            // ndarrays default to float, and mp_obj_get_int() does not support 
            // float values, so we need to do the type conversion ourselves
            mp_float_t val_float = mp_obj_get_float(val);
            params_vec.push_back((int) val_float);
        }
    }

    // Encode the image from the buffer
    bool retval;
    try {
        retval = imencode(filename_str, img, buf, params_vec);
    } catch(Exception& e) {
        mp_raise_msg(&mp_type_Exception, MP_ERROR_TEXT(e.what()));
    }

    // Convert the vector of uint8_t to a bytes object
    mp_obj_t buf_obj = mp_obj_new_bytes((const byte *)buf.data(), buf.size());

    // Call MicroPython's `open()` function to write the image file
    mp_obj_t open_args[2];
    open_args[0] = filename;
    open_args[1] = mp_obj_new_str("wb", 2); // Open in binary write mode
    mp_map_t open_kw_args = MP_EMPTY_MAP(); // No keyword arguments
    mp_obj_t file_obj = mp_builtin_open(2, open_args, &open_kw_args);

    // Call the `write()` method on the file object to write the image data
    mp_obj_t write_method[3];
    mp_load_method(file_obj, MP_QSTR_write, write_method);
    write_method[2] = buf_obj; // Set the data to write
    mp_call_method_n_kw(1, 0, write_method);

    // Close the file object
    mp_obj_t close_method[2];
    mp_load_method(file_obj, MP_QSTR_close, close_method);
    mp_call_method_n_kw(0, 0, close_method);

    // Return the result
    return mp_obj_new_bool(retval);
}

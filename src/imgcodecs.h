/*
 *------------------------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * 
 * Copyright (c) 2025 SparkFun Electronics
 *------------------------------------------------------------------------------
 * imgcodecs.h
 * 
 * MicroPython wrappers for functions from the OpenCV imgcodecs module.
 *------------------------------------------------------------------------------
 */

// C headers
#include "py/runtime.h"

// Function declarations
extern mp_obj_t cv2_imgcodecs_imread(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgcodecs_imwrite(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

// Python references to the functions
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgcodecs_imread_obj, 1, cv2_imgcodecs_imread);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgcodecs_imwrite_obj, 2, cv2_imgcodecs_imwrite);

// Global definitions for functions and constants
#define OPENCV_IMGCODECS_GLOBALS \
    /* Functions */ \
    { MP_ROM_QSTR(MP_QSTR_imread), MP_ROM_PTR(&cv2_imgcodecs_imread_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_imwrite), MP_ROM_PTR(&cv2_imgcodecs_imwrite_obj) }, \
    \
    /* Image read flags, from opencv2/imgcodecs.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_UNCHANGED), MP_ROM_INT(-1) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_GRAYSCALE), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_COLOR_BGR), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_COLOR), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_ANYDEPTH), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_ANYCOLOR), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_LOAD_GDAL), MP_ROM_INT(8) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_REDUCED_GRAYSCALE_2), MP_ROM_INT(16) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_REDUCED_COLOR_2), MP_ROM_INT(17) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_REDUCED_GRAYSCALE_4), MP_ROM_INT(32) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_REDUCED_COLOR_4), MP_ROM_INT(33) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_REDUCED_GRAYSCALE_8), MP_ROM_INT(64) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_REDUCED_COLOR_8), MP_ROM_INT(65) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_IGNORE_ORIENTATION), MP_ROM_INT(128) }, \
    { MP_ROM_QSTR(MP_QSTR_IMREAD_COLOR_RGB), MP_ROM_INT(256) }, \
    \
    /* Image write flags, from opencv2/imgcodecs.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_QUALITY), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_PROGRESSIVE), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_OPTIMIZE), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_RST_INTERVAL), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_LUMA_QUALITY), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_CHROMA_QUALITY), MP_ROM_INT(6) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG_SAMPLING_FACTOR), MP_ROM_INT(7) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_PNG_COMPRESSION), MP_ROM_INT(16) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_PNG_STRATEGY), MP_ROM_INT(17) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_PNG_BILEVEL), MP_ROM_INT(18) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_PXM_BINARY), MP_ROM_INT(32) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_EXR_TYPE), MP_ROM_INT((3 << 4) + 0) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_EXR_COMPRESSION), MP_ROM_INT((3 << 4) + 1) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_EXR_DWA_COMPRESSION_LEVEL), MP_ROM_INT((3 << 4) + 2) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_WEBP_QUALITY), MP_ROM_INT(64) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_HDR_COMPRESSION), MP_ROM_INT((5 << 4) + 0) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_PAM_TUPLETYPE), MP_ROM_INT(128) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_TIFF_RESUNIT), MP_ROM_INT(256) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_TIFF_XDPI), MP_ROM_INT(257) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_TIFF_YDPI), MP_ROM_INT(258) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_TIFF_COMPRESSION), MP_ROM_INT(259) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_TIFF_ROWSPERSTRIP), MP_ROM_INT(278) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_TIFF_PREDICTOR), MP_ROM_INT(317) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEG2000_COMPRESSION_X1000), MP_ROM_INT(272) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_AVIF_QUALITY), MP_ROM_INT(512) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_AVIF_DEPTH), MP_ROM_INT(513) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_AVIF_SPEED), MP_ROM_INT(514) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEGXL_QUALITY), MP_ROM_INT(640) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEGXL_EFFORT), MP_ROM_INT(641) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEGXL_DISTANCE), MP_ROM_INT(642) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_JPEGXL_DECODING_SPEED), MP_ROM_INT(643) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_GIF_LOOP), MP_ROM_INT(1024) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_GIF_SPEED), MP_ROM_INT(1025) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_GIF_QUALITY), MP_ROM_INT(1026) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_GIF_DITHER), MP_ROM_INT(1027) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_GIF_TRANSPARENCY), MP_ROM_INT(1028) }, \
    { MP_ROM_QSTR(MP_QSTR_IMWRITE_GIF_COLORTABLE), MP_ROM_INT(1029) }
    
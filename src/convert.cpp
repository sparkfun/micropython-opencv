// C++ headers
#include "convert.h"
#include "numpy.h"

// C headers
extern "C" {
#include "ulab_tools.h"
#include "py/obj.h"
} // extern "C"

uint8_t mat_depth_to_ndarray_type(int depth)
{
    switch (depth) {
        case CV_8U: return NDARRAY_UINT8;
        case CV_8S: return NDARRAY_INT8;
        case CV_16U: return NDARRAY_UINT16;
        case CV_16S: return NDARRAY_INT16;
        case CV_32F: return NDARRAY_FLOAT;
        case CV_Bool: return NDARRAY_BOOL;
        default: mp_raise_TypeError(MP_ERROR_TEXT("Unsupported Mat depth"));
    }
}

int ndarray_type_to_mat_depth(uint8_t type)
{
    switch (type) {
        case NDARRAY_UINT8: return CV_8U;
        case NDARRAY_INT8: return CV_8S;
        case NDARRAY_UINT16: return CV_16U;
        case NDARRAY_INT16: return CV_16S;
        case NDARRAY_FLOAT: return CV_32F;
        case NDARRAY_BOOL: return CV_Bool;
        default: mp_raise_TypeError(MP_ERROR_TEXT("Unsupported ndarray type"));
    }
}

ndarray_obj_t *mat_to_ndarray(Mat& mat)
{
    // Derived from:
    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L313-L328
    if(mat.data == NULL)
        mp_const_none;
    Mat temp, *ptr = (Mat*)&mat;
    if(!ptr->u || ptr->allocator != &GetNumpyAllocator())
    {
        temp.allocator = &GetNumpyAllocator();
        mat.copyTo(temp);
        ptr = &temp;
    }
    ndarray_obj_t* ndarray = (ndarray_obj_t*) ptr->u->userdata;
    return ndarray;
}

Mat ndarray_to_mat(ndarray_obj_t *ndarray)
{
    // Derived from:
    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L54-L311
    // Some steps are skipped, as they are not needed or handle extreme edge
    // cases. Skipping these steps helps keep the code smaller and run faster.

    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L56-L119
    // We have an ndarray_obj_t, so these checks have already been done.

    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L130-L172
    int type = ndarray_type_to_mat_depth(ndarray->dtype);

    int ndims = ndarray->ndim;
    size_t elemsize = ndarray->itemsize;
    int _sizes[ndarray->ndim];
    size_t _strides[ndarray->ndim];
    for (int i = 0; i < ndarray->ndim; i++) {
        _sizes[i] = ndarray->shape[ULAB_MAX_DIMS - ndarray->ndim + i];
        _strides[i] = ndarray->strides[ULAB_MAX_DIMS - ndarray->ndim + i];
    }

    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L176-L221
    bool ismultichannel = ndims == 3;

    if (ismultichannel)
    {
        int channels = ndims >= 1 ? (int)_sizes[ndims - 1] : 1;
        ndims--;
        type |= CV_MAKETYPE(0, channels);
        elemsize = CV_ELEM_SIZE(type);
    }

    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L243-L261
    int size[CV_MAX_DIM+1] = {};
    size_t step[CV_MAX_DIM+1] = {};

    size_t default_step = elemsize;
    for ( int i = ndims - 1; i >= 0; --i )
    {
        size[i] = (int)_sizes[i];
        if ( size[i] > 1 )
        {
            step[i] = (size_t)_strides[i];
            default_step = step[i] * size[i];
        }
        else
        {
            step[i] = default_step;
            default_step *= size[i];
        }
    }

    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L290-L294
    if( ndims == 0) {
        size[ndims] = 1;
        step[ndims] = elemsize;
        ndims++;
    }

    // https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_convert.cpp#L300-L10
    Mat mat = Mat(ndims, size, type, (void*) ndarray->array, step);
    mat.u = GetNumpyAllocator().allocate(ndarray, ndims, size, type, step);
    mat.addref();

    mat.allocator = &GetNumpyAllocator();

    return mat;
}

mp_obj_t mat_to_mp_obj(Mat &mat)
{
    return MP_OBJ_FROM_PTR(mat_to_ndarray(mat));
}

Mat mp_obj_to_mat(mp_obj_t obj)
{
    // Check for None object
    if(obj == mp_const_none)
    {
        // Create an empty Mat object. Set the NumpyAllocator in case this
        // matrix gets automatically allocated later
        Mat mat;
        mat.allocator = &GetNumpyAllocator();
        return mat;
    }

    // Assume the object is a ndarray, or can be converted to one. Will raise an
    // exception if not
    ndarray_obj_t *ndarray = ndarray_from_mp_obj(obj, 0);

    // Convert ndarray to Mat
    Mat mat = ndarray_to_mat(ndarray);

    return mat;
}

Size mp_obj_to_size(mp_obj_t obj)
{
    // Check for None object
    if(obj == mp_const_none)
    {
        // Create an empty Size object
        return Size();
    }

    // Assume the object is a ndarray, or can be converted to one. Will raise an
    // exception if not
    ndarray_obj_t *ndarray = ndarray_from_mp_obj(obj, 0);
    
    // Validate the length of the ndarray
    if(ndarray->len != 2)
    {
        mp_raise_TypeError(MP_ERROR_TEXT("Size must be length 2"));
    }

    // Compute the size, checking the type of the ndarray
    Size size;
    switch(ndarray->dtype)
    {
        case NDARRAY_UINT8:
            size.width = ((uint8_t*) ndarray->array)[0];
            size.height = ((uint8_t*) ndarray->array)[1];
            break;
        case NDARRAY_INT8:
            size.width = ((int8_t*) ndarray->array)[0];
            size.height = ((int8_t*) ndarray->array)[1];
            break;
        case NDARRAY_UINT16:
            size.width = ((uint16_t*) ndarray->array)[0];
            size.height = ((uint16_t*) ndarray->array)[1];
            break;
        case NDARRAY_INT16:
            size.width = ((int16_t*) ndarray->array)[0];
            size.height = ((int16_t*) ndarray->array)[1];
            break;
        case NDARRAY_FLOAT:
            size.width = ((float*) ndarray->array)[0];
            size.height = ((float*) ndarray->array)[1];
            break;
        case NDARRAY_BOOL:
            size.width = ((bool*) ndarray->array)[0];
            size.height = ((bool*) ndarray->array)[1];
            break;
        default:
            mp_raise_TypeError(MP_ERROR_TEXT("Unsupported ndarray type"));
            break;
    }

    return size;
}

Point mp_obj_to_point(mp_obj_t obj)
{
    // Check for None object
    if(obj == mp_const_none)
    {
        // Create an empty Point object
        return Point();
    }

    // Assume the object is a ndarray, or can be converted to one. Will raise an
    // exception if not
    ndarray_obj_t *ndarray = ndarray_from_mp_obj(obj, 0);
    
    // Validate the length of the ndarray
    if(ndarray->len != 2)
    {
        mp_raise_TypeError(MP_ERROR_TEXT("Point must be length 2"));
    }

    // Compute the point, checking the type of the ndarray
    Point point;
    switch(ndarray->dtype)
    {
        case NDARRAY_UINT8:
            point.x = ((uint8_t*) ndarray->array)[0];
            point.y = ((uint8_t*) ndarray->array)[1];
            break;
        case NDARRAY_INT8:
            point.x = ((int8_t*) ndarray->array)[0];
            point.y = ((int8_t*) ndarray->array)[1];
            break;
        case NDARRAY_UINT16:
            point.x = ((uint16_t*) ndarray->array)[0];
            point.y = ((uint16_t*) ndarray->array)[1];
            break;
        case NDARRAY_INT16:
            point.x = ((int16_t*) ndarray->array)[0];
            point.y = ((int16_t*) ndarray->array)[1];
            break;
        case NDARRAY_FLOAT:
            point.x = ((float*) ndarray->array)[0];
            point.y = ((float*) ndarray->array)[1];
            break;
        case NDARRAY_BOOL:
            point.x = ((bool*) ndarray->array)[0];
            point.y = ((bool*) ndarray->array)[1];
            break;
        default:
            mp_raise_TypeError(MP_ERROR_TEXT("Unsupported ndarray type"));
            break;
    }

    return point;
}

Scalar mp_obj_to_scalar(mp_obj_t obj)
{
    // Check for None object
    if(obj == mp_const_none)
    {
        // Create an empty Scalar object
        return Scalar();
    }

    // Assume the object is a ndarray, or can be converted to one. Will raise an
    // exception if not
    ndarray_obj_t *ndarray = ndarray_from_mp_obj(obj, 0);
    
    // Validate the length of the ndarray
    if(ndarray->len > 4)
    {
        mp_raise_TypeError(MP_ERROR_TEXT("Scalar must be length 4 or less"));
    }

    // Compute the scalar, checking the type of the ndarray
    Scalar scalar;
    switch(ndarray->dtype)
    {
        case NDARRAY_UINT8:
            for(size_t i = 0; i < ndarray->len; i++)
                scalar[i] = ((uint8_t*) ndarray->array)[i];
            break;
        case NDARRAY_INT8:
            for(size_t i = 0; i < ndarray->len; i++)
                scalar[i] = ((int8_t*) ndarray->array)[i];
            break;
        case NDARRAY_UINT16:
            for(size_t i = 0; i < ndarray->len; i++)
                scalar[i] = ((uint16_t*) ndarray->array)[i];
            break;
        case NDARRAY_INT16:
            for(size_t i = 0; i < ndarray->len; i++)
                scalar[i] = ((int16_t*) ndarray->array)[i];
            break;
        case NDARRAY_FLOAT:
            for(size_t i = 0; i < ndarray->len; i++)
                scalar[i] = ((float*) ndarray->array)[i];
            break;
        case NDARRAY_BOOL:
            for(size_t i = 0; i < ndarray->len; i++)
                scalar[i] = ((bool*) ndarray->array)[i];
            break;
        default:
            mp_raise_TypeError(MP_ERROR_TEXT("Unsupported ndarray type"));
            break;
    }

    return scalar;
}

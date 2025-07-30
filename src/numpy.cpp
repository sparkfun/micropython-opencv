/*
 *------------------------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * 
 * Copyright (c) 2025 SparkFun Electronics
 *------------------------------------------------------------------------------
 * numpy.cpp
 * 
 * OpenCV Mat allocator implementation for ulab NumPy. Derived from:
 * https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_numpy.cpp
 * Licensed under the Apache License, Version 2.0
 * Copyright (C) 2000-2022, Intel Corporation, all rights reserved.
 * Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
 * Copyright (C) 2009-2016, NVIDIA Corporation, all rights reserved.
 * Copyright (C) 2010-2013, Advanced Micro Devices, Inc., all rights reserved.
 * Copyright (C) 2015-2023, OpenCV Foundation, all rights reserved.
 * Copyright (C) 2008-2016, Itseez Inc., all rights reserved.
 * Copyright (C) 2019-2023, Xperience AI, all rights reserved.
 * Copyright (C) 2019-2022, Shenzhen Institute of Artificial Intelligence and Robotics for Society, all rights reserved.
 * Copyright (C) 2022-2023, Southern University of Science And Technology, all rights reserved.
 * 
 * Third party copyrights are property of their respective owners.
 *------------------------------------------------------------------------------
 */

// C++ headers
#include "numpy.h"
#include "convert.h"

UMatData* NumpyAllocator::allocate(ndarray_obj_t* ndarray, int dims, const int* sizes, int type, size_t* step) const
{
    UMatData* u = new UMatData(this);
    u->data = (uchar*) ndarray->array;
    u->origdata = (uchar*) ndarray->origin;
    for( int i = 0; i < dims - 1; i++ )
        step[i] = ndarray->strides[ULAB_MAX_DIMS - ndarray->ndim + i];
    step[dims-1] = CV_ELEM_SIZE(type);
    u->size = sizes[0]*step[0];
    u->userdata = ndarray;
    return u;
}

UMatData* NumpyAllocator::allocate(int dims0, const int* sizes, int type, void* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) const
{
    if( data != 0 )
    {
        return stdAllocator->allocate(dims0, sizes, type, data, step, flags, usageFlags);
    }

    int depth = CV_MAT_DEPTH(type);
    int cn = CV_MAT_CN(type);
    int typenum = mat_depth_to_ndarray_type(depth);
    int i, dims = dims0;
    size_t _sizes[ULAB_MAX_DIMS];
    if( cn > 1 )
    {
        _sizes[ULAB_MAX_DIMS - 1] = cn;
        dims++;
    }
    for( i = 0; i < dims0; i++ )
        _sizes[ULAB_MAX_DIMS - dims + i] = sizes[i];
    ndarray_obj_t* ndarray = ndarray_new_dense_ndarray(dims, _sizes, typenum);
    return allocate(ndarray, dims0, sizes, type, step);
}

bool NumpyAllocator::allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) const
{
    return stdAllocator->allocate(u, accessFlags, usageFlags);
}

void NumpyAllocator::deallocate(UMatData* u) const
{
    if(!u)
        return;
    CV_Assert(u->urefcount >= 0);
    CV_Assert(u->refcount >= 0);
    if(u->refcount == 0)
    {
        delete u;
    }
}

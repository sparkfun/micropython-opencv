/*
 *------------------------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * 
 * Copyright (c) 2025 SparkFun Electronics
 *------------------------------------------------------------------------------
 * numpy.h
 * 
 * OpenCV Mat allocator implementation for ulab NumPy. Derived from:
 * https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_numpy.hpp
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
#include "opencv2/core.hpp"

// C headers
extern "C" {
#include "py/runtime.h"
#include "ndarray.h"
} // extern "C"

using namespace cv;

// Derived from:
// https://github.com/opencv/opencv/blob/aee828ac6ed3e45d7ca359d125349a570ca4e098/modules/python/src2/cv2_numpy.hpp#L7-L21
class NumpyAllocator : public MatAllocator
{
public:
    NumpyAllocator() { stdAllocator = Mat::getStdAllocator(); }
    ~NumpyAllocator() {}

    UMatData* allocate(ndarray_obj_t* o, int dims, const int* sizes, int type, size_t* step) const;
    UMatData* allocate(int dims0, const int* sizes, int type, void* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) const CV_OVERRIDE;
    bool allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) const CV_OVERRIDE;
    void deallocate(UMatData* u) const CV_OVERRIDE;

    const MatAllocator* stdAllocator;
};

inline NumpyAllocator& GetNumpyAllocator() {static NumpyAllocator gNumpyAllocator;return gNumpyAllocator;}

/*
 *------------------------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * 
 * Copyright (c) 2025 SparkFun Electronics
 *------------------------------------------------------------------------------
 * alloc.c
 * 
 * Wrapper functions for malloc(), free(), calloc(), and realloc(). These ensure
 * memory gets allocated on the C heap before the MicroPython garbage collector
 * has been initialized, and and in the GC pool afterwards.
 *------------------------------------------------------------------------------
 */

// C headers
#include "py/runtime.h"

// Hacky solution to see if the GC is initialized. If so, we can use gc_alloc
// and gc_free. Otherwise, we need to use __real_malloc and __real_free.
// TODO: Determine if this is acceptable or if we should use a different method.
// Would be nice to be able to #define MICROPY_BOARD_EARLY_INIT to set a flag,
// however that needs to be #defined in the board's mpconfigboard.h file.
bool gc_inited() {
    return MP_STATE_MEM(area).gc_pool_start != NULL;
}

// Since the linker flag `-Wl,--wrap=malloc` (and calloc, realloc, and free) is
// set, calls to `malloc()` get replaced with `__wrap_malloc()` by the linker.
// To use the original `malloc()`, we can instead use `__real_malloc()`, which
// the linker replaces with the original `malloc()`. However because that's done
// by the linker, we need to declare it here so the compiler knows about it.
extern void *__real_malloc(size_t size);
extern void *__real_calloc(size_t count, size_t size);
extern void *__real_realloc(void *mem, size_t size);
extern void *__real_free(void *mem);

// Implementations of the malloc, calloc, realloc, and free functions. If the
// GC is initialized, we use the MicroPython functions to use the GC heap.
// Otherwise, we use the "real" functions to use the C heap.
void *__wrap_malloc(size_t size) {
    if(gc_inited()) {
        return m_tracked_calloc(1, size);
    }
    else {
        return __real_malloc(size);
    }
}
void __wrap_free(void *ptr) {
    if(gc_inited()) {
        m_tracked_free(ptr);
    }
    else {
        __real_free(ptr);
    }
}
void *__wrap_calloc(size_t count, size_t size)
{
    if(gc_inited()) {
        return m_tracked_calloc(count, size);
    }
    else {
        return __real_calloc(count, size);
    }
}
void *__wrap_realloc(void *ptr, size_t size)
{
    if(gc_inited()) {
        void *new_ptr = m_tracked_calloc(1, size);
        if (new_ptr == NULL) {
            return NULL;
        }
        memcpy(new_ptr, ptr, size);
        m_tracked_free(ptr);
        return new_ptr;
    }
    else {
        return __real_realloc(ptr, size);
    }
}

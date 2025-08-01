#ifndef RP2350_UNSAFE_CV_XADD_H
#define RP2350_UNSAFE_CV_XADD_H

// Fix for https://github.com/raspberrypi/pico-sdk/issues/2505
// TLDR; OpenCV uses atomic operations for incrementing reference counters by
// default. However, the Pico SDK does not support atomic operations on data in
// PSRAM; attempting to do so just causes an infinite loop where the value is
// incremented forever. The workaround is to use a non-atomic operation by
// re-defining the `CV_XADD` macro. This is "unsafe" because it's not atomic,
// but it *should* be fine since we're only using one thread. Also see:
// https://github.com/opencv/opencv/blob/52bed3cd7890192700b2451e2713c340209ffd79/modules/core/include/opencv2/core/cvdef.h#L697-L723
static inline int unsafe_cv_xadd(int* addr, int delta)
{
    int tmp = *addr;
    *addr += delta;
    return tmp;
}
#define CV_XADD(addr, delta) unsafe_cv_xadd(addr, delta)

#endif

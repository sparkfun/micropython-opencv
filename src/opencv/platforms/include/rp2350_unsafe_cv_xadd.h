#ifndef RP2350_UNSAFE_CV_XADD_H
#define RP2350_UNSAFE_CV_XADD_H

// Fix for https://github.com/raspberrypi/pico-sdk/issues/2505
static inline int unsafe_cv_xadd(int* addr, int delta)
{
    int tmp = *addr;
    *addr += delta;
    return tmp;
}
#define CV_XADD(addr, delta) unsafe_cv_xadd(addr, delta)

#endif
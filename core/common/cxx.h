// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

// Integer type aliases.

using uint64_t = unsigned long;
using uint32_t = unsigned int;
using uint16_t = unsigned short;
using uint8_t  = unsigned char;

using int64_t = long;
using int32_t = int;
using int16_t = short;
using int8_t  = signed char;

using clock_t = uint64_t;

using nullptr_t = decltype(nullptr);

#if defined(PROCESSOR_x86_64) || defined(PROCESSOR_ARMv8)
using size_t    = unsigned long;
using uintptr_t = unsigned long;
#else
using size_t    = unsigned int;
using uintptr_t = unsigned long;
#endif

// Standard C++ compatibility (limited)

using ptr_func_t = void (*)();

extern ptr_func_t __init_array_start[];
extern ptr_func_t __init_array_end[];

extern ptr_func_t __fini_array_start[];
extern ptr_func_t __fini_array_end[];

// #if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
// #else
// #define STACK_CHK_GUARD 0x595e9fbd94fda766
// #endif

#if defined(__has_feature)
#if __has_feature(undefined_behavior_sanitizer)
#define UBSAN_ENABLED 1
#endif
#endif

// Suppress the "unused-paramater" warning.
#define Q_UNUSED(x) \
    (void)(x);      \
    static_assert(true)

#ifndef NDEBUG
// NDEBUG often requires a double negative which is confusing. We define Q_DEBUG as
// positive alternative.
#define DEBUG_MODE
#endif

#define Q_FOREVER(x) \
    while (true)     \
    x

#define Q_NO_RETURN(x) \
    x;                 \
    while (true)

extern "C"
{
    void __cxa_pure_virtual();
    int  __cxa_atexit(void (*func)(void*), void* arg, void* dso);
    void __cxa_finalize(void* dso);

    void __call_global_ctor();
    void __call_global_dtor();
}

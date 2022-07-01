// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "core/common/cxx.h"

#include "core/logger/logger.h"

extern "C"
{
    namespace __cxxabiv1 {

    // Needed for sanitizer.
    static __attribute__((used)) struct __function_type_info
    {
        __function_type_info()          = default;
        virtual ~__function_type_info() = default;
    } __function_type_info;

    } // namespace __cxxabiv1

    const auto MAX_GLOBAL_DESTRUCTORS = 128;

    // Each member is at least 4 bytes large. Such that each entry is 12 bytes.
    // 128 * 12 = 1.5KB exact.
    struct DestructorRef
    {
        void* argument;
        void* dso;
        void (*function)(void*);
    };

    // Array of destructors.
    DestructorRef __global_destructors[MAX_GLOBAL_DESTRUCTORS];
    uint8_t       __global_destructor_count = 0;

    int __cxa_atexit(void (*func)(void*), void* arg, void* dso)
    {
        if (__global_destructor_count >= MAX_GLOBAL_DESTRUCTORS) {
            qError() << "Global destructor limit exceeded:" << __global_destructor_count;
            return -1;
        }

        __global_destructors[__global_destructor_count] = { arg, dso, func };
        __global_destructor_count++;

        return 0;
    }

    void __cxa_pure_virtual()
    {
        qFatal() << "Unimplemented pure virtual function called";
    }

    void __cxa_finalize(void*)
    {
    }

    void __call_global_ctor()
    {
        int count = 0;

        for (auto** p = __init_array_start; p != __init_array_end; ++p) {
            (*p)();
            ++count;
        }
    }

    void __call_global_dtor()
    {
        for (auto** p = __fini_array_end; p != __fini_array_start; --p) {
            (*p)();
        }

        int count = __global_destructor_count;

        while (count--) {
            auto& dtor = __global_destructors[count];
            dtor.function(dtor.argument);
            dtor.function = nullptr;
        }
    }
}

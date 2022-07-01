// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

enum SystemCall
{
    Print  = 48 + 0,
    Panic  = 48 + 1,
    Halt   = 48 + 2,
    Abort  = 48 + 3,
    Assert = 48 + 4,
    Exit   = 48 + 5,
    Malloc = 48 + 6,
    Free   = 48 + 7,
    Sleep  = 48 + 8,
};

extern "C"
{
    void              __print(const char* data);
    void              __print_raw(const char* data, size_t length);
    void              __assert(bool expression);
    [[noreturn]] void __exit();
    [[noreturn]] void __panic(const char* message);
    [[noreturn]] void __abort();
    [[noreturn]] void __halt();
    void              __sleep();
    void              __free(void* ptr);
    void*             __malloc(size_t size);
}

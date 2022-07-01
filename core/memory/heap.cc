// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "core/common/sys.h"

void* operator new(size_t size)
{
    return __malloc(size);
}

void* operator new[](size_t size)
{
    return __malloc(size);
}

void operator delete(void* ptr)
{
    __free(ptr);
}

void operator delete[](void* ptr)
{
    __free(ptr);
}

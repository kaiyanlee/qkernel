// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "core/common/cxx.h"
#include "core/logger/logger.h"

extern "C"
{
    [[noreturn]] void __stack_chk_fail()
    {
        qError() << "Stack smashing detected...";
        __abort();
    }
}

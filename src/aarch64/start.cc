// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/system/system.h"

extern "C" [[noreturn]] void _startup(uintptr_t mb_address, uintptr_t mb_magic)
{
    system.startup(mb_address, mb_magic);
}

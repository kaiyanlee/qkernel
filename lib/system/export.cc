// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/system/export.h"

#include "lib/system/system.h"

void __print(const char* str)
{
    system.terminal.print(str);
}

void __print_raw(const char* str, size_t length)
{
    system.terminal.print(str, length);
}

void __panic(const char* message)
{
    __print(message);
    __exit();
}

void __exit()
{
    system.shutdown();
}

void __halt()
{
    while (true) {
        asm("cli; hlt");
    }
}

void __sleep()
{
    asm volatile("hlt");
}

void __abort()
{
    __exit();
}

void __assert(bool expression)
{
    if (!expression) {
        __exit();
    }
}

void* __malloc(size_t size)
{
    return system.memory.allocate(size);
}

void __free(void* data)
{
    system.memory.free(data);
}

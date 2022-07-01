// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "core/common/sys.h"

#ifndef ENABLE_KERNEL_MODE

void __syscall(SystemCall syscall)
{
    // TODO
    asm volatile("syscall" : : "a"(syscall) : "rcx", "r11", "memory");
}

void __syscall(SystemCall syscall, long)
{
    // TODO
    asm volatile("syscall" : : "a"(syscall) : "rcx", "r11", "memory");
}

void __syscall(SystemCall syscall, long, long)
{
    // TODO
    asm volatile("syscall" : : "a"(syscall) : "rcx", "r11", "memory");
}

void __print(const char*)
{
    __syscall(SystemCall::Print);
}

void __print_raw(const char*, size_t)
{
    __syscall(SystemCall::Print);
}

void __assert(bool)
{
    __syscall(SystemCall::Assert);
}

[[noreturn]] void __exit()
{
    Q_NO_RETURN(__syscall(SystemCall::Exit));
}

[[noreturn]] void __panic(const char*)
{
    Q_NO_RETURN(__syscall(SystemCall::Panic));
}

[[noreturn]] void __abort()
{
    Q_NO_RETURN(__syscall(SystemCall::Abort));
}

[[noreturn]] void __halt()
{
    Q_NO_RETURN(__syscall(SystemCall::Halt));
}

void __sleep()
{
    Q_NO_RETURN(__syscall(SystemCall::Halt));
}

void* __malloc(size_t)
{
    __syscall(SystemCall::Malloc);
    return nullptr;
}

void __free(void*)
{
    __syscall(SystemCall::Free);
}

#endif

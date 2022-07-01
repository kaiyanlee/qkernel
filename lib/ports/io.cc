// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/ports/io.h"

#include "core/common/cxx.h"

uint8_t Ports::read(uint16_t port) const
{
    uint8_t ret;
    asm volatile("xorl %eax,%eax");
    asm volatile("inb %%dx,%%al" : "=a"(ret) : "d"(port));
    return ret;
}

void Ports::write(uint16_t port, uint8_t data) const
{
    asm volatile("outb %%al,%%dx" ::"a"(data), "d"(port));
}

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"
#include "core/macros/repeat.h"

#define DECLARE_ISR(x) extern void _isr_##x();

#define PIC1 0x20 // Master
#define PIC2 0xA0 // Slave

#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define PIC1_OFFSET 0x20 // Vector offset for master PIC
#define PIC2_OFFSET 0x28 // Vector offset for slave PIC

extern "C"
{
    Q_REPEAT_PP(64, DECLARE_ISR);

    extern void _ignore_interrupt_request();
    uint32_t    _cpu_interrupt_handler(uint8_t n, uint32_t esp);
}

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

class Interrupts
{
public:
    Interrupts() = default;

public:
    void activate();
    void setup();

private:
    void set_idt_gate(uint8_t interrupt, uint32_t handler_addr);
    void load_idt();
    void install_routines();
    void remap_pic();
    void set_enable_nmi(bool enable);

private:
    struct GateIDT
    {
        uint16_t low_offset;
        uint16_t gdt_cs;
        uint8_t  reserved;
        uint8_t  flags;
        uint16_t high_offset;
    } __attribute__((packed));

    struct RegisterIDT
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

private:
    GateIDT m_idt[256];
};

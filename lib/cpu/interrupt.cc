// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/cpu/interrupt.h"

#include "core/logger/logger.h"
#include "core/macros/repeat.h"
#include "lib/cpu/isr.h"
#include "lib/system/system.h"

#define ISR_ENTRY(x) set_idt_gate(x, (uint32_t)_isr_##x);

#define ICW1_ICW4      0x01 // ICW4 (not) needed
#define ICW1_SINGLE    0x02 // Single (cascade) mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4 (8)
#define ICW1_LEVEL     0x08 // Level triggered (edge) mode
#define ICW1_INIT      0x10 // Initialization - required!

#define ICW4_8086       0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM       0x10 // Special fully nested (not)

void Interrupts::set_idt_gate(uint8_t interrupt, uint32_t handler_addr)
{
    m_idt[interrupt].low_offset  = (uint16_t)((handler_addr)&0xFFFF);
    m_idt[interrupt].gdt_cs      = 0x08;
    m_idt[interrupt].reserved    = 0;
    m_idt[interrupt].flags       = 0x8E;
    m_idt[interrupt].high_offset = (uint16_t)(((handler_addr) >> 16) & 0xFFFF);
}

void Interrupts::setup()
{
    set_enable_nmi(false);
    remap_pic();
    install_routines();
    load_idt();
}

void Interrupts::load_idt()
{
    qDebug() << "Loading IDT...";

    RegisterIDT idt_reg;
    idt_reg.base = (uint32_t)m_idt;
    idt_reg.size = 256 * sizeof(GateIDT) - 1;

    qDebug() << "IDT (pointer):" << qHex(idt_reg.base) << qHex(idt_reg.size);

    asm volatile("lidt %0" : : "m"(idt_reg));
}

void Interrupts::install_routines()
{
    // Register our routines.
    qDebug() << "Installing interrupt service routines...";
    Q_REPEAT_PP(64, ISR_ENTRY);

    for (uint8_t i = 64; i < 255; ++i) {
        set_idt_gate(i, (uint32_t)_ignore_interrupt_request);
    }
}

void Interrupts::remap_pic()
{
    qDebug() << "Remapping the PIC...";

    system.ports.write(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    system.ports.write(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    system.ports.write(PIC1_DATA, PIC1_OFFSET);
    system.ports.write(PIC2_DATA, PIC2_OFFSET);

    system.ports.write(PIC1_DATA, 0x04);
    system.ports.write(PIC2_DATA, 0x02);

    system.ports.write(PIC1_DATA, ICW4_8086);
    system.ports.write(PIC2_DATA, ICW4_8086);

    system.ports.write(PIC1_DATA, 0x0);
    system.ports.write(PIC2_DATA, 0x0);
}

void Interrupts::activate()
{
    asm volatile("sti");
}

void Interrupts::set_enable_nmi(bool enable)
{
    system.ports.write(0x70, (enable ? 0xFF & 0x7F : 0xFF | 0x80));
    system.ports.read(0x71);
}

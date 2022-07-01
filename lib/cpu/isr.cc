// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/cpu/isr.h"

#include "lib/system/system.h"

#define PIC_EOI 0x20

// After every interrupt we need to send an EOI to the PICs
// or they will not send another interrupt again.
static inline void end_of_interrupt(uint8_t n)
{
    if (n >= 40) {
        system.ports.write(PIC2_COMMAND, PIC_EOI);
    }

    system.ports.write(PIC1_COMMAND, PIC_EOI);
}

static inline uint32_t hardware_interrupt(uint8_t n, uint32_t esp)
{
    system.receive_irq(static_cast<System::IRQ>(n), esp);
    esp = (uint32_t)system.scheduler.schedule((CPU::State*)esp);
    end_of_interrupt(n);
    return esp;
}

static inline uint32_t exception_interrupt(System::Exception n, uint32_t esp)
{
    switch (n) {
    case System::DivisionByZero:
        qFatal() << "Division By Zero";
        break;
    case System::Debug:
        qDebug() << "Debug";
        break;
    case System::NonMaskableInterrupt:
        qFatal() << "Non Maskable Interrupt";
        break;
    case System::Breakpoint:
        qDebug() << "Breakpoint";
        break;
    case System::IntoDetectedOverflow:
        qFatal() << "Into Detected Overflow";
        break;
    case System::OutofBounds:
        qFatal() << "Out of Bounds";
        break;
    case System::InvalidOpcode:
        qFatal() << "Invalid Opcode";
        break;
    case System::NoCoprocessor:
        qFatal() << "No Coprocessor";
        break;
    case System::DoubleFault:
        qFatal() << "Double Fault";
        break;
    case System::CoprocessorSegmentOverrun:
        qFatal() << "Coprocessor Segment Overrun";
        break;
    case System::BadTSS:
        qFatal() << "Bad TSS";
        break;
    case System::SegmentNotPresent:
        qFatal() << "Segment Not Present";
        break;
    case System::StackFault:
        qFatal() << "Stack Fault";
        break;
    case System::GeneralProtectionFault:
        qFatal() << "General Protection Fault";
        break;
    case System::PageFault:
        qFatal() << "Page Fault";
        break;
    case System::UnknownInterrupt:
        qFatal() << "Unknown Interrupt";
        break;
    case System::CoprocessorFault:
        qFatal() << "Coprocessor Fault";
        break;
    case System::AlignmentCheck:
        qFatal() << "Alignment Check";
        break;
    case System::MachineCheck:
        qFatal() << "Machine Check";
        break;
    default:
        qFatal() << "Reserved";
        break;
    }

    return esp;
}

static inline uint32_t software_interrupt(uint8_t n, uint32_t esp)
{
    system.receive_syscall(static_cast<SystemCall>(n), esp);
    return esp;
}

uint32_t _cpu_interrupt_handler(uint8_t n, uint32_t esp)
{
    if (n > 64 || n <= 0) {
        qError() << "Unsupported interrupt number:" << n << esp;
        return 0;
    }

    if (n < 32) {
        return exception_interrupt(static_cast<System::Exception>(n), esp);
    }

    if (n < (32 + 16)) {
        return hardware_interrupt(n, esp);
    }

    return software_interrupt(n, esp);
}

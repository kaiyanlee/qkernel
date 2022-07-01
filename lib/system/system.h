// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "lib/cpu/cpu.h"
#include "lib/cpu/interrupt.h"
#include "lib/cpu/scheduler.h"
#include "lib/disk/files.h"
#include "lib/memory/memory.h"
#include "lib/ports/io.h"
#include "lib/process/process.h"
#include "lib/shell/shell.h"
#include "lib/terminal/terminal.h"
#include "lib/time/clock.h"

[[maybe_unused]] static struct System
{
public:
    enum Exception
    {
        DivisionByZero            = 0,
        Debug                     = 1,
        NonMaskableInterrupt      = 2,
        Breakpoint                = 3,
        IntoDetectedOverflow      = 4,
        OutofBounds               = 5,
        InvalidOpcode             = 6,
        NoCoprocessor             = 7,
        DoubleFault               = 8,
        CoprocessorSegmentOverrun = 9,
        BadTSS                    = 10,
        SegmentNotPresent         = 11,
        StackFault                = 12,
        GeneralProtectionFault    = 13,
        PageFault                 = 14,
        UnknownInterrupt          = 15,
        CoprocessorFault          = 16,
        AlignmentCheck            = 17,
        MachineCheck              = 18,
        Reserved_0                = 19,
        Reserved_1                = 20,
        Reserved_2                = 21,
        Reserved_3                = 22,
        Reserved_4                = 23,
        Reserved_5                = 24,
        Reserved_6                = 25,
        Reserved_7                = 26,
        Reserved_8                = 27,
        Reserved_9                = 28,
        Reserved_10               = 29,
        Reserved_11               = 30,
        Reserved_12               = 31,
    };

    enum IRQ : uint8_t
    {
        NMI        = 32 + 0,  // Non-maskable Interrupt (NMI); system timer
        Keyboard   = 32 + 1,  // Keyboard controller
        PIC        = 32 + 2,  // Programmable Interrupt Controller (PIC); cascade to IRQ 09
        COM2       = 32 + 3,  // Serial port 2
        COM1       = 32 + 4,  // Serial port 1
        LPT2       = 32 + 5,  // Sound card
        FloppyDisk = 32 + 6,  // Floppy disk controller
        LPT1       = 32 + 7,  // Printer
        CMOS       = 32 + 8,  // System CMOS
        Open01     = 32 + 9,  // Legacy NIC
        Open02     = 32 + 10, // NIC/USB
        Open03     = 32 + 11, // Video adapter/ SCSI host adapter
        Mouse      = 32 + 12, // PS/2 mouse
        FPU        = 32 + 13, // Math coprocessor
        IDE01      = 32 + 14, // Primary IDE/ATA hard disk
        IDE02      = 32 + 15, // Secondary IDE/ATA hard disk
    };

public:
    static inline Files      files;
    static inline Shell      shell;
    static inline Terminal   terminal;
    static inline Clock      clock;
    static inline Processes  processes;
    static inline Memory     memory;
    static inline Interrupts interrupts;
    static inline CPU        cpu;
    static inline Scheduler  scheduler;
    static inline Ports      ports;

public:
    static void              receive_syscall(SystemCall syscall, uint32_t esp);
    static void              receive_irq(IRQ irq, uint32_t esp);
    [[noreturn]] static void startup(uintptr_t mb_address, uintptr_t mb_magic);
    [[noreturn]] static void shutdown();

} system;

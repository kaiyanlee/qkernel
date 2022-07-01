// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/system/system.h"

#include "core/logger/logger.h"
#include "core/strings/string.h"
#include "lib/boot/multiboot1.h"
#include "lib/disk/f32.h"
#include "lib/system/export.h"

void System::startup(uintptr_t mb_address, uintptr_t mb_magic)
{
    // Call global constructors.
    __call_global_ctor();

    // Initialize the terminal for rendering new content.
    if (!system.terminal.initialize()) {
        __exit();
    }

    // Clear the terminal to hide QEMU boot messages.
    system.terminal.clear();

    qDebug() << "MB Address:" << qHex(mb_magic) << "MB Magic:" << qHex(mb_address);

    // Check the magic number.
    if (uint64_t expected = MULTIBOOT_BOOTLOADER_MAGIC; mb_address != expected) {
        qFatal() << "Multiboot magic number is incorrect:" << qHex(mb_address) << "Expected:" << qHex(expected);
    }

    // Check address alignment.
    if (mb_magic & 7) {
        qFatal() << "Unaligned address:" << qHex(mb_magic);
    }

    qDebug() << "ELF_START:" << qHex((uint32_t)&_ELF_START_) << "ELF_END:" << qHex((uint32_t)&_ELF_END_);
    qDebug() << "ELF_TEXT_START:" << qHex((uint32_t)&_ELF_TEXT_START_) << "ELF_TEXT_END:" << qHex((uint32_t)&_ELF_TEXT_END_);
    qDebug() << "ELF_READONLY_START:" << qHex((uint32_t)&_ELF_READONLY_START_)
             << "ELF_READONLY_END:" << qHex((uint32_t)&_ELF_READONLY_END_);
    qDebug() << "ELF_DATA_START:" << qHex((uint32_t)&_ELF_DATA_START_) << "ELF_DATA_END:" << qHex((uint32_t)&_ELF_DATA_END_);
    qDebug() << "ELF_BSS_START:" << qHex((uint32_t)&_ELF_BSS_START_) << "ELF_BSS_END:" << qHex((uint32_t)&_ELF_BSS_END_);

    qDebug() << "Setting up memory manager and paging...";
    system.memory.setup((uint32_t)&_ELF_END_);

    qDebug() << "Testing dynamic memory allocation...";
    for (int i = 1; i < 10; ++i) {
        auto a = new int();
        *a     = i;
        qDebug() << a << *a;
        delete a;
        a = nullptr;
        qDebug() << (a == nullptr);
    }

    // Setup start.
    qDebug() << "Starting system setup...";

    qDebug() << "Testing system logger...";

    double b = 2, dx = 123.123123 + mb_address;
    qDebug() << 3.14f << 1232.123123123 << 3.14 << 3.14 << 3.14;
    qDebug() << qBlue_HB << 0.00001 << 102 << 0x10 << "hello" << &b << b << nullptr;
    qDebug() << -10 << dx << 12.19 << 132312.89789 << -132312.829843821323;
    qDebug() << -3.14 << 3.14 << true << qB_Magenta << "Hello (magenta)";
    qDebug() << qBH_Green << Logger::NoSpaces << "A" << 16 << ":" << 12 << 13 << 14 << "B";

    qDebug() << "Getting CPU features...";
    system.cpu.print_features();

    qDebug() << "Setting up IDT/ISRs/IRQs...";
    system.interrupts.setup();

    qDebug() << "Setting up system clock...";
    system.clock.setup();

    // qDebug() << system.clock.boot_time();
    // qDebug() << system.clock.boot_time();
    // qDebug() << system.clock.boot_time();
    // qDebug() << system.clock.current_time();

    qDebug() << "Testing interrupts...";
    asm volatile("int $69");
    asm volatile("int $63");
    asm volatile("int $64");
    asm volatile("int $1");

#ifdef PROCESSOR_x86_32
    qDebug() << "Arch: x86 (i386 / 32-bit)";
#endif

#ifdef PROCESSOR_x86_64
    qDebug() << "Arch: x86-64 (64-bit)";
#endif

#ifdef PROCESSOR_ARMv8
    qDebug() << "Arch: ARMv8 (aarch64 / 64-bit)";
#endif

#ifdef UBSAN_ENABLED
    qDebug() << "UBSan:" << true;
#else
    qDebug() << "UBSan:" << false;
#endif

    qDebug() << "Now activating CPU interrupts...";
    system.interrupts.activate();

    qDebug() << "Setting up file systems...";
    Fat32FileSystem fat32;
    system.files.set_file_system(&fat32);

    qDebug() << "Finishing setup and entering userspace...";

    system.shell.set_splash_screen_enabled(true);

#ifdef ENABLE_TERM_COLOR
    system.shell.set_enable_color(true);
#else
    system.shell.set_enable_color(false);
#endif

    system.shell.add_service(new Shell::Service {
      "exit",
      [](const Shell::Context&) {
          __exit();
          return Shell::Error::NoError;
      },
    });

    system.shell.add_service(new Shell::Service {
      "shutdown",
      [](const Shell::Context&) {
          system.shutdown();
          return Shell::Error::NoError;
      },
    });

    system.shell.add_service(new Shell::Service {
      "help",
      [](const Shell::Context&) {
          auto services = system.shell.services();

          while (*services) {
              qPrint() << "Service:" << (*services)->name;
              ++services;
          }

          return Shell::Error::NoError;
      },
    });

    system.shell.start();
}

void System::receive_syscall(SystemCall syscall, uint32_t)
{
    switch (syscall) {
    case Print:
    case Panic:
    case Halt:
    case Abort:
    case Assert:
    case Exit:
    case Malloc:
    case Free:
    default:
        break;
    }
}

void System::receive_irq(IRQ irq, uint32_t esp)
{
    switch (irq) {
    case COM1:
    case COM2:
        terminal.receive_data(esp);
        break;
    default:
        break;
    }
}

void System::shutdown()
{
    qDebug() << "Shutting down...";

    // Call global destructors.
    __call_global_dtor();

    // Print a newline before termination.
    __print("\n");

    // This function must never return.
    __halt();
}

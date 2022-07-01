# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License v3.0.

# Declare our constants.
# ----------------------------------------------------------------------

.set MB_ALIGN, 1 << 0                     # Align loaded modules on page boundaries.
.set MB_MINFO, 1 << 1                     # Provide memory map.
.set MB_FLAGS, MB_ALIGN | MB_MINFO        # Multiboot flags.
.set MB_MAGIC, 0x1BADB002                 # Magic number used to locate multiboot header.
.set CHECKSUM, -(MB_MAGIC + MB_FLAGS)     # Checksum to prove we are multiboot.

.set CR0_EM, 1 << 2
.set CR0_ET, 1 << 4
.set CR0_NE, 1 << 5
.set CR0_TS, 1 << 3
.set CR0_MP, 1 << 1

.set CR4_OSFXSR,     1 << 9
.set CR4_OSXMMEXCPT, 1 << 10
.set CR4_OSXSAVE,    1 << 18

# External symbols.
# ----------------------------------------------------------------------

.extern _ELF_START_
.extern _ELF_END_

.extern _ELF_TEXT_START_
.extern _ELF_TEXT_END_

.extern _ELF_READONLY_START_
.extern _ELF_READONLY_END_

.extern _ELF_DATA_START_
.extern _ELF_DATA_END_

.extern _ELF_BSS_START_
.extern _ELF_BSS_END_

# Declare the multiboot header that allows us to be recognized
# as a kernel by the bootloader.
# ----------------------------------------------------------------------

.section .multiboot
    .align 4
        .long MB_MAGIC
        .long MB_FLAGS
        .long CHECKSUM

# Provide a stack. The stack is in its own section so it can be marked
# nobits, which means the kernel file is smaller because it does not contain an
# uninitialized stack. The stack on x86 *must* be 16-byte aligned.
# ----------------------------------------------------------------------
.section .bss
    .align 16
        stack_bottom:
            .skip 16384
        stack_top:


# The linker script specifies _start as the entry point to the kernel and
# the bootloader will jump to this position once the kernel has been
# loaded. This function will never return.

.section .text
    .data
        interrupt_number:
            .byte 0x0
        cpu_feat_temp:
            .word 0x55AA
        float_data:
            .word 0x55AA

    .global _ignore_interrupt_request
        _ignore_interrupt_request:
            iret

    .macro _isr i
        .global _isr_\i
        _isr_\i:
            movb $\i, (interrupt_number)
            pushl $0
            jmp _handle_interrupt
    .endm

    .global  _start
    .type _start, @function
        _start:
            # To set up a stack, we set the esp register to point to the top of the
            # stack (as it grows downwards on x86 systems). This is done here in
            # assembly because C++ cannot function without a stack.
            mov $stack_top, %esp
            mov %esp, %ebp

            # Push these before we call _startup.
            pushl %ebx
            pushl %eax

            # Disable interrupts.
            cli

            # Load the GDT descriptor.
            lgdt (gdtr)

            # Update the segment registers
            movw $0x10, %ax
            movw   %ax, %ds
            movw   %ax, %es
            movw   %ax, %fs
            movw   %ax, %gs
            movw   %ax, %ss

            # Far jump: 0x08 is the offset to our code segment.
            jmp $0x08, $_pre_startup

        _pre_startup:
            # Enable FPU coprocessor
            mov %cr0, %eax
            or (-1) - (CR0_TS + CR0_EM), %eax
            mov %eax, %cr0
            fninit
            fnstsw (cpu_feat_temp)
            cmpw $0, (cpu_feat_temp)
            jne _unexpected_loop # No FPU detected...
            fldcw (0x37f)
            fldcw (0x37e)
            fldcw (0x37a)

            # Enable stack protector.
            # Linux uses GS:0x14 to access stack protector value.
            rdtsc
            mov %eax, (0x1014)

            # The arguments for the startup function.
            # Start the kernel.
            call _startup

            # Halt in case _startup unexpectedly returns.
            jmp _unexpected_loop
        
        _unexpected_loop:
            cli
            hlt
            jmp _unexpected_loop

        # Interrupt Descriptor Table (IDT)
        # See: https://wiki.osdev.org/Interrupt_Descriptor_Table

        # Exception interrupts
        # -------------------------------------------------------
        _isr 0
        _isr 1
        _isr 2
        _isr 3
        _isr 4
        _isr 5
        _isr 6
        _isr 7
        _isr 8
        _isr 9
        _isr 10
        _isr 11
        _isr 12
        _isr 13
        _isr 14
        _isr 15
        _isr 16
        _isr 17
        _isr 18
        _isr 19
        _isr 20
        _isr 21
        _isr 22
        _isr 23
        _isr 24
        _isr 25
        _isr 26
        _isr 27
        _isr 28
        _isr 29
        _isr 30
        _isr 31

        # Hardware interrupts
        # -------------------------------------------------------
        _isr 32
        _isr 33
        _isr 34
        _isr 35
        _isr 36
        _isr 37
        _isr 38
        _isr 39
        _isr 40
        _isr 41
        _isr 42
        _isr 43
        _isr 44
        _isr 45
        _isr 46
        _isr 47

        # Software interrupts
        # -------------------------------------------------------
        _isr 48
        _isr 49
        _isr 50
        _isr 51
        _isr 52
        _isr 53
        _isr 54
        _isr 55
        _isr 56
        _isr 57
        _isr 58
        _isr 59
        _isr 60
        _isr 61
        _isr 62
        _isr 63
        _isr 64

        _handle_interrupt:
            # pusha

            # Save registers
            pushl %ebp
            pushl %edi
            pushl %esi
            pushl %edx
            pushl %ecx
            pushl %ebx
            pushl %eax

            # Passed to the handler.
            pushl %esp
            push (interrupt_number)

            # Call handler
            call _cpu_interrupt_handler

            # Switch the stack
            mov %eax, %esp

            # restore registers
            popl %eax
            popl %ebx
            popl %ecx
            popl %edx
            popl %esi
            popl %edi
            popl %ebp

            # popa

            # int $1

            add $4, %esp
            iret

    # Global Descriptor Table (GDT)
    # See: https://wiki.osdev.org/Global_Descriptor_Table
    .align 32
        gdt:
            .quad 0     
        gdt_code_entry:
            .word 0xffff
            .word 0x0   
            .byte 0x0   
            .byte 0x9a  
            .byte 0xcf  
            .byte 0x0   
        gdt_data_entry:
            .word 0xffff 
            .word 0x0    
            .byte 0x0    
            .byte 0x92   
            .byte 0xcf   
            .byte 0x0    
        gdtr:
            .word gdtr - gdt - 1
            .long gdt

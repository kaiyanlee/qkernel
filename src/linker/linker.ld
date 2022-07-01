/* Copyright (c) 2022 Kaiyan M. Lee
 *
 * Invictus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3.0.
 */

/* Set '_start' as our program entry point. */
ENTRY(_start)

/* Declare the memory layout of our program. */
SECTIONS
{
    PROVIDE(_ELF_START_ = . + 0x100000);
    . = _ELF_START_;

    /* The code or text segment (.text).
     *
     * See: https://en.wikipedia.org/wiki/Code_segment
     */
    .text :
    {
        . = ALIGN(0x08);
        KEEP(*(.multiboot))
        PROVIDE(_ELF_TEXT_START_ = .);
        *(.text)
        *(.gnu.linkonce.t*)
        PROVIDE(_ELF_TEXT_END_ = .);
    }

    /* Read-Only data (.rodata).
     *
     * This is the location of global constants.
     */
    .rodata :
    {
        PROVIDE(_ELF_READONLY_START_ = .);
        *(.rodata)
        *(.gnu.linkonce.r*)
        PROVIDE(_ELF_READONLY_END_ = .);
    }

    /* Include the list of initialization functions sorted. 
     *
     * Used for global constructors.
     */
    .init_array :
    {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    }

    /* Include the list of termination functions sorted. 
     *
     * Used for global destructors.
     */
    .fini_array :
    {
        __fini_array_start = .;
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        __fini_array_end = .;
    }

    /* The initialized data segment (.data).
     *
     * Used for initialized static variables. The size is constant but
     * the values can change.
     *
     * See: https://en.wikipedia.org/wiki/Data_segment
     */
    .data :
    {
        PROVIDE(_ELF_DATA_START_ = .);
        *(.data)
        *(.gnu.linkonce.d*)
        PROVIDE(_ELF_DATA_END_ = .);
    }

    /* The block starting symbol segment (.bss).
     *
     * Used for uninitialized static variables and constants. Our kernel
     * will also use it for the stack.
     *
     * See: https://en.wikipedia.org/wiki/BSS_segment
     */
    .bss :
    {
        PROVIDE(_ELF_BSS_START_ = .);
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(.bss)
        *(COMMON)
        PROVIDE(_ELF_BSS_END_ = .);
    }

    . = ALIGN(0x10);
    PROVIDE(_ELF_END_ = .);
}

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/cpu/task.h"

// Reference: wyoos
Task::Task(void entrypoint())
{
    cpu_state         = (CPU::State*)(stack + 4096 - sizeof(CPU::State));
    cpu_state->eax    = 0;
    cpu_state->ebx    = 0;
    cpu_state->ecx    = 0;
    cpu_state->edx    = 0;
    cpu_state->esi    = 0;
    cpu_state->edi    = 0;
    cpu_state->ebp    = 0;
    cpu_state->eip    = (uint32_t)entrypoint;
    cpu_state->cs     = 0x08;
    cpu_state->eflags = 0x202;
}

Task::~Task()
{
}

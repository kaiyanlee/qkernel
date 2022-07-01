// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"
#include "lib/cpu/cpu.h"

class Task
{
public:
    uint8_t     stack[4096];
    CPU::State* cpu_state;

public:
    Task(void entrypoint());
    ~Task();
};

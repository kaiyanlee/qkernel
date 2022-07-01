// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "lib/cpu/task.h"

class Scheduler
{
public:
    explicit Scheduler();
    ~Scheduler();

public:
    bool        add_task(Task* task);
    CPU::State* schedule(CPU::State* cpu_state);

private:
    Task* m_tasks[256];
    int   m_count;
    int   m_current;
};

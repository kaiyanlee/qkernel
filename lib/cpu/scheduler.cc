// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/cpu/scheduler.h"

#include "core/logger/logger.h"

Scheduler::Scheduler()
{
    m_count   = 0;
    m_current = -1;
}

bool Scheduler::add_task(Task* task)
{
    qDebug() << "Add task:" << (uint32_t)task;

    if (m_count >= 256) {
        return false;
    }

    m_tasks[m_count++] = task;

    return true;
}

CPU::State* Scheduler::schedule(CPU::State* cpu_state)
{
    if (m_count <= 0) {
        return cpu_state;
    }

    if (m_current >= 0) {
        m_tasks[m_current]->cpu_state = cpu_state;
    }

    if (++m_current >= m_count) {
        m_current %= m_count;
    }

    return m_tasks[m_current]->cpu_state;
}

Scheduler::~Scheduler()
{
}

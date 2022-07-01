// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/logger/logger.h"

class Clock
{
public:
    struct Time
    {
        uint8_t  century;
        uint8_t  second;
        uint8_t  minute;
        uint8_t  hour;
        uint8_t  day;
        uint8_t  month;
        uint16_t year;

        friend Logger& operator<<(Logger& logger, const Time& time)
        {
            return logger << "Time:" << time.second << time.minute << time.hour << time.day << time.month << time.year << time.century;
        }
    };

public:
    const Time& boot_time() const;
    void        setup();
    clock_t     secs_since_boot() const;
    Time        current_time() const;

private:
    int     get_update_in_progress_flag() const;
    uint8_t get_RTC_register(int reg) const;

    void read_rtc();

private:
    Time m_boot_time;
};

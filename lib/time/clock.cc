// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/time/clock.h"

#include "core/logger/logger.h"
#include "lib/system/system.h"

constexpr int CURRENT_YEAR     = 2022;
constexpr int CENTURY_REGISTER = 0x00;
constexpr int CMOS_DATA        = 0x71;
constexpr int CMOS_ADDRESS     = 0x70;

void Clock::setup()
{
    m_boot_time = current_time();
}

const Clock::Time& Clock::boot_time() const
{
    return m_boot_time;
}

int Clock::get_update_in_progress_flag() const
{
    system.ports.write(CMOS_ADDRESS, 0x0A);
    return (system.ports.read(CMOS_DATA) & 0x80);
}

uint8_t Clock::get_RTC_register(int reg) const
{
    system.ports.write(CMOS_ADDRESS, reg);
    return system.ports.read(CMOS_DATA);
}

Clock::Time Clock::current_time() const
{
    Time time_1;
    Time time_2;

    uint8_t register_b;

    while (get_update_in_progress_flag()) {
    }

    time_1.second = get_RTC_register(0x00);
    time_1.minute = get_RTC_register(0x02);
    time_1.hour   = get_RTC_register(0x04);
    time_1.day    = get_RTC_register(0x07);
    time_1.month  = get_RTC_register(0x08);
    time_1.year   = get_RTC_register(0x09);

    if (CENTURY_REGISTER != 0) {
        time_1.century = get_RTC_register(CENTURY_REGISTER);
    }

    do {
        time_2.second  = time_1.second;
        time_2.minute  = time_1.minute;
        time_2.hour    = time_1.hour;
        time_2.day     = time_1.day;
        time_2.month   = time_1.month;
        time_2.year    = time_1.year;
        time_2.century = time_1.century;

        while (get_update_in_progress_flag()) {
        }

        time_1.second = get_RTC_register(0x00);
        time_1.minute = get_RTC_register(0x02);
        time_1.hour   = get_RTC_register(0x04);
        time_1.day    = get_RTC_register(0x07);
        time_1.month  = get_RTC_register(0x08);
        time_1.year   = get_RTC_register(0x09);

        if (CENTURY_REGISTER != 0) {
            time_1.century = get_RTC_register(CENTURY_REGISTER);
        }
    } while ((time_2.second != time_1.second) || (time_2.minute != time_1.minute) || (time_2.hour != time_1.hour)
             || (time_2.day != time_1.day) || (time_2.month != time_1.month) || (time_2.year != time_1.year)
             || (time_2.century != time_1.century));

    register_b = get_RTC_register(0x0B);

    if (!(register_b & 0x04)) {

        time_1.second = (time_1.second & 0x0F) + ((time_1.second / 16) * 10);
        time_1.minute = (time_1.minute & 0x0F) + ((time_1.minute / 16) * 10);
        time_1.hour   = ((time_1.hour & 0x0F) + (((time_1.hour & 0x70) / 16) * 10)) | (time_1.hour & 0x80);
        time_1.day    = (time_1.day & 0x0F) + ((time_1.day / 16) * 10);
        time_1.month  = (time_1.month & 0x0F) + ((time_1.month / 16) * 10);
        time_1.year   = (time_1.year & 0x0F) + ((time_1.year / 16) * 10);

        if (CENTURY_REGISTER != 0) {
            time_1.century = (time_1.century & 0x0F) + ((time_1.century / 16) * 10);
        }
    }

    if (!(register_b & 0x02) && (time_1.hour & 0x80)) {
        time_1.hour = ((time_1.hour & 0x7F) + 12) % 24;
    }

    if (CENTURY_REGISTER != 0) {
        time_1.year += time_1.century * 100;
    } else {
        time_1.year += (CURRENT_YEAR / 100) * 100;

        if (time_1.year < CURRENT_YEAR) {
            time_1.year += 100;
        }
    }

    return m_boot_time;
}

clock_t Clock::secs_since_boot() const
{
    return 0;
}

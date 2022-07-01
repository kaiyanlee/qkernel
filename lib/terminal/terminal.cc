// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/terminal/terminal.h"

#include "lib/system/system.h"

Terminal::Terminal()
  : m_active(false)
{
}

bool Terminal::initialize()
{
    if (m_active) {
        return false;
    }

    system.ports.write(Ports::COM1 + 1, 0x01);
    system.ports.write(Ports::COM1 + 3, 0x80);
    system.ports.write(Ports::COM1 + 0, 0x03);
    system.ports.write(Ports::COM1 + 1, 0x00);
    system.ports.write(Ports::COM1 + 3, 0x03);
    system.ports.write(Ports::COM1 + 2, 0xC7);

    return true;
}

void Terminal::print(const char* str)
{
    while (*str) {
        while (!(system.ports.read(Ports::COM1 + 5) & 0x20)) {
        }

        system.ports.write(Ports::COM1, *str++);
    }
}

void Terminal::print(const char* str, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        while (!(system.ports.read(Ports::COM1 + 5) & 0x20)) {
        }
        system.ports.write(Ports::COM1, str[i]);
    }
}

int Terminal::serial_received()
{
    return system.ports.read(Ports::COM1 + 5) & 1;
}

char Terminal::read_serial()
{
    while (serial_received() == 0) {
    }

    return system.ports.read(Ports::COM1);
}

void Terminal::clear_read_buffer()
{
    for (int i = 0; i < 255; ++i) {
        if (m_read_buffer[i] == 0) {
            break;
        } else {
            m_read_buffer[i] = 0;
        }
    }
}

const char* Terminal::readline(const char* prompt)
{
    m_reading = true;
    m_count   = 0;

    print(prompt);

    clear_read_buffer();

    while (m_reading) {
        __sleep();
    }

    return m_read_buffer;
}

void Terminal::receive_data(uint32_t)
{
    while (!(system.ports.read(Ports::COM1 + 5) & 0x20)) {
    }

    auto c = read_serial();

    if (c == DEL || c == BS || c == ETB) {
        return backspace();
    }

    if (c == ETX) {
        return cancel();
    }

    if (c == CR) {
        return accept();
    }

    if (c < 31) {
        return; // Unhandled control characters
    }

    m_read_buffer[m_count] = c;
    system.ports.write(Ports::COM1, c);
    ++m_count;
}

void Terminal::backspace()
{
    if (m_count > 0) {
        print("\b \b");
        --m_count;
        m_read_buffer[m_count] = 0;
    }
}

void Terminal::accept()
{
    if (m_count > 0) {
        m_reading = false;
        print("\n");
    }
}

void Terminal::cancel()
{
    m_reading = false;
    clear_read_buffer();
    print("^C\n");
}

void Terminal::clear()
{
    print("\033[2J\033[H");
}

Terminal::~Terminal()
{
}

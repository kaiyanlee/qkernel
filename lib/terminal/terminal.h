// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

class Terminal
{
public:
    enum SpecialCharacters : char
    {
        NUL = 0x00, // Null
        SOH = 0x01, // Start of header
        STX = 0x02, // Start of text
        ETX = 0x03, // End of text
        BS  = 0x08, // Backspace
        CR  = 0x0d, // Return
        ETB = 0x17, // End of transmission block
        DEL = 0x7f, // Delete
    };

public:
    explicit Terminal();
    ~Terminal();

public:
    bool        initialize();
    void        print(const char* str);
    void        print(const char* str, size_t length);
    const char* readline(const char* prompt);

public:
    void clear();
    void receive_data(uint32_t esp);

private:
    int  serial_received();
    char read_serial();
    void clear_read_buffer();

private:
    void accept();
    void backspace();
    void cancel();

private:
    bool m_active;
    int  m_count;
    bool m_reading;
    char m_read_buffer[256];
};

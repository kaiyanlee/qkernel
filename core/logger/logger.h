// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/sys.h"
#include "core/containers/list.h"
#include "core/logger/escape.h"
#include "core/math/math.h"
#include "core/strings/helper.h"
#include "core/types/templates.h"

class Logger
{
public:
    enum Type
    {
        Print = 0,
        Debug = 1,
        Error = 2,
        Fatal = 3,
    };

    struct State
    {
        size_t count;
        size_t depth;
    };

    enum Option
    {
        None     = 0,      // No options (internal use only).
        NoSpaces = 1 << 0, // Do not space out arguments.
        Reset    = 1 << 1, // Reset style (internal use only).
        NoQuotes = 1 << 2, // Do not quote special strings.
    };

private:
    static inline State m_state = { 0, 0 };

private:
    struct SourceLocation
    {
        const char* file;
        const char* function;
        int         line;
    };

private:
    size_t         m_length;
    Type           m_type;
    int            m_options;
    SourceLocation m_src;
    AnsiStyleCode  m_style;

public:
    constexpr Logger(const char* file, const char* func, int line, Type type)
      : m_length(0)
      , m_type(type)
      , m_options(0)
      , m_src({ file, func, line })
      , m_style(qDefault)
    {
        if (m_state.depth != 0 && m_state.count > 1) {
            __print("\n");
        }

        m_state.count += 1;
        m_state.depth += 1;

        if (m_type == Type::Print) {
            __print("    ");
        } else if (m_type == Type::Debug) {
            print_header("[?] ", qB_Black);
        } else {
            m_style = qB_Red;
            print_header("[!] ", m_style);
        }
    }

    ~Logger()
    {
        if (m_options & Option::Reset) {
            print_style(qReset);
        }

        if (m_state.depth == 1) {
            __print("\n");
        }

        m_state.depth -= 1;

        if (m_type == Type::Fatal && m_length > 1) {
            __abort();
        }
    }

public:
    constexpr Logger& log()
    {
        m_length = m_length + 1;
        return *this;
    }

    template<typename T>
    constexpr Logger& operator<<(T)
    {
        return fprint_unknown();
    }

    template<typename T>
    constexpr Logger& operator<<(T* pointer)
    {
        return fprint_address((uintptr_t)pointer);
    }

    template<typename T>
    constexpr Logger& operator<<(qHex<T> hex)
    {
        return fprint_address(hex.value);
    }

    template<>
    constexpr Logger& operator<<(const Logger::SourceLocation& source)
    {
        return fprint_source_location(source);
    }

    template<>
    constexpr Logger& operator<<(AnsiEscapeCode code)
    {
        return fprint_escape(code);
    }

    template<>
    constexpr Logger& operator<<(AnsiStyleCode style)
    {
        return fprint_style(style);
    }

    template<>
    constexpr Logger& operator<<(Logger::Option option)
    {
        return fprint_option(option);
    }

    template<>
    constexpr Logger& operator<<(bool boolean)
    {
        return fprint_boolean(boolean);
    }

    template<>
    constexpr Logger& operator<<(uint64_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(uint32_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(uint16_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(uint8_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(int64_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(int32_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(int16_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(int8_t number)
    {
        return fprint_number(number);
    }

    template<>
    constexpr Logger& operator<<(double number)
    {
        return fprint_double(number);
    }

    template<>
    constexpr Logger& operator<<(float number)
    {
        return fprint_float(number);
    }

    template<>
    constexpr Logger& operator<<(nullptr_t)
    {
        return fprint_nullptr();
    }

    template<>
    constexpr Logger& operator<<(const char* message)
    {
        return fprint_string(message);
    }

    template<>
    constexpr Logger& operator<<(char message)
    {
        const char buffer[] = { message, '\0' };
        return fprint_string(buffer);
    }

private:
    void print_style(const AnsiStyleCode& code)
    {
#ifdef ENABLE_TERM_COLOR
        __print(code.value);
#else
        Q_UNUSED(code);
#endif
    }

private:
    void print_header(const char* text, const AnsiStyleCode& style)
    {
        __print(style.value);
        __print(text);
        char buffer[128];
        recursive_print(m_src.file, ":", m_src.function, ":", int_to_str(m_src.line, buffer));
        __print(qReset.value);
        __print("\n    ");
    }

    void recursive_print(const char* s)
    {
        __print(s);
    }

    template<typename T = const char*, typename... Targs>
    void recursive_print(T& value, Targs... args)
    {
        recursive_print(value);
        recursive_print(args...);
    }

    template<class... Ts>
    Logger& fprint(const AnsiStyleCode& style, Ts... strings)
    {
        if (!(m_options & Option::NoSpaces) && m_length > 1) {
            __print(" ");
        }

#ifdef ENABLE_TERM_COLOR
        __print(style.value);
#endif

        recursive_print(strings...);

#ifdef ENABLE_TERM_COLOR
        __print(qReset.value);
#endif

        return log();
    }

    Logger& fprint_source_location(const SourceLocation& src)
    {
        char buffer[128];
        return fprint(qB_Blue, src.function, src.file, ":", int_to_str(src.line, buffer));
    }

    Logger& fprint_option(Option option)
    {
        switch (option) {
        case Option::NoSpaces:
            m_options |= Option::NoSpaces;
            break;
        case Option::NoQuotes:
            m_options |= Option::NoQuotes;
            break;
        case Option::Reset:
            m_options = Option::None;
        default:
            break;
        }

        return *this;
    }

    Logger& fprint_nullptr()
    {
        return fprint(qB_Red, "nullptr");
    }

    Logger& fprint_escape(const AnsiEscapeCode& code)
    {
        __print(code.value);
        return log();
    }

    Logger& fprint_style(const AnsiStyleCode& code)
    {
#ifdef ENABLE_TERM_COLOR
        if (!(m_options & Option::Reset)) {
            m_options |= Option::Reset;
        }

        m_style = code;
        m_length -= 1;
#else
        Q_UNUSED(code);
#endif
        return log();
    }

    Logger& fprint_boolean(const bool boolean)
    {
        if (boolean) {
            return fprint(qB_Green, "true");
        } else {
            return fprint(qB_Red, "false");
        }
    }

    Logger& fprint_number(const int n)
    {
        char buffer[128];
        return fprint(qB_Blue, int_to_str(n, buffer));
    }

    Logger& fprint_float(float n)
    {
        char b1[256];
        char b2[256];
        return fprint(qB_Blue, int_to_str(qRound(n), b1), ".", fract_to_string(n, b2, 2));
    }

    Logger& fprint_double(double n)
    {
        char b1[256];
        char b2[256];
        return fprint(qB_Blue, int_to_str(qRound(n), b1), ".", fract_to_string(n, b2, 4));
    }

public:
    Logger& fprint_address(const auto addr)
    {
        char buffer[9];
        return fprint(qB_Blue, "0x", hex_to_str(addr, buffer));
    }

    Logger& fprint_string(const char* str)
    {
        return fprint(m_style, str);
    }

    Logger& fprint_unknown()
    {
        return fprint(qB_Red, "<UNKNOWN>");
    }
};

#define qPrint Logger(__FILE__, __FUNCTION__, __LINE__, Logger::Print).log
#define qFatal Logger(__FILE__, __FUNCTION__, __LINE__, Logger::Fatal).log

#ifdef DEBUG_MODE
#define qDebug Logger(__FILE__, __FUNCTION__, __LINE__, Logger::Debug).log
#else
// Noop; this should never execute.
#define qDebug    \
    while (false) \
    Logger
#endif

#define qError Logger(__FILE__, __FUNCTION__, __LINE__, Logger::Error).log

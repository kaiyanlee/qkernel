// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"
#include "core/math/math.h"

constexpr int strcmp(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Check if two strings are equal.
constexpr bool strmatch(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

// Reference: https://stackoverflow.com/a/62492776
template<typename T>
const char* hex_to_str(T n, char* buf)
{
    static const char ref[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', 'E', 'F' };

    auto  hn  = (n >> 4);
    auto* bln = (unsigned char*)&n;
    auto* bhn = (unsigned char*)&hn;
    char* ptr = buf + (sizeof(n) << 1);

    *ptr = 0;

    do {
        *--ptr = ref[(*bln++) & 0xF];
        *--ptr = ref[(*bhn++) & 0xF];
    } while (ptr > buf);

    return buf;
}

template<typename T>
const char* int_to_str(T n, char buf[])
{
    char* b = buf;

    if (n < 0) {
        *b++ = '-';
        n *= -1;
    }

    int tmp = n;
    do {
        ++b;
        tmp /= 10;
    } while (tmp);

    *b = 0;

    do {
        *--b = '0' + (n % 10);
        n /= 10;
    } while (n);

    return buf;
}

template<typename T>
const char* fract_to_string(T f, char* buffer, int round)
{
    static const int  ints[]  = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    static const char chars[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

    int count = 0, y = 0;
    f = qAbs(f) - qRound(f);

    while (f <= 1) {
        f *= 10;

        for (y = 0; y < 10; y++) {
            if (qRound(f) == ints[y]) {
                buffer[count] = chars[y];
                break;
            }
        }

        f -= qRound(f);

        if (f <= 0 || count == round) {
            break;
        }

        ++count;
    }

    while (count > 0 && buffer[count] == '0') {
        buffer[count] = 0;
        --count;
    }

    return buffer;
}

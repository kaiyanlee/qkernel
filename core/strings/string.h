// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"
#include "core/common/sys.h"
#include "core/math/math.h"

struct String
{
    char*  data;
    size_t length;

    constexpr String()
      : data(nullptr)
      , length(0)
    {
    }

    String(const char* d, size_t l)
      : data(new char(l))
      , length(l)
    {
        for (size_t i = 0; i < l; ++i) {
            data[i] = d[i];
        }
    }

    String(const char* d)
    {
        size_t l = 0;

        while (*d) {
            ++l, ++d;
        }

        data = new char[l];

        for (size_t i = 0; i < l; ++i) {
            data[i] = d[i];
        }
    }

    ~String()
    {
        if (data != nullptr) {
            delete data;
        }
    }

    String append(const String& s)
    {
        size_t l = (length + s.length) + 1;
        char*  b = new char[length];
        size_t i, j;

        for (i = 0; i < l; ++i) {
            b[i] = data[i];
        }

        for (j = i, i = 0; j < l; ++j, ++i) {
            b[j] = data[i];
        }

        return String(b, l);
    }
};

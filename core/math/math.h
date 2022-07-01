// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

const double Pi = 3.1415926535;

template<typename T>
struct qHex
{
    const T value;
    constexpr qHex(const T value)
      : value(value)
    {
    }
};

constexpr inline int qRound(double n)
{
    return (n > 0.0) ? (n + 0.05) : (n - 0.05);
}

constexpr inline int qRound(float n)
{
    return (n > 0.0) ? (n + 0.5) : (n - 0.5);
}

template<typename T>
constexpr inline T qMax(T a, T b)
{
    return a > b ? a : b;
}

template<typename T>
constexpr inline T qMin(T a, T b)
{
    return a > b ? b : a;
}

template<typename T>
constexpr inline T qAbs(T v)
{
    return v * ((v > 0) - (v < 0));
}

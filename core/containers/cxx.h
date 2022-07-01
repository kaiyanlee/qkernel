// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

namespace std {

// C++ uses "std::initializer_list" for situations like:
//      for (a : { 1 })

template<class T>
class initializer_list
{
private:
    const T* m_array;
    size_t   m_len;

public:
    constexpr initializer_list(const T* array, size_t length)
      : m_array(array)
      , m_len(length)
    {
    }

public:
    constexpr initializer_list() noexcept
      : m_array(0)
      , m_len(0)
    {
    }

    constexpr size_t size() const noexcept
    {
        return m_len;
    }

    constexpr const T* begin() const noexcept
    {
        return m_array;
    }

    constexpr const T* end() const noexcept
    {
        return begin() + size();
    }
};

}; // namespace std

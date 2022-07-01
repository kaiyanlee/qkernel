// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

template<class T, class U>
struct is_same_type
{
    static const bool value = false;
};

template<class T>
struct is_same_type<T, T>
{
    static const bool value = true;
};

template<class T, class U>
constexpr inline bool same_type()
{
    return is_same_type<T, U>::value;
}

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

class Url
{
public:
    const char* data;

public:
    constexpr Url(const char* url)
      : data(url)
    {
    }
};

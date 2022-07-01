// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/disk/fs.h"

FileSystem::FileSystem(const char* name)
  : m_name(name)
{
}

const char* FileSystem::name() const
{
    return m_name;
}

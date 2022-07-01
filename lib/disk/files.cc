// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/disk/files.h"

#include "core/logger/logger.h"

Files::Files()
{
}

FileSystem* Files::file_system() const
{
    return m_fs;
}

void Files::set_file_system(FileSystem* file_system)
{
    if (m_fs == file_system) {
        return;
    }

    qDebug() << "Using file system:" << file_system->name();
    m_fs = file_system;
}

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "lib/disk/fs.h"

class Files
{
public:
    explicit Files();

public:
    void        set_file_system(FileSystem* file_system);
    FileSystem* file_system() const;

private:
    FileSystem* m_fs;
};

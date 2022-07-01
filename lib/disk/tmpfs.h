// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "lib/disk/fs.h"

class TemporaryFileSystem : public FileSystem
{
public:
    TemporaryFileSystem();

public:
    virtual FileDescriptor open(Url url);
    virtual bool           close(FileDescriptor fd);
    virtual bool           read(FileDescriptor fd, char* data);
    virtual bool           write(FileDescriptor fd, const char* data);
};

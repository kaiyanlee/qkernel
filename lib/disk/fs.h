// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/resource/url.h"

using FileDescriptor = uint32_t;

class FileSystem
{
protected:
    explicit FileSystem(const char* name);

public:
    virtual FileDescriptor open(Url url)                              = 0;
    virtual bool           read(FileDescriptor fd, char* data)        = 0;
    virtual bool           write(FileDescriptor fd, const char* data) = 0;
    virtual bool           close(FileDescriptor fd)                   = 0;
    const char*            name() const;

private:
    const char* m_name;
};

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/disk/tmpfs.h"

TemporaryFileSystem::TemporaryFileSystem()
  : FileSystem("VFS")
{
}

FileDescriptor TemporaryFileSystem::open(Url)
{
    return 0;
}

bool TemporaryFileSystem::close(FileDescriptor)
{
    return true;
}

bool TemporaryFileSystem::read(FileDescriptor, char*)
{
    return true;
}

bool TemporaryFileSystem::write(FileDescriptor, const char*)
{
    return true;
}

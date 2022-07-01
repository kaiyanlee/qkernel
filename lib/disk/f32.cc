// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/disk/f32.h"

Fat32FileSystem::Fat32FileSystem()
  : FileSystem("FAT32")
{
}

FileDescriptor Fat32FileSystem::open(Url)
{
    return 0;
}

bool Fat32FileSystem::close(FileDescriptor)
{
    return true;
}

bool Fat32FileSystem::read(FileDescriptor, char*)
{
    return true;
}

bool Fat32FileSystem::write(FileDescriptor, const char*)
{
    return true;
}

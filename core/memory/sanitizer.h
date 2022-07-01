// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

struct SourceLocation
{
    const char* file_name;
    uint32_t    line;
    uint32_t    column;
};

struct TypeDescriptor
{
    uint16_t type_kind;
    uint16_t type_info;
    char     type_name[1];
};

struct OutOfBoundsInfo
{
    SourceLocation  source;
    TypeDescriptor* array_type;
    TypeDescriptor* index_type;
};

struct OverflowInfo
{
    SourceLocation source;
};

struct MismatchInfo
{
    SourceLocation  source;
    TypeDescriptor* type;
    unsigned char   log_align;
    unsigned char   type_kind;
};

struct FunctionTypeMismatchInfo
{
    const SourceLocation  source;
    const TypeDescriptor* type;
};

struct NonNullReturnInfo
{
    SourceLocation source;
    SourceLocation attr;
};

struct UnreachableInfo
{
    SourceLocation source;
};

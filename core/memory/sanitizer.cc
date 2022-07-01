// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "core/memory/sanitizer.h"

#include "core/logger/logger.h"

extern "C"
{
    void __ubsan_handle_out_of_bounds(OutOfBoundsInfo* info)
    {
        qFatal() << Logger::NoSpaces << "Out of bounds access:" << info->source.file_name << ":" << (int)info->source.line << ":"
                 << (int)info->source.column << "\n"
                 << info->array_type->type_name << info->array_type->type_info << info->array_type->type_kind;
    }

    void __ubsan_handle_missing_return()
    {
        qFatal() << "Missing return";
    }

    void __ubsan_handle_nonnull_return_v1(NonNullReturnInfo* info)
    {
        qFatal() << Logger::NoSpaces << "Non-null return" << info->source.file_name << ":" << (int)info->source.line << ":"
                 << (int)info->source.column;
    }

    void __ubsan_handle_add_overflow()
    {
        qFatal() << "Overflow on addition";
    }

    void __ubsan_handle_sub_overflow()
    {
        qFatal() << "Overflow on subtraction";
    }

    void __ubsan_handle_mul_overflow()
    {
        qFatal() << "Overflow on multiplication";
    }

    void __ubsan_handle_negate_overflow()
    {
        qFatal() << "Overflow on negation";
    }

    void __ubsan_handle_pointer_overflow()
    {
        qFatal() << "Pointer overflow";
    }

    void __ubsan_handle_divrem_overflow(OverflowInfo* info, unsigned long lhs, unsigned long rhs)
    {
        qFatal() << Logger::NoSpaces << "Division remainder overflow: LHS " << lhs << " / " << rhs << "\n\t" << info->source.file_name
                 << ":" << (int)(int)info->source.line << ":" << (int)(int)info->source.column;
    }

    void __ubsan_handle_float_cast_overflow()
    {
        qFatal() << "Float-cast overflow";
    }

    void __ubsan_handle_shift_out_of_bounds()
    {
        qFatal() << "Shift out-of-bounds";
    }

    void __ubsan_handle_type_mismatch_v1(MismatchInfo* info, uint64_t ptr)
    {
        const char* reason    = "Type mismatch";
        const long  alignment = 1 << info->log_align;

        if (alignment && (ptr & (alignment - 1)) != 0) {
            reason = "Misaligned access";
        } else if (ptr == 0) {
            reason = "Null-pointer access";
        }

        qFatal() << Logger::NoSpaces << info->source.file_name << ":" << (int)info->source.line << ":" << (int)info->source.column << "\n\t"
                 << reason << " on ptr " << qHex(ptr) << " (aligned " << alignment << ")\n\t"
                 << "Type Name: " << info->type->type_name;
    }

    void __ubsan_handle_function_type_mismatch_v1(FunctionTypeMismatchInfo* info, uint64_t ptr)
    {
        qFatal() << Logger::NoSpaces << info->source.file_name << ":" << (int)info->source.line << ":" << (int)info->source.column << "\n\t"
                 << "Function type mismatch on ptr " << qHex(ptr) << "\n\t"
                 << "Type Name: " << info->type->type_name << "\n\t"
                 << "Function:" << qHex(ptr); // TODO: Resolve symbol name...
    }

    void __ubsan_handle_nonnull_arg()
    {
        qFatal() << "Non-null argument violated";
    }

    void __ubsan_handle_invalid_builtin()
    {
        qFatal() << "Invalid built-in function";
    }

    void __ubsan_handle_load_invalid_value()
    {
        qFatal() << "Load of invalid value";
    }

    __attribute__((noreturn)) void __ubsan_handle_builtin_unreachable(UnreachableInfo* info)
    {
        qError() << Logger::NoSpaces << "Unreachable code reached: " << info->source.file_name << ":" << (int)info->source.line << ":"
                 << (int)info->source.column;
        __exit();
    }
}

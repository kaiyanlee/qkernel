// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

class Memory
{
public:
    struct Block
    {
        void*  data;
        size_t size;
        Block* next;
        Block* prev;
        bool   free;
    };

public:
    Memory() = default;

public:
    // Allocate memory (a.k.a malloc).
    void* allocate(size_t size);

    // Free memory.
    void free(void* ptr);

public:
    Block* find_block(size_t s, Block** block);
    void   merge_next_block(Block* chunk);
    void   split_next_block(Block* chunk, size_t size);
    void*  base();
    void*  sbrk(uintptr_t increment);

public:
    void setup(uint32_t start_address);
    void create_page_directory();
    void enable_paging();

private:
    uint64_t m_heap_max;
    uint64_t m_heap_start;
    uint64_t m_heap_end;
};

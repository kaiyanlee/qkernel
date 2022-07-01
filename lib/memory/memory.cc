// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/memory/memory.h"

#include "core/logger/logger.h"

static inline size_t word_align(size_t size)
{
    return size + ((sizeof(size_t) - 1) & ~(sizeof(size_t) - 1));
}

void* Memory::allocate(size_t size)
{
    qDebug() << "Allocate:" << size;

    if (!size) {
        return nullptr;
    }

    const auto length = word_align(size + sizeof(Block));

    qDebug() << "Block size:" << length;

    Block* prev  = nullptr;
    Block* block = find_block(size, &prev);

    qDebug() << "Prev block:" << block;

    if (!block) {
        auto* newBlock = (Block*)(sbrk(length));

        if (newBlock == (void*)-1) {
            qFatal() << "New block is not valid";
        }

        newBlock->next = nullptr;
        newBlock->prev = prev;
        newBlock->size = length - sizeof(Block);
        newBlock->data = newBlock + 1;

        qDebug() << "New block data:" << newBlock->data;
        qDebug() << "New block size:" << newBlock->size;

        if (prev != nullptr) {
            prev->next = newBlock;
        }

        qDebug() << "Previous block:" << prev;

        block = newBlock;
    } else if (length + sizeof(size_t) < block->size) {
        split_next_block(block, length);
    }

    block->free = false;

    if (block->data == nullptr) {
        qFatal() << "Memory block is not valid";
    }

    return block->data;
}

Memory::Block* Memory::find_block(size_t s, Block** block)
{
    qDebug() << "Find Block:" << s << block;

    auto c = (Block*)base();
    for (; c && (!c->free || c->size < s); *block = c, c = c->next) {
    }
    return c;
}

void Memory::merge_next_block(Block* block)
{
    qDebug() << "Merge Next Block:" << block;

    block->size = block->size + block->next->size + sizeof(Block);
    block->next = block->next->next;

    if (block->next) {
        block->next->prev = block;
    }
}

void Memory::split_next_block(Block* block, size_t size)
{
    qDebug() << "Split Next Block:" << size << block;

    auto newBlock = (Block*)((char*)(block) + size);

    newBlock->prev = block;
    newBlock->next = block->next;
    newBlock->size = block->size - size;
    newBlock->free = false;
    newBlock->data = newBlock + 1;

    if (block->next) {
        block->next->prev = newBlock;
    }

    block->next = newBlock;
    block->size = size - sizeof(Block);
}

void Memory::free(void* ptr)
{
    qDebug() << "Free:" << ptr;

    if (!ptr || ptr < base() || ptr > sbrk(0)) {
        ptr = nullptr;
        return;
    }

    Block* block = (Block*)(ptr)-1;

    if (block->data != ptr) {
        ptr = nullptr;
        return;
    }

    block->free = true;

    if (block->next && block->next->free) {
        merge_next_block(block);
    }

    if (block->prev->free) {
        merge_next_block(block = block->prev);
    }

    if (!block->next) {
        block->prev->next = nullptr;
        sbrk(-block->size - sizeof(Block));
    }
}

void* Memory::base()
{
    static Block* b = nullptr;

    if (!b) {
        b = (Block*)sbrk(word_align(sizeof(Block)));

        if (b == (void*)-1) {
            qFatal() << "Memory error";
        }

        b->next = nullptr;
        b->prev = nullptr;
        b->size = 0;
        b->free = false;
        b->data = nullptr;
    }

    return b;
}

void* Memory::sbrk(uintptr_t increment)
{
    uintptr_t old = m_heap_end;

    if (m_heap_end + increment <= m_heap_max) {
        m_heap_end += increment;
        return (void*)old;
    }

    return (void*)-1;
}

void Memory::create_page_directory()
{
    qDebug() << "Creating page directory...";
    // TODO...
}

void Memory::enable_paging()
{
    qDebug() << "Enabling paging...";
    // TODO...
}

void Memory::setup(uint32_t start_address)
{
    qDebug() << "Starting memory at:" << qHex(start_address);

    create_page_directory();
    enable_paging();

    m_heap_start = start_address;

    if (m_heap_start & 0xF) {
        m_heap_start += 0x10 - (m_heap_start & 0xF);
    }

    m_heap_end = m_heap_start;

    if ((m_heap_start & 0xF) != 0) {
        qFatal() << "Heap should be aligned";
    }

    m_heap_max = 0xC0000000;
}

// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

template<typename T>
class List
{
public:
    struct Node
    {
        T     data;
        Node* next;
        Node* prev;
    };

public:
    explicit List()
      : m_size(0)
      , m_head(new Node())
    {
        m_head->next = nullptr;
        m_head->prev = nullptr;
    }

public:
    void append(T item)
    {
        m_head->data = item;
        m_head->next = new Node();
        m_head       = m_head->next;
        m_head->next = nullptr;
        m_head->prev = nullptr;

        ++m_size;
    }

    void remove(size_t index)
    {
        auto node = at(index);
        delete node;
        --m_size;
    }

    T at(size_t index) const
    {
        Node* n = m_head;

        while (index > 0) {
            n = n->prev;
            --index;
        }

        return n->data;
    }

    T operator[](size_t index) const
    {
        return at(index);
    }

    size_t size() const
    {
        return m_size;
    }

private:
    size_t m_size;
    Node*  m_head;
};

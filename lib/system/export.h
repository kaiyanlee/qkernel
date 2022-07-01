// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/common/cxx.h"

extern "C"
{
    extern uint8_t _ELF_START_;
    extern uint8_t _ELF_END_;

    extern uint8_t _ELF_TEXT_START_;
    extern uint8_t _ELF_TEXT_END_;

    extern uint8_t _ELF_READONLY_START_;
    extern uint8_t _ELF_READONLY_END_;

    extern uint8_t _ELF_DATA_START_;
    extern uint8_t _ELF_DATA_END_;

    extern uint8_t _ELF_BSS_START_;
    extern uint8_t _ELF_BSS_END_;
}

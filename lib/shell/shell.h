// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

#include "core/macros/repeat.h"
#include "core/strings/string.h"

class Shell
{

public:
    static const uint8_t SERVICE_COUNT = 8;

    // The context of each service.
    struct Context
    {
        int argc;
    };

    // Error code to be returned by each service.
    enum Error
    {
        NoError        = 0,
        NoServiceFound = 1,
    };

    // The available service indexes.
    struct Service
    {
        const char* name;
        Error (*execute)(const Context&);
    };

public:
    explicit Shell();

public:
    bool              initialize();
    [[noreturn]] void start();

    bool color_enabled() const;
    void set_enable_color(bool color);
    bool splash_screen_enabled() const;
    void add_service(Service* service);
    void set_splash_screen_enabled(bool splash);
    void list_services();

    Service** services();

private:
    bool     m_color, m_splash;
    Service* m_services[SERVICE_COUNT];
    uint8_t  m_count;
};

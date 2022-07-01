// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/shell/shell.h"

#include "core/logger/logger.h"
#include "lib/system/system.h"

Shell::Shell()
  : m_color(false)
  , m_splash(false)
  , m_count(0)
{
}

void Shell::add_service(Service* service)
{
    qDebug() << "Enable service:" << service->name;

    if (m_count >= SERVICE_COUNT) {
        qError() << "Service count exceeds maximum number of services";
        return;
    }

    m_services[m_count] = service;
    ++m_count;
}

Shell::Service** Shell::services()
{
    return m_services;
}

bool Shell::initialize()
{
    qDebug() << "Enable Splash Screen:" << splash_screen_enabled();
    qDebug() << "Enable Color:" << color_enabled();

    return true;
}

bool Shell::color_enabled() const
{
    return m_color;
}

void Shell::set_enable_color(bool color)
{
    m_color = color;
}

bool Shell::splash_screen_enabled() const
{
    return m_splash;
}

void Shell::set_splash_screen_enabled(bool splash)
{
    m_splash = splash;
}

void Shell::start()
{
    if (splash_screen_enabled()) {
        qPrint();
        qPrint() << "Welcome to QKernel!";
        qPrint();
        qPrint() << "If you are running this operating system in a headless qemu session";
        qPrint() << "press 'Ctrl-A X' to exit.";
        qPrint();
    }

    int         i;
    const char* line;
    Error       error;

    while (true) {
        line  = system.terminal.readline("[>] ");
        error = Error::NoServiceFound;
        for (i = 0; i < m_count; ++i) {
            auto service = m_services[i];
            if (service && service->name && strmatch(line, service->name)) {
                error = service->execute({ 0 });
            }
        }

        if (error == Error::NoServiceFound) {
            qPrint() << qRed << "No service found to match:" << line;
        }
    }
}

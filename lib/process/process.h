// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

class Process
{
public:
    int pid;
};

class Processes
{
public:
    Processes();

public:
    void start(int pid);
    void kill(int pid);
};

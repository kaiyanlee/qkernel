// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#pragma once

struct AnsiEscapeCode
{
    const char* value;
};

struct AnsiStyleCode
{
    const char* value;
};

#ifdef ENABLE_TERM_COLOR
#define STYLE_CODE(x) \
    AnsiStyleCode     \
    {                 \
        x             \
    }
#else
#define STYLE_CODE(x) \
    AnsiStyleCode     \
    {                 \
    }
#endif

// Default
#define qEndl AnsiEscapeCode { "\n" };
#define qTab  AnsiEscapeCode { "\t" };

// Colors and style
#define qBlack      STYLE_CODE("\033[0;30m") // Regular text
#define qRed        STYLE_CODE("\033[0;31m")
#define qGreen      STYLE_CODE("\033[0;32m")
#define qYellow     STYLE_CODE("\033[0;33m")
#define qBlue       STYLE_CODE("\033[0;34m")
#define qMagenta    STYLE_CODE("\033[0;35m")
#define qCyan       STYLE_CODE("\033[0;36m")
#define qWhite      STYLE_CODE("\033[0;37m")
#define qB_Black    STYLE_CODE("\033[1;30m") // Regular bold text
#define qB_Red      STYLE_CODE("\033[1;31m")
#define qB_Green    STYLE_CODE("\033[1;32m")
#define qB_Yellow   STYLE_CODE("\033[1;33m")
#define qB_Blue     STYLE_CODE("\033[1;34m")
#define qB_Magenta  STYLE_CODE("\033[1;35m")
#define qB_Cyan     STYLE_CODE("\033[1;36m")
#define qB_White    STYLE_CODE("\033[1;37m")
#define qU_Black    STYLE_CODE("\033[4;30m") // Regular underline text
#define qU_Red      STYLE_CODE("\033[4;31m")
#define qU_Green    STYLE_CODE("\033[4;32m")
#define qU_Yellow   STYLE_CODE("\033[4;33m")
#define qU_Blue     STYLE_CODE("\033[4;34m")
#define qU_Magenta  STYLE_CODE("\033[4;35m")
#define qU_Cyan     STYLE_CODE("\033[4;36m")
#define qU_White    STYLE_CODE("\033[4;37m")
#define qBlack_B    STYLE_CODE("\033[40m") // Regular background
#define qRed_B      STYLE_CODE("\033[41m")
#define qGreen_B    STYLE_CODE("\033[42m")
#define qYellow_B   STYLE_CODE("\033[43m")
#define qBlue_B     STYLE_CODE("\033[44m")
#define qMagenta_B  STYLE_CODE("\033[45m")
#define qCyan_B     STYLE_CODE("\033[46m")
#define qWhite_B    STYLE_CODE("\033[47m")
#define qBlack_HB   STYLE_CODE("\033[0;100m ") // High intensity background
#define qRed_HB     STYLE_CODE("\033[0;101m")
#define qGreen_HB   STYLE_CODE("\033[0;102m")
#define qYellow_HB  STYLE_CODE("\033[0;103m")
#define qBlue_HB    STYLE_CODE("\033[0;104m")
#define qMagenta_HB STYLE_CODE("\033[0;105m")
#define qCyan_HB    STYLE_CODE("\033[0;106m")
#define qWhite_HB   STYLE_CODE("\033[0;107m")
#define qH_Black    STYLE_CODE("\033[0;90m") // High intensity text
#define qH_Red      STYLE_CODE("\033[0;91m")
#define qH_Green    STYLE_CODE("\033[0;92m")
#define qH_Yellow   STYLE_CODE("\033[0;93m")
#define qH_Blue     STYLE_CODE("\033[0;94m")
#define qH_Magenta  STYLE_CODE("\033[0;95m")
#define qH_Cyan     STYLE_CODE("\033[0;96m")
#define qH_White    STYLE_CODE("\033[0;97m")
#define qBH_Black   STYLE_CODE("\033[1;90m") // Bold high intensity text
#define qBH_Red     STYLE_CODE("\033[1;91m")
#define qBH_Green   STYLE_CODE("\033[1;92m")
#define qBH_Yellow  STYLE_CODE("\033[1;93m")
#define qBH_Blue    STYLE_CODE("\033[1;94m")
#define qBH_Magenta STYLE_CODE("\033[1;95m")
#define qBH_Cyan    STYLE_CODE("\033[1;96m")
#define qBH_White   STYLE_CODE("\033[1;97m")
#define qReset      STYLE_CODE("\033[0m")
#define qDefault    STYLE_CODE("\033[39;49m")

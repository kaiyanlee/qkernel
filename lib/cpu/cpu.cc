// Copyright (c) 2022 Kaiyan M. Lee
//
// QKernel is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License v3.0.

#include "lib/cpu/cpu.h"

#include "core/logger/logger.h"

#define CPUID_VENDOR_DEFAULT       "????????????"
#define CPUID_VENDOR_OLDAMD        "AMDisbetter!"
#define CPUID_VENDOR_AMD           "AuthenticAMD"
#define CPUID_VENDOR_INTEL         "GenuineIntel"
#define CPUID_VENDOR_VIA           "VIA VIA VIA "
#define CPUID_VENDOR_OLDTRANSMETA  "TransmetaCPU"
#define CPUID_VENDOR_TRANSMETA     "GenuineTMx86"
#define CPUID_VENDOR_CYRIX         "CyrixInstead"
#define CPUID_VENDOR_CENTAUR       "CentaurHauls"
#define CPUID_VENDOR_NEXGEN        "NexGenDriven"
#define CPUID_VENDOR_UMC           "UMC UMC UMC "
#define CPUID_VENDOR_SIS           "SiS SiS SiS "
#define CPUID_VENDOR_NSC           "Geode by NSC"
#define CPUID_VENDOR_RISE          "RiseRiseRise"
#define CPUID_VENDOR_VORTEX        "Vortex86 SoC"
#define CPUID_VENDOR_OLDAO486      "GenuineAO486"
#define CPUID_VENDOR_AO486         "MiSTer AO486"
#define CPUID_VENDOR_ZHAOXIN       "  Shanghai  "
#define CPUID_VENDOR_HYGON         "HygonGenuine"
#define CPUID_VENDOR_ELBRUS        "E2K MACHINE "
#define CPUID_VENDOR_QEMU          "TCGTCGTCGTCG"
#define CPUID_VENDOR_KVM           " KVMKVMKVM  "
#define CPUID_VENDOR_VMWARE        "VMwareVMware"
#define CPUID_VENDOR_VIRTUALBOX    "VBoxVBoxVBox"
#define CPUID_VENDOR_XEN           "XenVMMXenVMM"
#define CPUID_VENDOR_HYPERV        "Microsoft Hv"
#define CPUID_VENDOR_PARALLELS     " prl hyperv "
#define CPUID_VENDOR_PARALLELS_ALT " lrpepyh vr "
#define CPUID_VENDOR_BHYVE         "bhyve bhyve "
#define CPUID_VENDOR_QNX           " QNXQVMBSQG "

#define CPUID_GET_VENDOR_STRING 0
#define CPUID_GET_FEATURES      1

void CPU::cpuid(int code, uint32_t* a, uint32_t* d) const
{
    asm volatile("cpuid" : "=a"(*a), "=d"(*d) : "a"(code) : "ecx", "ebx");
}

void CPU::cpuid_string(int code, uint32_t* where) const
{
    asm volatile("cpuid" : "=a"(*where), "=b"(*(where + 0)), "=d"(*(where + 1)), "=c"(*(where + 2)) : "a"(code));
}

void CPU::print_features() const
{
    uint32_t    eax(0), edx(0);
    const char* s = CPUID_VENDOR_DEFAULT;
    cpuid_string(CPUID_GET_VENDOR_STRING, (uint32_t*)s);
    qDebug() << "CPU_VENDOR:" << s;
    cpuid(CPUID_GET_FEATURES, &eax, &edx);
    qDebug() << "FEATURES:" << edx << "SSE3:" << ((eax & FEAT_ECX_SSE3) == true) << "FPU:" << ((edx & FEAT_EDX_FPU) == true);
}

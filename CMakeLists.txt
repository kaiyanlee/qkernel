# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License v3.0.

cmake_minimum_required(VERSION 3.22.1)

project(qkernel VERSION 0.0.0 LANGUAGES CXX ASM)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

macro(set_opt)
  set(${ARGV0} ${ARGV1} CACHE INTERNAL "")
endmacro()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_opt(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
endif()

# CMake options.
# ----------------------------------------------------------------------------
set_opt(CMAKE_CXX_STANDARD 23)
set_opt(CMAKE_CXX_STANDARD_REQUIRED ON)
set_opt(CMAKE_CXX_EXTENSIONS OFF)
set_opt(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set_opt(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
set_opt(CMAKE_VERBOSE_MAKEFILE OFF)
set_opt(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_opt(CMAKE_COLOR_MAKEFILE ON)
set_opt(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set_opt(CMAKE_UNITY_BUILD OFF)
set_opt(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set_opt(CMAKE_ENABLE_EXPORTS OFF)
set_opt(CMAKE_POSITION_INDEPENDENT_CODE OFF)
set_opt(BUILD_SHARED_LIBS OFF)
set_opt(CMAKE_AUTOGEN_VERBOSE OFF)
set_opt(CMAKE_AUTOMOC_PATH_PREFIX OFF)
set_opt(CMAKE_SKIP_INSTALL_RPATH ON)
set_opt(CMAKE_RULE_MESSAGES OFF)
set_opt(CMAKE_ERROR_DEPRECATED ON)

# ----------------------------------------------------------------------------

# Require clang.
find_program(CLANG_PROGRAM "clang++")
if(CLANG_PROGRAM)
  set_opt(CMAKE_CXX_COMPILER ${CLANG_PROGRAM})
else()
  message(FATAL_ERROR "Only the 'clang' compiler is supported")
endif()

# Require lld.
find_program(LLD_PROGRAM "lld")
if(LLD_PROGRAM)
  set_opt(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
else()
  message(FATAL_ERROR "Only the 'lld' linker is supported")
endif()

# ----------------------------------------------------------------------------

# Options

set_opt(QKERNEL_KERNEL_ARCH "x86")
set_opt(QKERNEL_ENABLE_DEVELOPMENT_MODE ON)
set_opt(QKERNEL_THREAD_SAFE OFF)
set_opt(QKERNEL_ENABLE_COLOR ON)
set_opt(QKERNEL_ENABLE_EXCEPTIONS OFF)
set_opt(QKERNEL_ENABLE_SECURITY ON)

# ----------------------------------------------------------------------------

# Default mode should be debug.
if(NOT CMAKE_BUILD_TYPE)
  set_opt(CMAKE_BUILD_TYPE "Debug")
endif()

# Enable CCache if found.
find_program(CCACHE_PROGRAM "ccache")
if(CCACHE_PROGRAM)
  set_opt(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
else()
  message(WARNING "Failed to find CCache executable")
endif()

# Custom linker script.
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker/linker.ld")
set(KERNEL_ROOT_DIR "${CMAKE_BINARY_DIR}/root")

# Flags only for C++ and not assembly.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz -trigraphs -Wall -Wextra -pedantic")

# Disable exceptions.
if(NOT QKERNEL_ENABLE_EXCEPTIONS)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -fno-non-call-exceptions -fno-exceptions"
  )
endif()

# Enable kernel mode. Note that we have to enable freestanding
# mode to avoid implementing main().
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fno-common -fno-rtti -nodefaultlibs -ffreestanding -fno-builtin -nostdlib -nostdlibinc"
)

# Enable compiler security options.
if(QKERNEL_ENABLE_SECURITY)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wstack-protector -fstack-protector-all -Wformat-security"
  )
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -DUBSAN_ENABLED  -fsanitize=undefined  -fsanitize=null -fsanitize=alignment -fsanitize=function -fsanitize=bounds -fsanitize=enum"
  )
endif()

# Thread safety.
if(NOT QKERNEL_THREAD_SAFE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics")
endif()

# The default link command uses the C++ compiler which does not accept more
# advanced options like linker scripts. By changing this variable we can ask
# CMake to use the lld linker directly.
set(CMAKE_CXX_LINK_EXECUTABLE
    "<CMAKE_LINKER> -o <TARGET> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES>"
)

# Custom linker options (these override the global ones).
list(APPEND
     LINK_OPTIONS
     "-static"
     "-nostdlib"
     "-N"
     "-T"
     "${LINKER_SCRIPT}"
     "-s"
)

# Set assembler flags. We have to use this to avoid warnings for unused
# arguments when using the Clang compiler.
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp")

if(QKERNEL_ENABLE_COLOR)
  add_compile_definitions(ENABLE_TERM_COLOR)
endif()

# Include directories.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------

# Configure executable based on specified processor architecture.
if(QKERNEL_KERNEL_ARCH STREQUAL "x86")
  add_compile_definitions(PROCESSOR_x86_32)

  list(PREPEND LINK_OPTIONS "-melf_i386")

  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -march=pentium -m32 -target i686-pc-linux"
  )
  set(CMAKE_ASM_FLAGS
      "${CMAKE_ASM_FLAGS} -march=pentium -m32 -target i686-pc-linux"
  )
elseif(QKERNEL_KERNEL_ARCH STREQUAL "x86_64")
  message(FATAL_ERROR "TODO: SUPPORT")
elseif(QKERNEL_KERNEL_ARCH STREQUAL "aarch64")
  message(FATAL_ERROR "TODO: SUPPORT")
else()
  message(FATAL_ERROR "Unknown kernel architecture argument")
endif()

# ----------------------------------------------------------------------------

add_subdirectory(core)
add_subdirectory(lib)
add_subdirectory(services)
add_subdirectory(src)

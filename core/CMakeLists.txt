# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License v3.0.

# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License v3.0.

file(GLOB_RECURSE SRC_FILES "*.h" "*.cc")

# The stdlib for services.
add_library(stdc STATIC ${SRC_FILES})
add_library(core::service ALIAS stdc)

# The stdlib for the kernel.
add_library(stdk STATIC ${SRC_FILES})
add_library(core::kernel ALIAS stdk)
target_compile_definitions(stdk PUBLIC ENABLE_KERNEL_MODE)

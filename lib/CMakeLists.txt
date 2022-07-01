# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License v3.0.

file(GLOB_RECURSE SRC_FILES "*.cc")
add_library(kernel STATIC ${SRC_FILES})
target_link_libraries(kernel PUBLIC core::kernel)
